#pragma once

#include "engnine/Input.hpp"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

class Input {

 public:

  static void integrate()
  {
    VALUE inputClass = rb_define_class("Input", rb_cObject);

    rb_define_module_function(inputClass, "update", RUBY_METHOD_FUNC(method_Update), 0);
    rb_define_module_function(inputClass, "trigger?", RUBY_METHOD_FUNC(method_trigger), 1);

    rb_define_const(inputClass, "DOWN", Convert::toRubyNumber(Eng::InputKey::DOWN));
    rb_define_const(inputClass, "LEFT", Convert::toRubyNumber(Eng::InputKey::LEFT));
    rb_define_const(inputClass, "RIGHT", Convert::toRubyNumber(Eng::InputKey::RIGHT));
    rb_define_const(inputClass, "UP", Convert::toRubyNumber(Eng::InputKey::UP));
    rb_define_const(inputClass, "A", Convert::toRubyNumber(Eng::InputKey::A));
    rb_define_const(inputClass, "B", Convert::toRubyNumber(Eng::InputKey::B));
    rb_define_const(inputClass, "C", Convert::toRubyNumber(Eng::InputKey::C));
    rb_define_const(inputClass, "X", Convert::toRubyNumber(Eng::InputKey::X));
    rb_define_const(inputClass, "Y", Convert::toRubyNumber(Eng::InputKey::Y));
    rb_define_const(inputClass, "Z", Convert::toRubyNumber(Eng::InputKey::Z));
    rb_define_const(inputClass, "L", Convert::toRubyNumber(Eng::InputKey::L));
    rb_define_const(inputClass, "R", Convert::toRubyNumber(Eng::InputKey::R));
    rb_define_const(inputClass, "SHIFT", Convert::toRubyNumber(Eng::InputKey::SHIFT));
    rb_define_const(inputClass, "CTRL", Convert::toRubyNumber(Eng::InputKey::CTRL));
    rb_define_const(inputClass, "ALT", Convert::toRubyNumber(Eng::InputKey::ALT));
    rb_define_const(inputClass, "F5", Convert::toRubyNumber(Eng::InputKey::F5));
    rb_define_const(inputClass, "F6", Convert::toRubyNumber(Eng::InputKey::F6));
    rb_define_const(inputClass, "F7", Convert::toRubyNumber(Eng::InputKey::F7));
    rb_define_const(inputClass, "F8", Convert::toRubyNumber(Eng::InputKey::F8));
    rb_define_const(inputClass, "F9", Convert::toRubyNumber(Eng::InputKey::F9));
  }

  static VALUE method_Update(VALUE self)
  {
    Eng::Input *obj = (Eng::Input *)DATA_PTR(self);
    obj->update();
    return Qnil;
  }

  static VALUE method_trigger(VALUE self, VALUE num)
  {
    return Qfalse;
  }
};

}  // namespace It