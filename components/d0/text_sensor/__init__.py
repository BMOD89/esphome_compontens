import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

from .. import CONF_OBIS_CODE, CONF_SERVER_ID, CONF_D0_ID, D0, obis_code, d0_ns

D0TextSensor = d0_ns.class_("D0TextSensor", text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(D0TextSensor),
        cv.GenerateID(CONF_D0_ID): cv.use_id(D0),
        cv.Required(CONF_OBIS_CODE): obis_code,
        cv.Optional(CONF_SERVER_ID, default=""): cv.string,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_SERVER_ID],
        config[CONF_OBIS_CODE],
    )
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)
    d0 = await cg.get_variable(config[CONF_D0_ID])
    cg.add(d0.register_d0_listener(var))
