#include "Lists.hpp"

#include "engnine/Viewport.hpp"

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

Lists::Lists() { }

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
  for (Viewport* vp : viewports) {
    std::sort(vp->children.begin(), vp->children.end(), compareZ);
  }
  zDirty = false;
}

bool Lists::compareZ(const IOnRender* a, const IOnRender* b)
{
  return a->getZIndex() < b->getZIndex();
}

}  // namespace Eng