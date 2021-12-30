#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include "constants.h"

namespace esphome {
namespace d0 {

using bytes = std::vector<uint8_t>;

class D0Node {
 public:
  std::string code;
  bytes value;
  //bytes unit;
};

class D0File {
 public:
  D0File(bytes buffer);
  bool setup_node(D0Node *node);
  std::vector<D0Node> messages;

 protected:
  const bytes buffer_;
  size_t pos_;
};
}  // namespace d0
}  // namespace esphome
