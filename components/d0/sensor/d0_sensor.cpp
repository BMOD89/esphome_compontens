#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "d0_sensor.h"
#include "../d0_parser.h"

namespace esphome {
namespace d0 {

static const char *const TAG = "d0_sensor";

D0Sensor::D0Sensor(std::string server_id, std::string obis_code)
    : D0Listener(std::move(server_id), std::move(obis_code)) {}

void D0Sensor::publish_val(const D0Node &obis_info) {
  auto delimiter = find(obis_info.value.begin(), obis_info.value.end(), '*');
  if (delimiter != obis_info.value.end())
  {
    optional<float> y = parse_number<float>(std::string(obis_info.value.begin(), delimiter));
    publish_state(y.value());
  }
  else
    ESP_LOGD(TAG, "Warning no Value found");
}

void D0Sensor::dump_config() { LOG_SENSOR("  ", "D0", this); }

}  // namespace d0
}  // namespace esphome
