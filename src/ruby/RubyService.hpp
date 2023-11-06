#include <iostream>

#include "ruby.h"

class RubyService {

 public:

  void run()
  {
    ruby_init();
    rb_eval_string("print 'Ruby version: '");
    rb_eval_string("puts RUBY_VERSION");

    rb_eval_string("print 'Working path: '");
    rb_eval_string("puts Dir.getwd");

    rb_eval_string("$LOAD_PATH << '/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/scripts'");
    rb_eval_string("print 'Load path: '");
    rb_eval_string("puts $LOAD_PATH");

    rb_eval_string("print '\n\n'");

    rb_require("script_01.rb");

    ruby_finalize();
  }
};