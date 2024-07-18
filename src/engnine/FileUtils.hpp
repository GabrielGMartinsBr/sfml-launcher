#pragma once

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <string>

#include "base/AppDefs.h"

typedef boost::filesystem::path FPath;

namespace Eng {

struct FileUtils {
  static app::String parseAssetPath(app::CStr assetName)
  {
    FPath p("/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/assets");
    p.append(assetName);
    p.replace_extension(".png");
    FPath absolutePath = boost::filesystem::canonical(p);
    return absolutePath.string();
  }

  static app::String parseRtpPath(app::CStr assetName)
  {
    FPath p("/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/GameData/RGSS-RTP");
    p.append(assetName);
    p.replace_extension(".png");
    FPath absolutePath = boost::filesystem::canonical(p);
    return absolutePath.string();
  }

  static app::FilePath combine(app::CStr basePath, app::CStr path)
  {
    boost::filesystem::path combined_path = boost::filesystem::path(basePath) / path;
    return combined_path;
  }

  static app::FilePath combine(app::String basePath, app::CStr path)
  {
    boost::filesystem::path combined_path = boost::filesystem::path(basePath) / path;
    return boost::filesystem::canonical(combined_path);
  }

  static bool exist(app::CStr path)
  {
    return boost::filesystem::exists(path);
  }

  static bool isFile(app::CStr path)
  {
    return boost::filesystem::is_regular_file(path);
  }

  static bool isDirectory(app::CStr path)
  {
    return boost::filesystem::is_directory(path);
  }

  static int getFileSize(app::CStr path)
  {
    return boost::filesystem::file_size(path);
  }
};

}  // namespace Eng