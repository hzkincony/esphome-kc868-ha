#pragma once

#include "esphome.h"

#define TAG "kc868_ha.component"

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
  namespace kc868_ha {

    class KC868HaBinarySensor : public Component, public binary_sensor::BinarySensor {
    public:
      void set_target_relay_controller_addr(uint8_t addr) { this->target_relay_controller_addr_=addr; };
      uint8_t get_target_relay_controller_addr() { return this->target_relay_controller_addr_; };
      void set_switch_adapter_addr(uint8_t addr) { this->switch_adapter_addr_ = addr; };
      uint8_t get_switch_adapter_addr() { return this->switch_adapter_addr_; };
      void set_bind_output(uint8_t bind_output) { this->bind_output_ = bind_output; };
      uint8_t get_bind_output() { return this->bind_output_; };
      void setup() override;
      void dump_config() override;

    protected:
      uint8_t target_relay_controller_addr_;
      uint8_t switch_adapter_addr_;
      uint8_t bind_output_;
    };

    class KC868HaSwitch : public Component, public switch_::Switch {
    public:
      void set_uart(uart::UARTComponent *uartComponent) { this->uart_ = uartComponent;};
      void set_target_relay_controller_addr(uint8_t addr) { this->target_relay_controller_addr_=addr; };
      uint8_t get_target_relay_controller_addr() { return this->target_relay_controller_addr_; };
      void set_switch_adapter_addr(uint8_t addr) { this->switch_adapter_addr_ = addr; };
      uint8_t get_switch_adapter_addr() { return this->switch_adapter_addr_; };
      void set_bind_output(uint8_t bind_output) { this->bind_output_ = bind_output; };
      uint8_t get_bind_output() { return this->bind_output_; };

      void setup() override;
      void write_state(bool state) override;
      void dump_config() override;
      void set_switches(std::vector<kc868_ha::KC868HaSwitch *>* switches_)  { this->switches_ = switches_; };

    protected:
      uart::UARTComponent *uart_;
      uint8_t target_relay_controller_addr_;
      uint8_t switch_adapter_addr_;
      uint8_t bind_output_;
      std::vector<kc868_ha::KC868HaSwitch *>* switches_;
    };

    class KC868HaComponent : public Component, public uart::UARTDevice {
    public:
      KC868HaComponent(uart::UARTComponent *uartComponent) : UARTDevice(uartComponent) {
        this->uart_ = uartComponent;
      }
      //void set_target_relay_controller_addr(uint8_t addr) { this->target_relay_controller_addr_=addr; };
      //uint8_t get_target_relay_controller_addr() { return this->target_relay_controller_addr_; };
      //void set_switch_adapter_addr(uint8_t addr) { this->switch_adapter_addr_=addr; };
      //uint8_t get_switch_adapter_addr() { return this->switch_adapter_addr_; };
      void register_binary_sensor(kc868_ha::KC868HaBinarySensor *obj)  { this->binary_sensors_.push_back(obj); };
      void register_switch(kc868_ha::KC868HaSwitch *obj)  {
        obj->set_uart(this->uart_);
        obj->set_switches(&(this->switches_));
        this->switches_.push_back(obj);
      };
      void setup() override;
      void loop() override;
      void dump_config() override;

    protected:
      uart::UARTComponent *uart_;
      uint8_t target_relay_controller_addr_;
      uint8_t switch_adapter_addr_;
      std::vector<kc868_ha::KC868HaBinarySensor *> binary_sensors_;
      std::vector<kc868_ha::KC868HaSwitch *> switches_;
    };

    uint16_t crc16(uint8_t *data, uint8_t length);
    char* format_uart_data(uint8_t *uart_data, int length);
  }
}
