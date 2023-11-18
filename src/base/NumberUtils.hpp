#pragma once

namespace Num {

template <typename T>
inline T clamp(T& v, const T min, const T max)
{
  return (v < min) ? min : ((v > max) ? max : v);
}

}  // namespace Num