#pragma once

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "base/AppDefs.h"

namespace Eng {

using app::CStr;
using app::FilePath;
using app::String;

static CStr RTP_PATH = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/GameData/RGSS-RTP";

struct FileUtils {
  static String parseRtpPath(CStr assetName)
  {
    FilePath path(RTP_PATH);
    path.append(assetName);
    path.replace_extension(".png");
    if (boost::filesystem::exists(path)) {
      return path.string();
    }
    path.replace_extension(".jpg");
    if (boost::filesystem::exists(path)) {
      return path.string();
    }
    return nullptr;
  }

  static String resolveRtpPath(CStr assetName)
  {
    FilePath path(RTP_PATH);
    path.append(assetName);
    return path.string();
  }

  static FilePath combine(CStr basePath, CStr path)
  {
    FilePath combined_path = FilePath(basePath) / path;
    return combined_path;
  }

  static FilePath combine(String basePath, CStr path)
  {
    FilePath combined_path = FilePath(basePath) / path;
    return boost::filesystem::canonical(combined_path);
  }

  static bool exist(CStr path)
  {
    return boost::filesystem::exists(path);
  }

  static bool isFile(CStr path)
  {
    return boost::filesystem::is_regular_file(path);
  }

  static bool isDirectory(CStr path)
  {
    return boost::filesystem::is_directory(path);
  }

  static int getFileSize(CStr path)
  {
    return boost::filesystem::file_size(path);
  }
};

}  // namespace Eng