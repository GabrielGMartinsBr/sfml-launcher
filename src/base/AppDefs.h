#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>

namespace app {

typedef unsigned int UInt;
typedef unsigned long ULong;

typedef std::string String;
typedef const char* CStr;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using UniqPtr = std::unique_ptr<T>;

typedef boost::filesystem::path FilePath;

}  // namespace app