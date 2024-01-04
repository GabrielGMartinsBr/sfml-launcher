#pragma once

#include <boost/filesystem/path.hpp>

namespace app {

typedef boost::filesystem::path FilePath;

typedef std::string String;
typedef const char* CStr;

template <typename T>
using Vector = std::vector<T>;

}  // namespace app