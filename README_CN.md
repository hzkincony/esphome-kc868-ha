# Esphome KC868-HA Component

# 例子

## 一块 ESP32 连接一块 KC868_HA
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/hzkincony/esphome-kc868-ha
      ref: v2.0.1

uart:
  - id: myuart1
    tx_pin: 1
    rx_pin: 3
    baud_rate: 9600

kc868_ha:
  target_relay_controller_addr: 1 # 默认值为 1，对应 HA485_Ctrl 软件上的参数 Target Relay Controller Addr
  switch_adapter_addr: 10 # 默认值为 10，对应 HA485_Ctrl 软件上的参数 Switch Adapter Addr

binary_sensor:
  # binary_sensor 对应 KC868 HA 板上的 K1~K6 产生的状态变化
  # bind_output 必填。对应 HA485_Ctrl 软件上的参数 BindOutput
  # 其他参数参考：https://esphome.io/components/binary_sensor/index.html
  - platform: kc868_ha
    bind_output: 1 # 对应 HA485_Ctrl 软件上的参数 BindOutput
    name: "binary_sensor 1"
  - platform: kc868_ha
    bind_output: 2
    name: "binary_sensor 2"
  - platform: kc868_ha
    bind_output: 3
    name: "binary_sensor 3"
  - platform: kc868_ha
    bind_output: 4
    name: "binary_sensor 4"
  - platform: kc868_ha
    bind_output: 5
    name: "binary_sensor 5"
  - platform: kc868_ha
    bind_output: 6
    name: "binary_sensor 6"

switch:
  # switch 用于控制 KC868 HA 板上的 D1~D6 电平输出
  # bind_output 必填。对应 HA485_Ctrl 软件上的参数 BindOutput
  # 其他参数参考：https://esphome.io/components/switch/index.html
  - platform: kc868_ha
    bind_output: 1
    name: "switch 1"
  - platform: kc868_ha
    bind_output: 2
    name: "switch 2"
  - platform: kc868_ha
    bind_output: 3
    name: "switch 3"
  - platform: kc868_ha
    bind_output: 4
    name: "switch 4"
  - platform: kc868_ha
    bind_output: 5
    name: "switch 5"
  - platform: kc868_ha
    bind_output: 6
    name: "switch 6"
```
![alt text](./images/ha485_assistant_20231014083148.png)

## 一块 ESP32 连接一块 KC868_HA 且修改过 KC868_HA 的各参数
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/hzkincony/esphome-kc868-ha
      ref: v2.0.1

uart:
  - id: myuart1
    tx_pin: 1
    rx_pin: 3
    baud_rate: 9600

kc868_ha:
  target_relay_controller_addr: 2 # 默认值为 1，对应 HA485_Ctrl 软件上的参数 Target Relay Controller Addr
  switch_adapter_addr: 11 # 默认值为 10，对应 HA485_Ctrl 软件上的参数 Switch Adapter Addr

binary_sensor:
  # binary_sensor 对应 KC868 HA 板上的 K1~K6 产生的状态变化
  # bind_output 必填。对应 HA485_Ctrl 软件上的参数 BindOutput
  # 其他参数参考：https://esphome.io/components/binary_sensor/index.html
  - platform: kc868_ha
    bind_output: 121 # 对应 HA485_Ctrl 软件上的参数 BindOutput
    name: "binary_sensor 1"
  - platform: kc868_ha
    bind_output: 122
    name: "binary_sensor 2"
  - platform: kc868_ha
    bind_output: 123
    name: "binary_sensor 3"
  - platform: kc868_ha
    bind_output: 124
    name: "binary_sensor 4"
  - platform: kc868_ha
    bind_output: 125
    name: "binary_sensor 5"
  - platform: kc868_ha
    bind_output: 126
    name: "binary_sensor 6"

switch:
  # switch 用于控制 KC868 HA 板上的 D1~D6 电平输出
  # bind_output 必填。对应 HA485_Ctrl 软件上的参数 BindOutput
  # 其他参数参考：https://esphome.io/components/switch/index.html
  - platform: kc868_ha
    bind_output: 121
    name: "switch 1"
  - platform: kc868_ha
    bind_output: 122
    name: "switch 2"
  - platform: kc868_ha
    bind_output: 123
    name: "switch 3"
  - platform: kc868_ha
    bind_output: 124
    name: "switch 4"
  - platform: kc868_ha
    bind_output: 125
    name: "switch 5"
  - platform: kc868_ha
    bind_output: 126
    name: "switch 6"
```
![alt text](./images/ha485_assistant_20231014083550.png)

## 一块 ESP32 连接多块 KC868_HA
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/hzkincony/esphome-kc868-ha
      ref: v2.0.1

uart:
  - id: myuart1
    tx_pin: 1
    rx_pin: 3
    baud_rate: 9600

kc868_ha:
  - id: kc868_ha_1 # 定义第 1 块 KC868 HA 板
    target_relay_controller_addr: 1
    switch_adapter_addr: 10
  - id: kc868_ha_2 # 定义第 2 块 KC868 HA 板
    target_relay_controller_addr: 1
    switch_adapter_addr: 11

binary_sensor:
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1 # 对应第 1 块 KC868 HA 板
    bind_output: 1
    name: "binary_sensor 1"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 2
    name: "binary_sensor 2"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 3
    name: "binary_sensor 3"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 4
    name: "binary_sensor 4"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 5
    name: "binary_sensor 5"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 6
    name: "binary_sensor 6"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2 # 对应第 2 块 KC868 HA 板
    bind_output: 7
    name: "binary_sensor 7"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 8
    name: "binary_sensor 8"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 9
    name: "binary_sensor 9"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 10
    name: "binary_sensor 10"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 11
    name: "binary_sensor 11"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 12
    name: "binary_sensor 12"

switch:
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1 # 对应第 1 块 KC868 HA 板
    bind_output: 1
    name: "switch 1"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 2
    name: "switch 2"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 3
    name: "switch 3"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 4
    name: "switch 4"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 5
    name: "switch 5"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_1
    bind_output: 6
    name: "switch 6"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2 # 对应第 2 块 KC868 HA 板
    bind_output: 7
    name: "switch 7"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 8
    name: "switch 8"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 9
    name: "switch 9"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 10
    name: "switch 10"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 11
    name: "switch 11"
  - platform: kc868_ha
    kc868_ha_id: kc868_ha_2
    bind_output: 12
    name: "switch 12"
