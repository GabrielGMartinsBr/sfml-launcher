#pragma once

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <ios>
#include <sstream>

#include "base/AppDefs.h"
#include "base/Log.hpp"

class ScriptsLoader {
 public:
  static ScriptsLoader& getInstance()
  {
    static ScriptsLoader instance;
    return instance;
  }

  void loadScripts(app::CStr rxFilePath)
  {
    app::String binContent = readFile(rxFilePath);
    Log::out() << binContent.size();
  }

 private:
  ScriptsLoader(const ScriptsLoader&);
  ScriptsLoader& operator=(const ScriptsLoader&);

  ScriptsLoader() { }

  app::FilePath resolvePath(app::CStr pathStr)
  {
    return boost::filesystem::canonical(pathStr);
  }

  app::String readFile(app::CStr path)
  {
    app::FilePath filePath = resolvePath(path);
    std::ifstream inputFile(filePath.c_str());
    if (!inputFile.is_open()) {
      throw std::runtime_error("Read file method failed.");
    }

    constexpr std::streamsize chunkSize = 4096;
    char buffer[chunkSize];
    std::ostringstream oss;

    while (inputFile.read(buffer, chunkSize)) {
      oss.write(buffer, inputFile.gcount());
    }

    inputFile.close();

    return oss.str();
  }
};