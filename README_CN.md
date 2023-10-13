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
  target_relay_controller_addr: 1 # 默认值为 1，对应 HA485_Ctrl 软件上的参数 Target Relay Controller Addr
  switch_adapter_addr: 10 # 默认值为 10，对应 HA485_Ctrl 软件上的参数 Switch Adapter Addr

binary_sensor:
  - platform: kc868_ha
    bind_output: 1 # 对应 HA485_Ctrl 软件上的参数 BindOutput
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
```