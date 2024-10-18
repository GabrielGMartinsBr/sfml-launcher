#pragma once

#include <ruby.h>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ColorParser.hpp"
#include "aeon/window/AeonElement.h"
#include "aeon/window/AeonStyleSheet.h"
#include "engnine/Color.hpp"
#include "integrator/Convert.hpp"

namespace ae {

using app::String;
using KeyPair = std::pair<String, String>;

constexpr unsigned int hashString(const char* str, int h = 0)
{
  return !str[h] ? 5381 : (hashString(str, h + 1) * 33) ^ str[h];
}

struct AeElementStyleParser {
  static void setStyleProp(AeonElement& element, VALUE propKey, VALUE value)
  {
    String rawKey = Convert::toCString(propKey);
    KeyPair key = splitKey(rawKey);
    AeonElementState state = parseElementState(key.first);

    if (state == AeonElementState::UNKNOW) {
      Log::err() << "Failed to get element style with state: " << key.first;
      return;
    }

    AeonStyleSheet* style = nullptr;
    if (state == AeonElementState::DEFAULT) {
      style = &element.getMutableStyle();
    } else {
      style = &element.getMutableStyle(state);
    }

    switch (hashString(key.second.c_str())) {
      case hashString("radius"): {
        style->radius = parseFloat(value);
        break;
      }
      case hashString("borderSize"): {
        style->borderSize = parseFloat(value);
        break;
      }
      case hashString("fontSize"): {
        style->fontSize = parseFloat(value);
        break;
      }
      case hashString("bgColor"): {
        style->bgColor = parseColor(value);
        break;
      }
      case hashString("borderColor"): {
        style->borderColor = parseColor(value);
        break;
      }
      case hashString("textColor"): {
        style->textColor = parseColor(value);
        break;
      }

      default: {
        break;
      }
    }
  }

  static KeyPair splitKey(const String& str)
  {
    size_t pos = str.find(':');
    if (pos == String::npos) {
      return { "", str };
    }

    String first_part = str.substr(0, pos);
    if (pos + 2 >= str.size()) {
      return { first_part, "" };
    }

    String second_part = str.substr(pos + 1);

    return { first_part, second_part };
  }

  static AeonElementState parseElementState(const String& stateName)
  {
    if (stateName.empty()) {
      return AeonElementState::DEFAULT;
    }

    static const std::unordered_map<String, AeonElementState> stateMap = {
      { "hover", AeonElementState::HOVER },
      { "focus", AeonElementState::FOCUS },
      { "focusVisible", AeonElementState::FOCUS_VISIBLE },
      { "active", AeonElementState::CLICKED },
      { "disabled", AeonElementState::DISABLED },
      { "invalid", AeonElementState::INVALID },
    };

    auto it = stateMap.find(stateName);
    if (it != stateMap.end()) {
      return it->second;
    }

    return AeonElementState::UNKNOW;
  }

  static float parseFloat(VALUE rawValue)
  {
    return Convert::toCDouble2(rawValue);
  }

  static Eng::Color parseColor(VALUE rawValue)
  {
    return ColorParser::hexToNrgssColor(Convert::toCStr(rawValue));
  }
};

}  // namespace ae