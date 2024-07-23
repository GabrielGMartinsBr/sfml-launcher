#pragma once

#include "RbUtils.hpp"
#include "engnine/Autotiles.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Table.hpp"
#include "engnine/Tilemap.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Autotiles.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Table.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

namespace It {

class Tilemap {
 public:

  static void integrate()
  {
    VALUE tilemaptRb = rb_define_class("Tilemap", rb_cObject);

    // Initialize

    rb_define_method(tilemaptRb, "initialize", RUBY_METHOD_FUNC(initialize), -1);

    // Properties

    rb_define_method(tilemaptRb, "tileset", RUBY_METHOD_FUNC(getter_tileset), 0);
    rb_define_method(tilemaptRb, "tileset=", RUBY_METHOD_FUNC(setter_tileset), 1);

    rb_define_method(tilemaptRb, "autotiles", RUBY_METHOD_FUNC(getter_autotiles), 0);
    rb_define_method(tilemaptRb, "autotiles=", RUBY_METHOD_FUNC(setter_autotiles), 1);

    rb_define_method(tilemaptRb, "map_data", RUBY_METHOD_FUNC(getter_map_data), 0);
    rb_define_method(tilemaptRb, "map_data=", RUBY_METHOD_FUNC(setter_map_data), 1);

    rb_define_method(tilemaptRb, "flash_data", RUBY_METHOD_FUNC(getter_map_data), 0);
    rb_define_method(tilemaptRb, "flash_data=", RUBY_METHOD_FUNC(setter_map_data), 1);

    rb_define_method(tilemaptRb, "priorities", RUBY_METHOD_FUNC(getter_priorities), 0);
    rb_define_method(tilemaptRb, "priorities=", RUBY_METHOD_FUNC(setter_priorities), 1);

    rb_define_method(tilemaptRb, "visible", RUBY_METHOD_FUNC(getter_visible), 0);
    rb_define_method(tilemaptRb, "visible=", RUBY_METHOD_FUNC(setter_visible), 1);

    rb_define_method(tilemaptRb, "ox", RUBY_METHOD_FUNC(getter_ox), 0);
    rb_define_method(tilemaptRb, "ox=", RUBY_METHOD_FUNC(setter_ox), 1);

    rb_define_method(tilemaptRb, "oy", RUBY_METHOD_FUNC(getter_oy), 0);
    rb_define_method(tilemaptRb, "oy=", RUBY_METHOD_FUNC(setter_oy), 1);

    // Methods

    rb_define_method(tilemaptRb, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(tilemaptRb, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);
    rb_define_method(tilemaptRb, "viewport", RUBY_METHOD_FUNC(method_viewport), 0);
    rb_define_method(tilemaptRb, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Tilemap"));
  }

  static VALUE createRubyObject(Eng::Tilemap *inst)
  {
    return Data_Wrap_Struct(getRbClass(), 0, free, inst);
  }

  static VALUE getRubyObject(Eng::Tilemap *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->ptr == Qnil) {
      inst->ptr = createRubyObject(inst);
    }
    return inst->ptr;
  }

  static Eng::Tilemap *getObjectValue(VALUE rbObj)
  {
    return (Eng::Tilemap *)DATA_PTR(rbObj);
  }

  // Private

 private:

  static Eng::Tilemap *getInstance(VALUE self)
  {
    return (Eng::Tilemap *)DATA_PTR(self);
  }

  /*
    Method initialize
  */

  static VALUE initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Tilemap *inst;

    if (argc == 0) {
      inst = new Eng::Tilemap();
    } else if (argc == 1) {
      VALUE _viewport;
      rb_scan_args(argc, argv, "1", &_viewport);
      Eng::Viewport *viewport = Viewport::getObjectValue(_viewport);
      inst = new Eng::Tilemap(viewport);
    } else {
      RbUtils::raiseRuntimeException(
        "Tilemap initializer takes 1 or 0 argument, but " + std::to_string(argc) + " were received."
      );
      return Qnil;
    }

    inst->ptr = self;
    DATA_PTR(self) = inst;
    return self;
  }

  /*
      Properties
  */

  // Getter tileset

  static VALUE getter_tileset(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    VALUE value = It::Bitmap::getRubyObject(
      inst->getter_tileset()
    );
    return value;
  }

  // Setter tileset

  static VALUE setter_tileset(VALUE self, VALUE _value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    Eng::Bitmap *value = Bitmap::getObjectValue(_value);
    inst->setter_tileset(value);
    return _value;
  }

  // Getter autotiles

  static VALUE getter_autotiles(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return It::Autotiles::getRubyObject(
      inst->getter_autotiles()
    );
  }

  // Setter autotiles

  static VALUE setter_autotiles(VALUE self, VALUE index, VALUE value)
  {
    Eng::Autotiles *inst = It::Autotiles::getObjectValue(self);
    return value;
  }

  // Getter map_data

  static VALUE getter_map_data(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    VALUE value = It::Table::getRubyObject(
      inst->getter_map_data()
    );
    return value;
  }

  // Setter map_data

  static VALUE setter_map_data(VALUE self, VALUE _value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    Eng::Table *value = Table::getObjectValue(_value);
    inst->setter_map_data(value);
    return _value;
  }

  // Getter flash_data

  static VALUE getter_flash_data(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return It::Table::getRubyObject(
      inst->getter_flash_data()
    );
  }

  // Setter flash_data

  static VALUE setter_flash_data(VALUE self, VALUE index, VALUE _value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    Eng::Table *value = Table::getObjectValue(_value);
    inst->setter_flash_data(value);
    return _value;
  }

  // Getter priorities

  static VALUE getter_priorities(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return It::Table::getRubyObject(
      inst->getter_priorities()
    );
  }

  // Setter priorities

  static VALUE setter_priorities(VALUE self, VALUE _value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    Eng::Table *value = Table::getObjectValue(_value);
    inst->setter_priorities(value);
    return _value;
  }

  // Getter visible

  static VALUE getter_visible(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return Convert::toRubyBool(
      inst->getter_visible()
    );
  }

  // Setter visible

  static VALUE setter_visible(VALUE self, VALUE value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    inst->setter_visible(
      Convert::toCBool(value)
    );
    return value;
  }

  // Getter ox

  static VALUE getter_ox(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_ox()
    );
  }

  // Setter ox

  static VALUE setter_ox(VALUE self, VALUE value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    inst->setter_ox(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter oy

  static VALUE getter_oy(VALUE self)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_oy()
    );
  }

  // Setter oy

  static VALUE setter_oy(VALUE self, VALUE value)
  {
    Eng::Tilemap *inst = getObjectValue(self);
    inst->setter_oy(
      Convert::toCInt2(value)
    );
    return value;
  }

  /*
      Methods
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Tilemap *inst = getInstance(self);
    inst->method_dispose();
    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Tilemap *inst = getInstance(self);
    return Convert::toRubyBool(
      inst->method_disposed()
    );
  }

  static VALUE method_viewport(VALUE self)
  {
    Eng::Tilemap *inst = getInstance(self);
    return Viewport::getRubyObject(
      inst->method_viewport()
    );
  }

  static VALUE method_update(VALUE self)
  {
    Eng::Tilemap *inst = getInstance(self);
    inst->method_update();
    return Qnil;
  }
};

}  // namespace It