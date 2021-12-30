#include "constants.h"
#include "d0_parser.h"

namespace esphome {
namespace d0 {

D0File::D0File(bytes buffer) : buffer_(std::move(buffer)) {
  // extract messages
  this->pos_ = 0;
  while (this->pos_ + 14 < this->buffer_.size()) { // 14 = minimal message length "1-0:0.0.0*0(x)"
    D0Node message = D0Node();
    auto startCode = find(this->buffer_.begin() + this->pos_, this->buffer_.end(), '1');
    if ( (char)(startCode[1]) != '-')
    {
      this->pos_ += distance(this->buffer_.begin() + this->pos_, startCode) + 1;
      continue;
    }
    if (startCode != this->buffer_.end())
    {
      auto startValue = find(startCode, this->buffer_.end(), '(');
      std::string value = std::string(startCode, startValue);
      if (startValue != this->buffer_.end())
        message.code = std::string(startCode,  startValue);
      else
        break;
      auto endValue = find(startValue + 1, this->buffer_.end(), ')');
      if (endValue != this->buffer_.end())
        message.value = bytes(startValue + 1,  endValue);
      else
        break;
      this->pos_ += distance(this->buffer_.begin() + this->pos_, endValue) + 1;
      this->messages.emplace_back(message);
    }
    else
      break;
  }
}

}  // namespace d0
}  // namespace esphome
