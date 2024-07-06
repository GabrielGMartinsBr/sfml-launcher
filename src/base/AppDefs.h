#pragma once

#include <boost/filesystem/path.hpp>

namespace app {

typedef unsigned int UInt;
typedef unsigned long ULong;

typedef std::string String;
typedef const char* CStr;

template <typename T>
using Vector = std::vector<T>;

typedef boost::filesystem::path FilePath;

}  // namespace app