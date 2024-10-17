#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace app {

typedef unsigned int UInt;
typedef unsigned long ULong;

typedef std::string String;
typedef const char* CStr;

typedef std::wstring WString;

typedef std::ostringstream OutStrStream;
typedef std::stringstream StrStream;

template <typename T>
using Vector = std::vector<T>;

using StrVector = Vector<String>;

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using VectorPtr = UPtr<Vector<T>>;

using StrVectorPtr = VectorPtr<String>;

template <typename Key, typename Tp>
using UnMap = std::unordered_map<Key, Tp>;

typedef boost::filesystem::path FilePath;

}  // namespace app