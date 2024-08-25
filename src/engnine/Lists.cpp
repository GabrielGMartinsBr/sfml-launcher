#include "Lists.hpp"

namespace Eng {

static Lists *instance = nullptr;

void Lists::Init()
{
  assert(!instance);
  instance = new Lists();
}

Lists &Lists::Instance()
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

}  // namespace Eng