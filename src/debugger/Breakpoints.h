#pragma once

#include "AppDefs.h"

namespace dbg {

using app::UInt;

struct Breakpoints {
  static Breakpoints& getInstance();

  void add(UInt line);

  void remove(UInt line);

  bool contains(UInt line);

  int getSize();

  bool isEmpty();

 private:
  static constexpr int MAX_SIZE = 512;
  UInt values[MAX_SIZE];
  UInt size;

  Breakpoints();
  Breakpoints(const Breakpoints&);
  Breakpoints& operator=(const Breakpoints&);

  long findIndex(int line);
};

}  // namespace dbg