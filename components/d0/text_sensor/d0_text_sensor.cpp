#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "d0_text_sensor.h"
#include "../d0_parser.h"

namespace esphome {
namespace d0 {

static const char *const TAG = "d0_text_sensor";

D0TextSensor::D0TextSensor(std::string server_id, std::string obis_code)
    : D0Listener(std::move(server_id), std::move(obis_code)) {}

void D0TextSensor::publish_val(const D0Node &obis_info) {
  publish_state(std::string(obis_info.value.begin(), obis_info.value.end()));
}

void D0TextSensor::dump_config() { LOG_TEXT_SENSOR("  ", "D0", this); }

}  // namespace d0
}  // namespace esphome
