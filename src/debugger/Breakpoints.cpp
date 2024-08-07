#include "Breakpoints.h"

namespace dbg {

Breakpoints& Breakpoints::getInstance()
{
  static Breakpoints instance;
  return instance;
}

Breakpoints::Breakpoints()
{
  size = 0;
  // Zero means that entry has no line set
  for (int i = 0; i < MAX_SIZE; i++) {
    values[i] = 0;
  }
}

void Breakpoints::add(UInt line)
{
  if (findIndex(line) == -1) {
    values[size] = line;
    size++;
  }
}

void Breakpoints::remove(UInt line)
{
  long id = findIndex(line);
  if (id == -1) {
    return;
  }
  int lastIndex = size - 1;
  values[id] = values[lastIndex];
  values[lastIndex] = 0;
  size--;
}

bool Breakpoints::contains(UInt line)
{
  return findIndex(line) > -1;
}

int Breakpoints::getSize()
{
  return size;
}

bool Breakpoints::isEmpty()
{
  return size == 0;
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Private ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

long Breakpoints::findIndex(int line)
{
  for (int i = 0; i < MAX_SIZE; i++) {
    if (values[i] == 0) {
      // Stop search, it has reached the end of valid entries
      break;
    }
    if (values[i] == line) {
      // Return found index
      return i;
    }
  }
  // Return not found
  return -1;
}

}  // namespace dbg