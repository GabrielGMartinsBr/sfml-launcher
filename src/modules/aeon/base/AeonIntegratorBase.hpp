#pragma once

#include <ruby.h>

#include "AppDefs.h"

namespace ae {

using app::SPtr;

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
};

}  // namespace ae