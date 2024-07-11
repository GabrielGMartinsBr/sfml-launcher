#pragma once

#include <string>
#include <vector>

#include "base/Log.hpp"
#include "engnine/Color.hpp"
#include "ruby.h"

namespace Eng {

class Font {
 public:
  static std::vector<std::string>* default_name;
  static int default_size;
  static bool default_bold;
  static bool default_italic;
  static Color* default_color;

  static bool exist(const char* name)
  {
    return false;
  }

 public:
  VALUE ptr;

  Font(std::vector<std::string>* name, int size = 22) :
      name(name),
      size(size),
      bold(false),
      italic(false),
      color(nullptr),
      ptr(Qnil)
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
      name(new std::vector<std::string>({ "Arial" })),
      size(22),
      bold(false),
      italic(false),
      color(nullptr),
      ptr(Qnil)
  {
    Log::out() << "Default constructor";
  }

  std::vector<std::string>* getter_name()
  {
    return name;
  }

  void setter_name(std::vector<std::string>& v)
  {
    name->swap(v);
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
  std::vector<std::string>* name;
  int size;
  bool bold;
  bool italic;
  Color* color;
};

}  // namespace Eng