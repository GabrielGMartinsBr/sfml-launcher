

#pragma once

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <sstream>

#include "base/AppDefs.h"
#include "base/RbUtils.hpp"
#include "ruby.h"

class RxdataLoader {
 public:

  static VALUE load(app::CStr filename)
  {
    app::FilePath filePath = resolvePath(filename);
    app::String binContent = readFile(filePath);
    return unserialize(binContent);
  }

 private:
  static app::String readFile(app::FilePath filePath)
  {
    std::ifstream inputFile(filePath.c_str());
    if (!inputFile.is_open()) {
      throw std::runtime_error("Read file method failed.");
    }

    constexpr std::streamsize chunkSize = 4096;
    char buffer[chunkSize];
    std::ostringstream oss;

    while (!inputFile.eof()) {
      inputFile.read(buffer, chunkSize);
      oss.write(buffer, inputFile.gcount());
    }

    inputFile.close();

    return oss.str();
  }

  static app::FilePath resolvePath(app::CStr filename)
  {
    std::string dir = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1";
    boost::filesystem::path combined_path = boost::filesystem::path(dir) / filename;
    return boost::filesystem::canonical(combined_path);
  }

  static VALUE unserialize(app::String& binContent)
  {
    VALUE binStr = RbUtils::createRbString(binContent);
    VALUE stringData = rb_str_new(binContent.c_str(), binContent.size());
    return RbUtils::marshalLoad(stringData);
  }
};