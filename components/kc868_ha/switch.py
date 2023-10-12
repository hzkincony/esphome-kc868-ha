import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from . import Kc868HaComponent, KC868_HA_ID

DEPENDENCIES = ['kc868_ha']

kc868_ha_ns = cg.esphome_ns.namespace('kc868_ha')
kc868HaSwitch = kc868_ha_ns.class_('KC868HaSwitch', switch.Switch)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(kc868HaSwitch),
    cv.GenerateID(KC868_HA_ID): cv.use_id(Kc868HaComponent),
    cv.Required("addr"): cv.int_,
    cv.Required("channel"): cv.int_range(min=1, max=6),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    paren = yield cg.get_variable(config[KC868_HA_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    yield switch.register_switch(var, config)
    cg.add(var.set_addr(config["addr"]))
    cg.add(var.set_channel(config["channel"]))
    cg.add(paren.register_switch(var))
