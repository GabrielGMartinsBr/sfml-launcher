
#include "aeon/window/AeonWindowManager.h"

namespace ae {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/

static AeonWindowManager* instance = nullptr;

void AeonWindowManager::Init()
{
  assert(!instance);
  instance = new AeonWindowManager();
}

AeonWindowManager& AeonWindowManager::Instance()
{
  assert(instance);
  return *instance;
}

void AeonWindowManager::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

void AeonWindowManager::handleMouseMoved(const Event::MouseMoveEvent& event)
{
  bool hasIntersection = false;
  for (AeonWindow* entry : entries) {
    hasIntersection = entry->intersects(event.x, event.y);
    entry->setRingVisibility(hasIntersection);
  }
}

void AeonWindowManager::addEntry(AeonWindow* entry)
{
  entries.push_back(entry);
}

void AeonWindowManager::removeEntry(AeonWindow* entry)
{
  auto it = std::find(entries.begin(), entries.end(), entry);
  if (it != entries.end()) {
    entries.erase(it);
  }
}

void AeonWindowManager::updateEntries()
{
  ts = clock.getElapsedTime().asSeconds();

  for (AeonWindow* entry : entries) {
    entry->handleAeonUpdate(ts);
  }
}

}  // namespace ae