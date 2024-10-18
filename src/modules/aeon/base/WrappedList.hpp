#pragma once

#include <ruby.h>

#include "AppDefs.h"
#include "aeon/base/AeonIntegrable.h"

namespace ae {

using app::UnMap;
using app::UPtr;
using app::Vector;

class WrappedList {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static WrappedList& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  template <typename T, typename... Args>
  T* createObject(VALUE id, Args&&... args)
  {
    UPtr<T> obj = std::make_unique<T>(std::forward<Args>(args)...);
    T* rawPtr = obj.get();
    objects[id] = std::move(obj);
    DATA_PTR(id) = rawPtr;
    return rawPtr;
  }

  template <typename T>
  void addObject(VALUE id, UPtr<T> obj)
  {
    objects[id] = std::move(obj);
  }

  void removeObject(VALUE id)
  {
    objects.erase(id);
  }

  WrappedList(const WrappedList&) = delete;
  WrappedList& operator=(const WrappedList&) = delete;

 private:
  WrappedList() = default;
  ~WrappedList() = default;

  UnMap<VALUE, UPtr<AeonIntegrable>> objects;
};

}  // namespace ae