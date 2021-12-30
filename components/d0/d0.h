#pragma once

#include <string>
#include <vector>
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "d0_parser.h"

namespace esphome {
namespace d0 {

class D0Listener {
 public:
  std::string server_id;
  std::string obis_code;
  D0Listener(std::string server_id, std::string obis_code);
  virtual void publish_val(const D0Node &obis_info){};
};

class D0 : public Component, public uart::UARTDevice {
 public:
  void register_d0_listener(D0Listener *listener);
  void loop() override;
  void dump_config() override;
  std::vector<D0Listener *> d0_listeners_{};

 protected:
  void process_d0_file_(const bytes &d0_data);
  void log_obis_info_(const std::vector<D0Node> &obis_info_vec);
  void publish_obis_info_(const std::vector<D0Node> &obis_info_vec);
  char check_start_end_bytes_(char c);
  void publish_value_(const D0Node &obis_info);

  // Serial parser
  bool record_ = false;
  bytes d0_data_;
};
}  // namespace d0
}  // namespace esphome
