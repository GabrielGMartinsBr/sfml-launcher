#pragma once

#include "AppDefs.h"

namespace pkg {

using app::String;

class PackageReader {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init(String projectPath);
  static PackageReader& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  const String& getProjectTile();
  int getWidth();
  int getHeight();

 private:
  String projectPath;
  String projectTile;
  int width;
  int height;

  PackageReader(String projectPath);

  void read();
};

}  // namespace pkg
