#include "./WrappedList.hpp"

namespace ae {
/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static WrappedList* instance = nullptr;

void WrappedList::Init()
{
  assert(!instance);
  instance = new WrappedList();
}

WrappedList& WrappedList::Instance()
{
  assert(instance);
  return *instance;
}

void WrappedList::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

}  // namespace ae