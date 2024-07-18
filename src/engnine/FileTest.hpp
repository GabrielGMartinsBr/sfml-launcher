#pragma once

#include "engnine/FileUtils.hpp"

namespace Eng {

class FileTest {
 public:
  static bool exist(app::CStr _path)
  {
    app::CStr dir = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1";
    app::FilePath path = FileUtils::combine(dir, _path);
    return FileUtils::exist(path.c_str());
  }

  static bool directory(app::CStr _path)
  {
    app::CStr dir = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1";
    app::FilePath path = FileUtils::combine(dir, _path);
    return FileUtils::isDirectory(path.c_str());
  }

  static bool file(app::CStr _path)
  {
    app::CStr dir = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1";
    app::FilePath path = FileUtils::combine(dir, _path);
    return FileUtils::isFile(path.c_str());
  }

  static int size(app::CStr _path)
  {
    app::CStr dir = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1";
    app::FilePath path = FileUtils::combine(dir, _path);
    auto result = FileUtils::getFileSize(path.c_str());
    return result;
  }
};

}  // namespace Eng