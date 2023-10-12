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
    void set_addr(uint8_t addr) { this->addr = addr; };
    uint8_t get_addr() { return this->addr; };
    void set_key(uint8_t key) { this->key = key;};
    uint8_t get_key() { return this->key;};
    void setup() override;
    void dump_config() override;

    protected:
    uint8_t addr;
    uint8_t key;
};

class KC868HaSwitch : public Component, public switch_::Switch {
    public:
    void set_addr(uint8_t addr) { this->addr = addr; };
    uint8_t get_addr() { return this->addr; };
    void set_channel(uint8_t channel) { this->channel = channel;};
    uint8_t get_channel() { return this->channel;};
    void set_uart(uart::UARTComponent *uartComponent) { this->uart_ = uartComponent;};
    void setup() override;
    void write_state(bool state) override;
    void dump_config() override;
    void set_switches(std::vector<kc868_ha::KC868HaSwitch *>* switches_)  { this->switches_ = switches_; };

    protected:
    uart::UARTComponent *uart_;
    uint8_t addr;
    uint8_t channel;
    std::vector<kc868_ha::KC868HaSwitch *>* switches_;
};

class KC868HaComponent : public Component, public uart::UARTDevice {
    public:
    KC868HaComponent(uart::UARTComponent *uartComponent) : UARTDevice(uartComponent) {
        this->uart_ = uartComponent;
    }
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
    std::vector<kc868_ha::KC868HaBinarySensor *> binary_sensors_;
    std::vector<kc868_ha::KC868HaSwitch *> switches_;
};



}
}
