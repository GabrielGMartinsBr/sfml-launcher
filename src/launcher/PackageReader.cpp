#include "launcher/PackageReader.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

#include "consts/PackageConsts.h"

namespace pkg {

using FilePath = std::filesystem::path;

/*
    ⇩⇩⇩ Static ⇩⇩⇩
*/
static PackageReader* instance = nullptr;

void PackageReader::Init(String projectPath)
{
  assert(!instance);
  instance = new PackageReader(projectPath);
}

PackageReader& PackageReader::Instance()
{
  assert(instance);
  return *instance;
}

void PackageReader::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
    ⇩⇩⇩ Instance Public ⇩⇩⇩
*/

const String& PackageReader::getProjectTile()
{
  return projectTile;
}

int PackageReader::getWidth()
{
  return width;
}

int PackageReader::getHeight()
{
  return height;
}

/*
    ⇩⇩⇩ Instance Private ⇩⇩⇩
*/

PackageReader::PackageReader(String projectPath) :
    projectPath(projectPath),
    projectTile(DEFAULT_WINDOW_TILE)
{
  width = 0;
  height = 0;
  read();
}

bool startsWith(const std::string& line, const std::string& prefix)
{
  return line.compare(0, prefix.length(), prefix) == 0;
}

void PackageReader::read()
{
  FilePath folderPath(projectPath);
  FilePath iniPath = folderPath / "Game.ini";

  std::ifstream file(iniPath);
  assert(file);

  String line;
  while (std::getline(file, line)) {
    if (startsWith(line, "Title=")) {
      projectTile = line.substr(6);
    } else if (startsWith(line, "Width=")) {
      int value = std::stoi(line.substr(6));
      if (value > 128) {
        width = value;
      }
    } else if (startsWith(line, "Height=")) {
      int value = std::stoi(line.substr(7));
      if (value > 96) {
        height = value;
      }
    }
  }

  file.close();
}

}  // namespace pkg