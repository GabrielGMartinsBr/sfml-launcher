#pragma once

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <string>

typedef boost::filesystem::path FPath;

namespace Eng {

struct FileUtils {
  static std::string parseAssetPath(const char* assetName)
  {
    FPath p("../assets");
    p.append(assetName);
    p.replace_extension(".png");
    FPath absolutePath = boost::filesystem::canonical(p);
    return absolutePath.string();
  }

  static std::string parseRtpPath(const char* assetName)
  {
    FPath p("../GameData/RGSS-RTP");
    p.append(assetName);
    p.replace_extension(".png");
    FPath absolutePath = boost::filesystem::canonical(p);
    return absolutePath.string();
  }
};

}  // namespace Eng