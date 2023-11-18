#pragma once

#include <memory>
#include <vector>

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T, typename... Args>
std::shared_ptr<T> MakeSharedPtr(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Vector = std::vector<T>;
