#pragma once

#include <stdexcept>

#include "MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Tone : public EngineBase {
 public:
  static Tone *deserialize(const char *data, float len)
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

  float red = 0;
  float green = 0;
  float blue = 0;
  float gray = 0;

  Tone(float red, float green, float blue, float gray = 0)
  {
    set(red, green, blue, gray);
  }

  Tone()
  {
    Tone(0, 0, 0, 0);
  }

  void set(float red, float green, float blue, float gray = 0)
  {
    clamp(red);
    clamp(green);
    clamp(blue);
    clamp(gray);
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->gray = gray;
  }

 private:
  void clamp(float &v)
  {
    if (v < 0) {
      v = 0;
    } else if (v > 255) {
      v = 255;
    }
  }
};

}  // namespace Eng