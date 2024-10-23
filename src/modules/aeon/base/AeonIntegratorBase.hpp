#pragma once

#include <ruby.h>

#include <format>
#include <iostream>

#include "AppDefs.h"
#include "RbUtils.hpp"
#include "aeon/base/WrappedList.hpp"

namespace ae {

using app::CStr;
using app::SPtr;
using app::String;

template <typename RubyClass>
struct AeonIntegratorBase {
  static RubyClass *getObjectValue(VALUE rbId)
  {
    return static_cast<RubyClass *>(DATA_PTR(rbId));
  }

  static RubyClass &getWrappedObject(VALUE rbId)
  {
    RubyClass *ptr = static_cast<RubyClass *>(DATA_PTR(rbId));
    assert(ptr);
    return *ptr;
  }

  static RubyClass &getWrappedSharedPtr(VALUE rbId)
  {
    SPtr<RubyClass> *ptr = static_cast<SPtr<RubyClass> *>(DATA_PTR(rbId));
    assert(ptr);
    return **ptr;
  }

  static VALUE raiseException(String msg)
  {
    RbUtils::raiseRuntimeException(msg);
    return Qnil;
  }

  static VALUE raiseArgException(CStr method, CStr args, int received)
  {
    std::ostringstream oss;
    oss << method << " takes " << args << " arguments, but " << received << " were received.";
    RbUtils::raiseRuntimeException(oss.str());
    return Qnil;
  }

  static WrappedList &wrapList()
  {
    return WrappedList::Instance();
  }
};

}  // namespace ae