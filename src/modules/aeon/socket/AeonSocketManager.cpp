
#include "aeon/socket/AeonSocketManager.hpp"

#include <cassert>
#include "Log.hpp"

namespace ae {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static AeonSocketManager* instance = nullptr;

void AeonSocketManager::Init()
{
  assert(!instance);
  instance = new AeonSocketManager();
}

AeonSocketManager& AeonSocketManager::Instance()
{
  assert(instance);
  return *instance;
}

void AeonSocketManager::Destroy()
{
  assert(instance);
  Log::out() << "Destroying all sockets...";
  instance->destroyAll();
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

AeonSocketManager::AeonSocketManager()
{
}

}  // namespace ae