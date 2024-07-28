#pragma once

#include <cstdint>
#include <cstring>

struct MarshalUtils {
  static inline int readInt32(const char **data)
  {
    int32_t result;

    std::memcpy(&result, *data, 4);
    *data += 4;

    return result;
  }

  static inline int readDouble(const char **data)
  {
    double result;

    std::memcpy(&result, *data, 8);
    *data += 8;

    return result;
  }
};