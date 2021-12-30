#pragma once
#include "esphome/components/d0/d0.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace d0 {

class D0Sensor : public D0Listener, public sensor::Sensor, public Component {
 public:
  D0Sensor(std::string server_id, std::string obis_code);
  void publish_val(const D0Node &obis_info) override;
  void dump_config() override;
};

}  // namespace d0
}  // namespace esphome
