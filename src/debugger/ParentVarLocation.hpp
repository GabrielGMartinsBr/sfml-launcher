#pragma once

namespace dbg {

enum struct ParentVarLocation {
  LOCAL = 1,
  GLOBAL = 2,
  CLASS = 3,
  OBJECT = 4,
  ARRAY = 5,
};

}