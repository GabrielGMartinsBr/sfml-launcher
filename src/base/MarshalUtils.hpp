#include <cstring>
struct MarshalUtils {
  static inline int readInt32(const char **data)
  {
    int result;

    std::memcpy(&result, *data, 4);
    *data += 4;

    return result;
  }
};