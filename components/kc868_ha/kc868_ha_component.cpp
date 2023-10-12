#include "kc868_ha_component.h"
#include "math.h"

namespace esphome {
namespace kc868_ha {

void KC868HaComponent::setup() {
    ESP_LOGD(TAG, "KC868HaComponent::setup");
}

void KC868HaComponent::loop() {
    while(available() >= 21) {
        uint8_t data[21];
        for (int i = 0; i <= 20; i++) {
            uint8_t c = read();
            data[i] = c;
        }

        ESP_LOGD(TAG, "uart bus receive %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
        data[0], data[1], data[2], data[3], data[4],
        data[5], data[6], data[7], data[8], data[9],
        data[10], data[11], data[12], data[13], data[14],
        data[15], data[16], data[17], data[18], data[19],
        data[20], data[21]);

        uint8_t crc_data[19] =  {
            data[0],data[1],data[2],data[3],
            data[4],data[5],data[6],data[7],
            data[8],data[9],data[10],data[11],
            data[12],data[13],data[14],data[15],
            data[16],data[17],data[18]};
        uint16_t crc = crc16(crc_data, sizeof(crc_data));

        if ((data[19] != (crc & 0x00FF)) && (data[19] != ((crc & 0xFF00) >> 8))) {
            ESP_LOGD(TAG, "crc check failed. ignore data");

            while(available() > 0) {
                read();
            }

            return;
        }

        for (auto & element : this->binary_sensors_)
        {
            if (element->get_addr() == data[0]) {
                if (data[7] == (element->get_key() + 100) && data[8] == 1) {
                    element->publish_state(true);
                }
                if (data[7] == (element->get_key() + 100) && data[8] == 2) {
                    element->publish_state(false);
                }
                if (data[9] == (element->get_key() + 100) && data[10] == 1) {
                    element->publish_state(true);
                }
                if (data[9] == (element->get_key() + 100) && data[10] == 2) {
                    element->publish_state(false);
                }
                if (data[11] == (element->get_key() + 100) && data[12] == 1) {
                    element->publish_state(true);
                }
                if (data[11] == (element->get_key() + 100) && data[12] == 2) {
                    element->publish_state(false);
                }
                if (data[13] == (element->get_key() + 100) && data[14] == 1) {
                    element->publish_state(true);
                }
                if (data[13] == (element->get_key() + 100) && data[14] == 2) {
                    element->publish_state(false);
                }
                if (data[15] == (element->get_key() + 100) && data[16] == 1) {
                    element->publish_state(true);
                }
                if (data[15] == (element->get_key() + 100) && data[16] == 2) {
                    element->publish_state(false);
                }
                if (data[17] == (element->get_key() + 100) && data[18] == 1) {
                    element->publish_state(true);
                }
                if (data[17] == (element->get_key() + 100) && data[18] == 2) {
                    element->publish_state(false);
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
    this->publish_state(false);
}
void KC868HaBinarySensor::dump_config(){
    ESP_LOGCONFIG(TAG, "KC868HaBinarySensor::dump_config");
}

void KC868HaSwitch::setup() {
    ESP_LOGD(TAG, "KC868HaSwitch::setup");
}
void KC868HaSwitch::dump_config(){
    ESP_LOGCONFIG(TAG, "KC868HaSwitch::dump_config");
}

void KC868HaSwitch::write_state(bool state) {

    uint8_t data[21] = {  this->get_addr(), 0x03, 0x12, 0x55, 0xBB,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t d = 0;
    for (auto & element : *(this->switches_)) {
        if ((element->get_addr() == this->get_addr()) && (element->get_channel() != this->get_channel())) {
            if (element->state == true) {
                d = d + pow(2,(element->get_channel() - 1));
            }
        }
    }

    if (state == true) {
        d = d + pow(2,(this->get_channel() - 1));
    }

    data[20] = d;
    uint16_t crc = crc16(data, sizeof(data));
    uint8_t uart_data[23] = {data[0],data[1],data[2],data[3],
        data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],
        data[12],data[13],data[14],data[15],data[16],data[17],data[18],data[19],data[20],
        (crc & 0x00FF), (crc & 0xFF00) >> 8};

    this->uart_->write_array(uart_data, sizeof(uart_data));

    ESP_LOGD(TAG, "uart bus send %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x",
        uart_data[0], uart_data[1], uart_data[2], uart_data[3], uart_data[4],
        uart_data[5], uart_data[6], uart_data[7], uart_data[8], uart_data[9],
        uart_data[10], uart_data[11], uart_data[12], uart_data[13], uart_data[14],
        uart_data[15], uart_data[16], uart_data[17], uart_data[18], uart_data[19],
        uart_data[20], uart_data[21], uart_data[22]);

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

}
}
