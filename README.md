# Esphome KC868-HA Component

# Core yaml
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/hzkincony/esphome-kc868-ha
      ref: v2.0.0

uart:
  - id: myuart1
    tx_pin: 1
    rx_pin: 3
    baud_rate: 9600

kc868_ha:
  target_relay_controller_addr: 1 # The default value is 1, corresponding to the parameter Target Relay Controller Addr in HA485_Ctrl software
  switch_adapter_addr: 10 # The default value is 10, corresponding to the parameter Switch Adapter Addr in HA485_Ctrl software

binary_sensor:
  - platform: kc868_ha
    bind_output: 1 # Corresponds to the BindOutput parameter in HA485_Ctrl software
    name: "binary_sensor 1"
  - platform: kc868_ha
    bind_output: 2
    name: "binary_sensor 2"

switch:
  - platform: kc868_ha
    bind_output: 1
    name: "switch 1"
  - platform: kc868_ha
    bind_output: 2
    name: "switch 2"
