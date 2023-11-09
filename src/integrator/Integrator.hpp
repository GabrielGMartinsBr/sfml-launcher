#pragma once

#include <boost/filesystem.hpp>
#include <iostream>

#include "ruby.h"

typedef VALUE (*Cb)(VALUE);
typedef boost::filesystem::path FPath;

class Integrator {
 public:
  void operator()()
  {
    init();
  }

  void init()
  {
    ruby_init();

    loadScriptsPath();

    // require("script_02.rb");

    ruby_finalize();

    std::cout << "ruby finalized\n";
  }

  void cleanup()
  {
    ruby_finalize();
  }

 private:

  void loadScriptsPath()
  {
    FPath relativePath("../scripts");
    FPath absolutePath = boost::filesystem::canonical(relativePath);
    const char* scriptDirPath = absolutePath.c_str();
    std::cout << scriptDirPath;

    VALUE loadPathArray = rb_gv_get("$LOAD_PATH");
    rb_funcall(loadPathArray, rb_intern("<<"), 1, rb_str_new2(scriptDirPath));
  }

  void require(const char* c)
  {
    int error;
    rb_protect((Cb)RUBY_METHOD_FUNC(rb_require), (VALUE)c, &error);
    if (error) {
      VALUE error_message = rb_gv_get("$!");
      rb_p(error_message);
    }
  }
};