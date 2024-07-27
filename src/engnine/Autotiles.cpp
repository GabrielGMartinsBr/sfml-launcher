#pragma once

#include "engnine/Autotiles.h"

#include <string>

#include "AppDefs.h"
#include "integrator/It_Autotiles.hpp"
#include "integrator/It_Bitmap.hpp"

namespace Eng {

Autotiles::Autotiles() :
    EngineBase(Qnil)
{
}

Autotiles::~Autotiles()
{
}


bool Autotiles::setter(int index, Bitmap* value)
{
  if (index < 0 && index > 6) {
    return false;
  }

  // delete bitmaps[index];
  // bitmaps[index] = nullptr;

  bitmaps[index] = value;

  return true;
}

Bitmap* Autotiles::getter(int index)
{
  if (index < 0 && index > 6) {
    return nullptr;
  }
  return bitmaps[index];
}

}  // namespace Eng