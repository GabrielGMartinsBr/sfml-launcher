#pragma once

#include <ruby.h>

#include "AppDefs.h"
#include "engnine/EngineBase.hpp"
#include "integrator/Convert.hpp"

namespace Eng {

using app::CStr;

template <typename T>
struct PropNumber {
  typedef VALUE (*ToRbValue)(T);
  typedef T (*ToCValue)(VALUE);

  PropNumber(CStr name, ToCValue toC, ToRbValue toRb, EngineBase &inst, T val) :
      inst(inst),
      name(name),
      toRbValue(toRb),
      toCValue(toC)
  {
    value = val;
    rbValue = Convert::toRubyNumber(val);
    setInstanceVar();
  }

  bool operator==(const PropNumber &other) const
  {
    return value == other.value;
  }

  bool operator==(T otherValue) const
  {
    return value == otherValue;
  }

  PropNumber &operator=(const PropNumber &other)
  {
    value = other.value;
    rbValue = other.rbValue;
    setInstanceVar();
    return *this;
  }

  T get() const
  {
    return value;
  }

  VALUE getVALUE() const
  {
    return rbValue;
  }

  void set(T val)
  {
    if (value == val) return;
    value = val;
    rbValue = Convert::toRubyDouble(val);
    setInstanceVar();
    inst.markAsDirty();
  }

  VALUE setVALUE(VALUE val)
  {
    if (rbValue == val) return rbValue;
    rbValue = val;
    value = Convert::toCDouble2(val);
    setInstanceVar();
    inst.markAsDirty();
    return rbValue;
  }

  void setInstanceVar()
  {
    if (!inst.hasRbObj()) {
      return;
    }
    rb_iv_set(inst.rbObj, name, rbValue);
  }

 private:
  EngineBase &inst;
  CStr name;
  T value;
  VALUE rbValue;

  ToRbValue toRbValue;
  ToCValue toCValue;
};

};  // namespace Eng