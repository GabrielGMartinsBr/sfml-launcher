
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

void AeonWindowManager::handleMouseMoved(const AeMouseMoveEvent& event)
{
  bool hasIntersection = false;
  for (AeonWindow* entry : entries) {
    hasIntersection = entry->intersects(event.x, event.y);
    entry->setIsHover(hasIntersection);
    if (hasIntersection) {
      entry->handleMouseMoved(event);
    }
    // entry->setRingVisibility(hasIntersection);
  }
}

void AeonWindowManager::handleMousePressed(const AeMouseButtonEvent& event)
{
  for (AeonWindow* entry : entries) {
    bool isHover = entry->intersects(event.x, event.y);
    entry->setIsFocused(isHover);
    if (isHover) {
      entry->handleMousePressed(event);
    }
  }
}

void AeonWindowManager::handleMouseReleased(const AeMouseButtonEvent& event)
{
  for (AeonWindow* entry : entries) {
    entry->handleMouseReleased(event);
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
  ts = getTimestamp();

  for (AeonWindow* entry : entries) {
    entry->handleAeonUpdate(ts);
  }
}

ULong AeonWindowManager::getTimestamp()
{
  return clock.getElapsedTime().asMilliseconds();
}

}  // namespace ae