#pragma once

#include "base/AppDefs.h"
#include "base/RbUtils.hpp"
#include "base/ZlibUtils.hpp"
#include "ruby.h"

class PlayerScript {
 public:

  app::UInt id;
  app::String name;
  app::String code;

  PlayerScript(VALUE id, VALUE name, VALUE code)
  {
    this->id = RbUtils::parseFix(id);
    this->name = RbUtils::parseString(name);
    this->code = parseCode(code);
  }

 private:

  app::UInt parseId(VALUE id)
  {
    return 0;
  }

  app::String parseCode(VALUE rbStr)
  {
    app::String compressed = RbUtils::parseString(rbStr);
    return ZlibUtils::decompress(compressed);
  }
};