#pragma once

#include "AppDefs.h"
#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

using app::StrVector;

class Font : public EngineBase {
 public:
  // static StrVector* default_name;
  static int default_size;
  static bool default_bold;
  static bool default_italic;
  // static Color* default_color;

  static bool exist(const char* name)
  {
    return false;
  }

 public:

  Font(const StrVector& names, int size = 22) :
      name(names),
      size(size),
      bold(false),
      italic(false)
  {
    color = new Color(255, 255, 255, 255);
  }

  ~Font()
  {
  }

  // Font(Font& other) :
  //     name(Font::default_name),
  //     size(Font::default_size),
  //     bold(Font::default_bold),
  //     italic(Font::default_italic),
  //     color(Font::default_color),
  //     ptr(Qnil)
  // {
  // }

  Font() :
      name(StrVector({ "Arial" })),
      size(22),
      bold(false),
      italic(false)
  {
    color = new Color(255, 255, 255, 255);
  }

  const StrVector& getter_name()
  {
    return name;
  }

  void setter_name(StrVector& v)
  {
    name.swap(v);
  }

  int getter_size()
  {
    return size;
  }

  void setter_size(int v)
  {
    size = v;
  }

  bool getter_bold()
  {
    return false;
  }

  void setter_bold(bool v)
  {
    bold = v;
  }

  bool getter_italic()
  {
    return italic;
  }

  void setter_italic(bool v)
  {
    italic = v;
  }

  Color* getter_color()
  {
    return color;
  }

  void setter_color(Color* v)
  {
    color = v;
  }

 private:
  StrVector name;
  int size;
  bool bold;
  bool italic;
  Color* color;
};

}  // namespace Eng