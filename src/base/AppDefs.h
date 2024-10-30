#pragma once

#include <boost/filesystem/path.hpp>
#include <condition_variable>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
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

using Thread = std::thread;

template <typename T>
using Atomic = std::atomic<T>;

template <typename T>
using Queue = std::queue<T>;

using Mutex = std::mutex;

using ConditionVariable = std::condition_variable;

template <typename Key, typename Tp>
using UnMap = std::unordered_map<Key, Tp>;

typedef boost::filesystem::path FilePath;

}  // namespace app