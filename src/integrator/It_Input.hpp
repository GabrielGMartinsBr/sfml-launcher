#pragma once

#include "engnine/Input.hpp"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

class Input {

 public:

  static void integrate()
  {
    VALUE inputModule = rb_define_module("Input");

    rb_define_module_function(inputModule, "update", RUBY_METHOD_FUNC(method_Update), 0);
    rb_define_module_function(inputModule, "press?", RUBY_METHOD_FUNC(method_press), 1);
    rb_define_module_function(inputModule, "trigger?", RUBY_METHOD_FUNC(method_trigger), 1);
    rb_define_module_function(inputModule, "repeat?", RUBY_METHOD_FUNC(method_repeat), 1);
    rb_define_module_function(inputModule, "dir4", RUBY_METHOD_FUNC(method_dir4), 0);
    rb_define_module_function(inputModule, "dir8", RUBY_METHOD_FUNC(method_dir8), 0);

    rb_define_const(inputModule, "DOWN", Convert::toRubyNumber(Eng::InputKey::DOWN));
    rb_define_const(inputModule, "LEFT", Convert::toRubyNumber(Eng::InputKey::LEFT));
    rb_define_const(inputModule, "RIGHT", Convert::toRubyNumber(Eng::InputKey::RIGHT));
    rb_define_const(inputModule, "UP", Convert::toRubyNumber(Eng::InputKey::UP));
    rb_define_const(inputModule, "A", Convert::toRubyNumber(Eng::InputKey::A));
    rb_define_const(inputModule, "B", Convert::toRubyNumber(Eng::InputKey::B));
    rb_define_const(inputModule, "C", Convert::toRubyNumber(Eng::InputKey::C));
    rb_define_const(inputModule, "X", Convert::toRubyNumber(Eng::InputKey::X));
    rb_define_const(inputModule, "Y", Convert::toRubyNumber(Eng::InputKey::Y));
    rb_define_const(inputModule, "Z", Convert::toRubyNumber(Eng::InputKey::Z));
    rb_define_const(inputModule, "L", Convert::toRubyNumber(Eng::InputKey::L));
    rb_define_const(inputModule, "R", Convert::toRubyNumber(Eng::InputKey::R));
    rb_define_const(inputModule, "SHIFT", Convert::toRubyNumber(Eng::InputKey::SHIFT));
    rb_define_const(inputModule, "CTRL", Convert::toRubyNumber(Eng::InputKey::CTRL));
    rb_define_const(inputModule, "ALT", Convert::toRubyNumber(Eng::InputKey::ALT));
    rb_define_const(inputModule, "F5", Convert::toRubyNumber(Eng::InputKey::F5));
    rb_define_const(inputModule, "F6", Convert::toRubyNumber(Eng::InputKey::F6));
    rb_define_const(inputModule, "F7", Convert::toRubyNumber(Eng::InputKey::F7));
    rb_define_const(inputModule, "F8", Convert::toRubyNumber(Eng::InputKey::F8));
    rb_define_const(inputModule, "F9", Convert::toRubyNumber(Eng::InputKey::F9));
  }

  static VALUE method_Update(VALUE self)
  {
    Eng::Input::getInstance().update();
    return Qnil;
  }

  static VALUE method_press(VALUE self, VALUE _num)
  {
    Eng::InputKey key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::getInstance().isPressed(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_trigger(VALUE self, VALUE _num)
  {
    Eng::InputKey key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::getInstance().isTriggered(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_repeat(VALUE self, VALUE _num)
  {
    // Log::out() << "repeat";
    Eng::InputKey key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::getInstance().isRepeated(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_dir4(VALUE self, VALUE _num)
  {
    return Convert::toRubyNumber(
      Eng::Input::getInstance().getDir4()
    );
  }

  static VALUE method_dir8(VALUE self, VALUE _num)
  {
    return Convert::toRubyNumber(0);
  }

  static Eng::InputKey convertKeyCode(VALUE _num)
  {
    int num = Convert::toCInt(_num);
    if (Eng::Input::getInstance().isValidKey(num)) {
      return (Eng::InputKey)num;
    }
    return Eng::InputKey::UNKNOW;
  }
};

}  // namespace It