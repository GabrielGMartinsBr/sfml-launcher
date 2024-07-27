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
  for (int i = 0; i < 6; ++i) {
    bitmaps[i] = nullptr;
  }
  bindRubyProps();
}

Autotiles::~Autotiles()
{
}

void Autotiles::bindRubyProps()
{
  if (rbObj == Qnil) {
    rbObj = It::Autotiles::createRubyObject(this);
  }
  app::String name;
  for (int i = 0; i < 6; ++i) {
    if (bitmaps[i] == nullptr) {
      continue;
    }
    if (bitmaps[i]->rbObj == Qnil) {
      bitmaps[i]->rbObj = It::Bitmap::createRubyObject(bitmaps[i]);
    }
    name = "bitmap_" + std::to_string(i);
    rb_iv_set(rbObj, name.c_str(), bitmaps[i]->rbObj);
  }
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