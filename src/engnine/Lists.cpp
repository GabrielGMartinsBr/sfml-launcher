#include "Lists.hpp"

namespace Eng {

static Lists* instance = nullptr;

void Lists::Init()
{
  assert(!instance);
  instance = new Lists();
}

Lists& Lists::Instance()
{
  assert(instance);
  return *instance;
}

void Lists::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

void Lists::markZOrderDirty()
{
  zDirty = true;
}

void Lists::sortZ()
{
  if (!zDirty) {
    return;
  }
  std::sort(renderList.begin(), renderList.end(), compareZ);
  zDirty = false;
}

bool Lists::compareZ(const OnRender* a, const OnRender* b)
{
  return a->getZIndex() < b->getZIndex();
}

}  // namespace Eng