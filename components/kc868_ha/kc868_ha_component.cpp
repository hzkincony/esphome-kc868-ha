#include "kc868_ha_component.h"
#include "math.h"
#include <cstring>

namespace esphome {
  namespace kc868_ha {

    static const char* const TAG = "kc868_ha";

    void KC868HaComponent::setup() {
      ESP_LOGD(TAG, "KC868HaComponent::setup");
    }

    // Enjoyliving patch (2026-10-19): the upstream loop() read fixed 21-byte chunks with no resync and,
    // on a CRC failure, flushed the whole RX buffer. Any foreign frame on the HAv2 bus (a Modbus sensor
    // poll/reply, an env frame) therefore cost the next button press. This version buffers bytes and
    // slides one byte at a time until it finds a CRC-valid HAv2 button frame:
    //   [ctrl] 10 00 [panel] 00 06 0C  (6 x [relay+100][action])  [crc lo][crc hi]
    void KC868HaComponent::loop() {
      uint32_t now = millis();
      if (!this->rx_.empty() && available() == 0 && now - this->last_rx_ms_ > 200) {
        // A partial frame that never completed: drop it. Only when nothing is waiting in the UART, so a loop
        // stall mid-frame never discards the bytes already read. Tidy-up only; header + CRC resync covers the rest.
        ESP_LOGV(TAG, "dropping %u stale bytes", (unsigned) this->rx_.size());
        this->rx_.clear();
      }
      while (available() > 0) {
        this->rx_.push_back(read());
        this->last_rx_ms_ = now;
      }
      if (this->rx_.size() > 256)
        this->rx_.erase(this->rx_.begin(), this->rx_.end() - 256);

      while (this->rx_.size() >= 21) {
        uint8_t *d = this->rx_.data();
        bool header = d[1] == 0x10 && d[2] == 0x00 && d[4] == 0x00 && d[5] == 0x06 && d[6] == 0x0C;
        if (header) {
          uint16_t crc = crc16(d, 19);
          if (d[19] == (crc & 0xFF) && d[20] == (crc >> 8)) {
            uint8_t frame[21];
            memcpy(frame, d, 21);
            this->rx_.erase(this->rx_.begin(), this->rx_.begin() + 21);
            this->handle_frame_(frame);
            continue;
          }
        }
        this->rx_.erase(this->rx_.begin());  // not a valid frame start: slide one byte
      }
    }

    void KC868HaComponent::handle_frame_(uint8_t *data) {
      ESP_LOGD(TAG, "uart bus receive %s", format_uart_data(data, 21));
      for (auto & element : this->binary_sensors_) {
        ESP_LOGD(TAG, "dump kc868_ha config, target_relay_controller_addr=%d, switch_adapter_addr=%d, bind_output=%d", 
            element->get_target_relay_controller_addr(),
            element->get_switch_adapter_addr(),
            element->get_bind_output());
      }

      for (auto & element : this->binary_sensors_)
        {
          if (element->get_target_relay_controller_addr() == data[0] &&
              element->get_switch_adapter_addr() == data[3]) {

            ESP_LOGD(TAG, "found a kc868_ha board, target_relay_controller_addr=%d, switch_adapter_addr=%d, bind_output=%d", 
            element->get_target_relay_controller_addr(),
            element->get_switch_adapter_addr(),
            element->get_bind_output());
            for (int i = 7; i <= 17; i += 2) {
              if (data[i] == (element->get_bind_output() + 100)) {
                if (data[i+1] == 1) {
                  element->publish_state(true);
                } else if (data[i+1] == 2) {
                  element->publish_state(false);
                }
              }
            }
          }
        }
    }

    void KC868HaComponent::dump_config(){
      ESP_LOGCONFIG(TAG, "KC868HaComponent::dump_config");
    }

    void KC868HaBinarySensor::setup() {
      ESP_LOGD(TAG, "KC868HaBinarySensor::setup");
      this->publish_initial_state(false);
    }
    void KC868HaBinarySensor::dump_config(){
      ESP_LOGCONFIG(TAG, "KC868HaBinarySensor::dump_config");
    }

    void KC868HaSwitch::setup() {
      ESP_LOGD(TAG, "KC868HaSwitch::setup");

      bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);

      if (initial_state) {
        this->turn_on();
      } else {
        this->turn_off();
      }
    }
    void KC868HaSwitch::dump_config(){
      ESP_LOGCONFIG(TAG, "KC868HaSwitch::dump_config");
    }

    void KC868HaSwitch::write_state(bool state) {

      uint8_t data[21] = {  this->get_target_relay_controller_addr(), 0x03, 0x12, 0x55, 0xBB,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

      int channel = this->get_bind_output();
      int byteIndex = (channel - 1) / 8; // determine which byte to modify
      int bitPosition = (channel - 1) % 8; // determine which bit to modify

      for (auto &element : *(this->switches_)) {
        if ((element->get_target_relay_controller_addr() == this->get_target_relay_controller_addr()) &&
            (element->get_bind_output() != this->get_bind_output())) {
          int elementChannel = element->get_bind_output();
          int elementByteIndex = (elementChannel - 1) / 8;
          int elementBitPosition = (elementChannel - 1) % 8;
          if (element->state == true) {
            data[20 - elementByteIndex] |= (1 << elementBitPosition); // set the bit
          } else {
            data[20 - elementByteIndex] &= ~(1 << elementBitPosition); // clear the bit
          }
        }
      }

      if (state == true) {
        data[20 - byteIndex] |= (1 << bitPosition); // set the bit
      } else {
        data[20 - byteIndex] &= ~(1 << bitPosition); // clear the bit
      }

      uint16_t crc = crc16(data, sizeof(data));
      uint8_t crc_h = static_cast<uint8_t>(crc & 0x00FF);
      uint8_t crc_l = static_cast<uint8_t>((crc & 0xFF00) >> 8);
      uint8_t uart_data[23] = {data[0], data[1], data[2], data[3],
                               data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11],
                               data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19], data[20],
                               crc_h, crc_l};

      this->uart_->write_array(uart_data, sizeof(uart_data));

      ESP_LOGD(TAG, "uart bus send %s", format_uart_data(uart_data, 23));
      this->publish_state(state);
    }

    uint16_t crc16(uint8_t *data, uint8_t length) {
      uint16_t crc = 0xFFFF;

      for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 8; j > 0; j--) {
          if (crc & 0x0001) {
            crc = (crc >> 1) ^ 0xA001;
          } else {
            crc >>= 1;
          }
        }
      }

      return crc;
    }

    char* format_uart_data(uint8_t *uart_data, int length) {
      static char str[256] = {0};  // Output buffer
      char tmp[10];  // Temporary buffer

      str[0] = '\0';  // Clear the buffer
      for (int i = 0; i < length; i++) {
        sprintf(tmp, "%x:", uart_data[i]);
        strcat(str, tmp);  // Append to str
      }

      str[strlen(str)-1] = '\0';  // Replace the last colon with a null terminator

      return str;
    }


  }
}
