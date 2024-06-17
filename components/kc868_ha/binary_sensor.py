import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from . import Kc868HaComponent, KC868_HA_ID

DEPENDENCIES = ['kc868_ha']

kc868_ha_ns = cg.esphome_ns.namespace('kc868_ha')
BinarySensor = kc868_ha_ns.class_('KC868HaBinarySensor', cg.Component, binary_sensor.BinarySensor)

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(BinarySensor),
    cv.GenerateID(KC868_HA_ID): cv.use_id(Kc868HaComponent),
    cv.Optional("target_relay_controller_addr", default=1): cv.int_range(min=1, max=128),
    cv.Optional("switch_adapter_addr", default=10): cv.int_range(min=10, max=99),
    cv.Required("bind_output"): cv.int_range(min=1, max=128),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    paren = yield cg.get_variable(config[KC868_HA_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield binary_sensor.register_binary_sensor(var, config)
    cg.add(var.set_target_relay_controller_addr(config["target_relay_controller_addr"]))
    cg.add(var.set_switch_adapter_addr(config["switch_adapter_addr"]))
    cg.add(var.set_bind_output(config["bind_output"]))
    cg.add(paren.register_binary_sensor(var))
