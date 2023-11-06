#pragma once

#include <iostream>

#include "./RM_API.hpp"
#include "ruby.h"

class RubyService {

 public:

  void run()
  {
    ruby_init();

    initRM_API();


    rb_eval_string("api = API.new \n api.printVersion;");

    // rb_eval_string("test()");

    // rb_eval_string("print 'Ruby version: '");
    // rb_eval_string("puts RUBY_VERSION");

    // rb_eval_string("print 'Working path: '");
    // rb_eval_string("puts Dir.getwd");

    rb_eval_string("$LOAD_PATH << '/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/scripts'");
    // rb_eval_string("print 'Load path: '");
    // rb_eval_string("puts $LOAD_PATH");

    // rb_eval_string("print '\n\n'");

    rb_require("script_01.rb");

    VALUE last_err = rb_gv_get("$!");

    rb_p(last_err);

    ruby_finalize();
  }
};