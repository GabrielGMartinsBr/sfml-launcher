#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>
#include <sstream>
namespace app {

typedef unsigned int UInt;
typedef unsigned long ULong;

typedef std::string String;
typedef const char* CStr;

typedef std::ostringstream OutStrStream;
typedef std::stringstream StrStream;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using VectorPtr = UPtr<Vector<T>>;

using StrVector = VectorPtr<String>;

typedef boost::filesystem::path FilePath;

}  // namespace app