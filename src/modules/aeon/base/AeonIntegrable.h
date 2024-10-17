#pragma once

#include <ruby.h>

#include <SFML/System/String.hpp>

#include "AppDefs.h"

namespace ae {

using app::CStr;
using app::String;

struct AeonIntegrable {
  VALUE rbId;

  AeonIntegrable(VALUE rbId = Qnil);

  void handleInitialize(VALUE rbId);

  inline bool isWrapped() const
  {
    return rbId != Qnil;
  }

  VALUE setInstanceVar(CStr name, VALUE value);
  VALUE setInstanceVar(CStr name, bool value);
  VALUE setInstanceVar(CStr name, int value);
  VALUE setInstanceVar(CStr name, double value);
  VALUE setInstanceVar(CStr name, CStr value);
  VALUE setInstanceVar(CStr name, const String& value);
  VALUE setInstanceVar(CStr name, AeonIntegrable* value);

  VALUE toRubyValue(bool value);
  VALUE toRubyValue(int value);
  VALUE toRubyValue(long value);
  VALUE toRubyValue(double value);
  VALUE toRubyValue(CStr value);
  VALUE toRubyValue(const String& value);
  VALUE toRubyValue(AeonIntegrable* value);
};

}  // namespace ae