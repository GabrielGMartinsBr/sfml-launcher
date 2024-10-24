#pragma once

#include "engnine/Input.h"
#include "engnine/Input.h"
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

    

    rb_define_const(inputModule, "DOWN", Convert::toRubyNumber(Eng::InputCode::DOWN));
    rb_define_const(inputModule, "LEFT", Convert::toRubyNumber(Eng::InputCode::LEFT));
    rb_define_const(inputModule, "RIGHT", Convert::toRubyNumber(Eng::InputCode::RIGHT));
    rb_define_const(inputModule, "UP", Convert::toRubyNumber(Eng::InputCode::UP));
    rb_define_const(inputModule, "A", Convert::toRubyNumber(Eng::InputCode::A));
    rb_define_const(inputModule, "B", Convert::toRubyNumber(Eng::InputCode::B));
    rb_define_const(inputModule, "C", Convert::toRubyNumber(Eng::InputCode::C));
    rb_define_const(inputModule, "X", Convert::toRubyNumber(Eng::InputCode::X));
    rb_define_const(inputModule, "Y", Convert::toRubyNumber(Eng::InputCode::Y));
    rb_define_const(inputModule, "Z", Convert::toRubyNumber(Eng::InputCode::Z));
    rb_define_const(inputModule, "L", Convert::toRubyNumber(Eng::InputCode::L));
    rb_define_const(inputModule, "R", Convert::toRubyNumber(Eng::InputCode::R));
    rb_define_const(inputModule, "SHIFT", Convert::toRubyNumber(Eng::InputCode::SHIFT));
    rb_define_const(inputModule, "CTRL", Convert::toRubyNumber(Eng::InputCode::CTRL));
    rb_define_const(inputModule, "ALT", Convert::toRubyNumber(Eng::InputCode::ALT));
    rb_define_const(inputModule, "F5", Convert::toRubyNumber(Eng::InputCode::F5));
    rb_define_const(inputModule, "F6", Convert::toRubyNumber(Eng::InputCode::F6));
    rb_define_const(inputModule, "F7", Convert::toRubyNumber(Eng::InputCode::F7));
    rb_define_const(inputModule, "F8", Convert::toRubyNumber(Eng::InputCode::F8));
    rb_define_const(inputModule, "F9", Convert::toRubyNumber(Eng::InputCode::F9));
  }

  static VALUE method_Update(VALUE self)
  {
    Eng::Input::Instance().update();
    return Qnil;
  }

  static VALUE method_press(VALUE self, VALUE _num)
  {
    Eng::InputCode::Key key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::Instance().isPressed(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_trigger(VALUE self, VALUE _num)
  {
    Eng::InputCode::Key key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::Instance().isTriggered(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_repeat(VALUE self, VALUE _num)
  {
    // Log::out() << "repeat";
    Eng::InputCode::Key key = convertKeyCode(_num);
    if (!key) {
      return Qnil;
    }
    bool isPressed = Eng::Input::Instance().isRepeated(key);
    return isPressed ? Qtrue : Qfalse;
  }

  static VALUE method_dir4(VALUE self, VALUE _num)
  {
    return Convert::toRubyNumber(
      Eng::Input::Instance().getDir4()
    );
  }

  static VALUE method_dir8(VALUE self, VALUE _num)
  {
    return Convert::toRubyNumber(0);
  }

  static Eng::InputCode::Key convertKeyCode(VALUE _num)
  {
    int num = Convert::toCInt(_num);
    if (Eng::Input::Instance().isValidKey(num)) {
      return (Eng::InputCode::Key)num;
    }
    return Eng::InputCode::Key::UNKNOW;
  }
};

}  // namespace It