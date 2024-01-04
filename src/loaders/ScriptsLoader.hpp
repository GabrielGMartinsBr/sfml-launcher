#pragma once

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <ios>
#include <sstream>

#include "base/AppDefs.h"
#include "base/RbUtils.hpp"
#include "loaders/PlayerScript.hpp"
#include "ruby.h"

class ScriptsLoader {
 public:
  static ScriptsLoader& getInstance()
  {
    static ScriptsLoader instance;
    return instance;
  }

  void loadScripts(app::Vector<PlayerScript>& scripts, app::CStr rxFilePath)
  {
    app::String binContent = readFile(rxFilePath);
    parseScripts(scripts, binContent);
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

    while (!inputFile.eof()) {
      inputFile.read(buffer, chunkSize);
      oss.write(buffer, inputFile.gcount());
    }

    inputFile.close();

    return oss.str();
  }

  void parseScripts(app::Vector<PlayerScript>& scripts, app::String& binContent)
  {
    VALUE rbArr = unserialize(binContent);
    app::UInt arrSize = RbUtils::getArraySize(rbArr);

    VALUE rbEntry, id, name, code;

    for (int i = 0; i < arrSize; i++) {
      rbEntry = rb_ary_entry(rbArr, i);
      id = rb_ary_entry(rbEntry, 0);
      name = rb_ary_entry(rbEntry, 1);
      code = rb_ary_entry(rbEntry, 2);
      scripts.push_back(
        PlayerScript(id, name, code)
      );
    }
  }

  VALUE unserialize(app::String& binContent)
  {
    VALUE rbBinStr = RbUtils::createRbString(binContent);
    return RbUtils::marshalLoad(rbBinStr);
  }
};