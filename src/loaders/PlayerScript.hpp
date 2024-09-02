#pragma once

#include "base/AppDefs.h"
#include "base/BacktraceUtils.hpp"
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
    this->id = rb_num2uint(id);
    this->name = RbUtils::parseString(name);
    this->code = parseCode(code);
    this->linesNumber = BacktraceUtils::countLines(this->code);
  }

  app::ULong getLinesNumber() const
  {
    return linesNumber;
  }

  app::ULong getStartLine() const
  {
    return startLine;
  }

  app::ULong getEndLine() const
  {
    return endLine;
  }

  void setStartLine(app::ULong number)
  {
    this->startLine = number;
    this->endLine = number + this->linesNumber;
  }

 private:
  app::ULong startLine;
  app::ULong endLine;
  app::ULong linesNumber;

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