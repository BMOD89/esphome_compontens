#pragma once

#include "esphome/components/d0/d0.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../constants.h"

namespace esphome {
namespace d0 {

class D0TextSensor : public D0Listener, public text_sensor::TextSensor, public Component {
 public:
  D0TextSensor(std::string server_id, std::string obis_code);
  void publish_val(const D0Node &obis_info) override;
  void dump_config() override;
};

}  // namespace d0
}  // namespace esphome
