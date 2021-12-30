#include "d0.h"
#include "esphome/core/log.h"
#include "d0_parser.h"

namespace esphome {
namespace d0 {

static const char *const TAG = "d0";

const char START_BYTE_DETECTED = 1;
const char END_BYTE_DETECTED = 2;

D0Listener::D0Listener(std::string server_id, std::string obis_code)
    : server_id(std::move(server_id)), obis_code(std::move(obis_code)) {}

char D0::check_start_end_bytes_(char c) {
  if (c == START_BYTE)
    return START_BYTE_DETECTED;
  if (c == END_BYTE)
    return END_BYTE_DETECTED;
  return 0;
}

void D0::loop() {
  while (available()) {
    const char c = read();

    if (this->record_)
      this->d0_data_.emplace_back(c);

    switch (this->check_start_end_bytes_(c)) {
      case START_BYTE_DETECTED: {
        this->record_ = true;
        this->d0_data_.assign(&START_BYTE, &START_BYTE);
        break;
      };
      case END_BYTE_DETECTED: {
        if (this->record_) {
          this->record_ = false;
          this->process_d0_file_(this->d0_data_);
        }
        break;
      };
    };
  }
}

void D0::process_d0_file_(const bytes &d0_data) {
  D0File d0_file = D0File(d0_data);
  this->publish_obis_info_(d0_file.messages);
  this->log_obis_info_(d0_file.messages);
//  std::string tmp = std::string(d0_data.begin(), d0_data.end());
//  ESP_LOGD(TAG, "%s", tmp.c_str());
}

void D0::log_obis_info_(const std::vector<D0Node> &obis_info_vec) {
  int i = 0;
  ESP_LOGD(TAG, "OBIS info:");
  for (auto const &obis_info : obis_info_vec) {
    std::string info;
    info += obis_info.code + " = ";
    info += std::string(obis_info.value.begin(), obis_info.value.end());
    ESP_LOGD(TAG, "%s", info.c_str());
  }
}

void D0::publish_obis_info_(const std::vector<D0Node> &obis_info_vec) {
  for (auto const &obis_info : obis_info_vec) {
    this->publish_value_(obis_info);
  }
}

void D0::publish_value_(const D0Node &obis_info) {
  for (auto const &d0_listener : d0_listeners_) {
    if (obis_info.code != d0_listener->obis_code)
      continue;
    d0_listener->publish_val(obis_info);
  }
}

void D0::dump_config() { ESP_LOGCONFIG(TAG, "D0:"); }

void D0::register_d0_listener(D0Listener *listener) { d0_listeners_.emplace_back(listener); }

}  // namespace d0
}  // namespace esphome
