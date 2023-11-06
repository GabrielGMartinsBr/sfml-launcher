#include <ruby.h>

class RubyService {

 public:

  void run()
  {
    ruby_init();

    rb_eval_string("puts 'Hello, Ruby from C++!'");

    ruby_finalize();
  }
};