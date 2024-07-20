#pragma once

#include <stdexcept>

#include "MarshalUtils.hpp"
#include "ruby.h"

namespace Eng {

class Tone {
 public:
  static Tone *deserialize(const char *data, int len)
  {
    if (len < 32) {
      throw std::runtime_error("Marshal error: Tone has a bad file format");
    }

    Tone *tone = new Tone();

    tone->red = MarshalUtils::readDouble(&data);
    tone->green = MarshalUtils::readDouble(&data);
    tone->blue = MarshalUtils::readDouble(&data);
    tone->gray = MarshalUtils::readDouble(&data);

    return tone;
  }

  VALUE ptr;

  int red = 0;
  int green = 0;
  int blue = 0;
  int gray = 0;

  Tone()
  {
    set(0, 0, 0);
  }

  Tone(int red, int green, int blue, int gray = 0)
  {
    set(red, green, blue, gray);
  }

  void set(int red, int green, int blue, int gray = 0)
  {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->gray = gray;
  }
};

}  // namespace Eng