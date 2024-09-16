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

/*
    ⇩⇩⇩ Instance Private ⇩⇩⇩
*/

PackageReader::PackageReader(String projectPath) :
    projectPath(projectPath),
    projectTile(DEFAULT_WINDOW_TILE)
{
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
    }
  }

  file.close();
}

}  // namespace pkg