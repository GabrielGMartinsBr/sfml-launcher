#pragma once

#include "Convert.hpp"
#include "RbUtils.hpp"
#include "engnine/Bitmap.h"
#include "engnine/Rect.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Font.hpp"
#include "integrator/It_Rect.hpp"
#include "ruby.h"

namespace It {

class Bitmap {

 public:

  static void integrate()
  {
    VALUE bitmapClass = rb_define_class("Bitmap", rb_cObject);
    rb_define_alloc_func(bitmapClass, instance_allocator);

    rb_define_method(bitmapClass, "initialize", RUBY_METHOD_FUNC(initialize), -1);

    rb_define_method(bitmapClass, "font", RUBY_METHOD_FUNC(getter_font), 0);
    rb_define_method(bitmapClass, "font=", RUBY_METHOD_FUNC(setter_font), 1);

    rb_define_method(bitmapClass, "clear", RUBY_METHOD_FUNC(method_clear), 0);

    rb_define_method(bitmapClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(bitmapClass, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);

    rb_define_method(bitmapClass, "width", RUBY_METHOD_FUNC(method_width), 0);
    rb_define_method(bitmapClass, "height", RUBY_METHOD_FUNC(method_height), 0);

    rb_define_method(bitmapClass, "get_pixel", RUBY_METHOD_FUNC(method_get_pixel), 2);
    rb_define_method(bitmapClass, "set_pixel", RUBY_METHOD_FUNC(method_set_pixel), 3);

    rb_define_method(bitmapClass, "fill_rect", RUBY_METHOD_FUNC(method_fill_rect), -1);

    rb_define_method(bitmapClass, "draw_text", RUBY_METHOD_FUNC(method_draw_text), -1);

    rb_define_method(bitmapClass, "text_size", RUBY_METHOD_FUNC(method_text_size), 1);

    rb_define_method(bitmapClass, "rect", RUBY_METHOD_FUNC(getter_rect), 0);

    rb_define_method(bitmapClass, "blt", RUBY_METHOD_FUNC(method_blt), -1);

    rb_define_method(bitmapClass, "stretch_blt", RUBY_METHOD_FUNC(method_stretch_blt), -1);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Bitmap"));
  }

  static VALUE createRubyObject(Eng::Bitmap *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Bitmap *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Bitmap *getObjectValue(VALUE rbObj)
  {
    return (Eng::Bitmap *)DATA_PTR(rbObj);
  }

  static inline bool isInstanceFrom(VALUE inst)
  {
    return rb_class_of(inst) == getRbClass();
  }


 private:

  /*
    Allocator
  */

  static VALUE instance_allocator(VALUE instanceClass)
  {
    return Data_Wrap_Struct(instanceClass, instance_mark, instance_free, nullptr);
  }

  /*
    Deallocator
  */

  static void instance_free(void *ptr)
  {
    Log::out() << "[[bitmap_free]]";
    delete static_cast<Eng::Bitmap *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
    Eng::Font *font = static_cast<Eng::Bitmap *>(ptr)->getter_font();
    if (font != nullptr && font->rbObj != Qnil) {
      rb_gc_mark(font->rbObj);
    }
  }

  /*
    Method initialize
  */

  static VALUE initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 2) {
      VALUE _width, _height;
      rb_scan_args(argc, argv, "2", &_width, &_height);
      return initializeSize(self, _width, _height);
    }

    if (argc == 1) {
      VALUE _fileName;
      rb_scan_args(argc, argv, "1", &_fileName);
      return initializeImage(self, _fileName);
    }

    RbUtils::raiseRuntimeException(
      "Bitmap initializer takes 1 or 2 arguments, but " + std::to_string(argc) + " were received."
    );
    return Qnil;
  }

  /*
    Initializer with dimensions
  */

  static VALUE initializeSize(VALUE self, VALUE _width, VALUE _height)
  {
    unsigned int width = Convert::toCInt2(_width);
    unsigned int height = Convert::toCInt2(_height);
    Eng::Bitmap *inst = new Eng::Bitmap(width, height, self);
    DATA_PTR(self) = inst;
    return self;
  }

  /*
    Initializer with image
  */

  static VALUE initializeImage(VALUE self, VALUE _fileName)
  {
    const char *fileName = Convert::toCStr(_fileName);
    Eng::Bitmap *inst = new Eng::Bitmap(fileName, self);
    DATA_PTR(self) = inst;
    return self;
  }

  /*
   Get font
  */
  static VALUE getter_font(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Font *font = inst->getter_font();
    return Font::getRubyObject(font);
  }

  /*
    Set font
  */
  static VALUE setter_font(VALUE self, VALUE value)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Font *prev = inst->getter_font();
    Eng::Font *font = Font::getObjectValue(value);
    inst->setter_font(font);
    return value;
  }

  /*
   Get rect
  */
  static VALUE getter_rect(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Rect *rect = new Eng::Rect(inst->get_rect());
    VALUE value = It::Rect::getRubyObject(rect);
    return value;
  }

  /*
    Method clear
  */

  static VALUE method_clear(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    inst->clear();
    return Qnil;
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    inst->dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    return inst->disposed() ? Qtrue : Qfalse;
  }

  /*
    Method get_width
  */

  static VALUE method_width(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_width()
    );
  }

  /*
    Method get_height
  */

  static VALUE method_height(VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_height()
    );
  }

  /*
    Method get_pixel
  */

  static VALUE method_get_pixel(VALUE self, VALUE _x, VALUE _y)
  {
    unsigned int x = Convert::toCInt2(_x);
    unsigned int y = Convert::toCInt2(_y);

    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Color *color = inst->get_pixel(x, y);
    return Color::getRubyObject(color);
  }

  /*
    Method set_pixel
  */

  static VALUE method_set_pixel(VALUE self, VALUE _x, VALUE _y, VALUE _color)
  {
    if (!Color::isInstance(_color)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_color),
        Color::getRbClass()
      );
      return Qnil;
    }

    unsigned int x = Convert::toCInt2(_x);
    unsigned int y = Convert::toCInt2(_y);

    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Color *color = Color::getObjectValue(_color);

    inst->set_pixel(x, y, color);

    return Qnil;
  }

  /*
    Method initialize
  */

  static VALUE method_fill_rect(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 5) {
      return overload_fill_rect1(argc, argv, self);
    } else if (argc == 2) {
      return overload_fill_rect2(argc, argv, self);
    } else {
      RbUtils::raiseRuntimeException(
        "Bitmap fill_rect takes 2 or 5 arguments, but " + std::to_string(argc) + " were received."
      );
      return Qnil;
    }
  }

  static VALUE overload_fill_rect1(int argc, VALUE *argv, VALUE self)
  {
    VALUE _x, _y, _width, _height, _color;
    rb_scan_args(argc, argv, "5", &_x, &_y, &_width, &_height, &_color);

    int x = Convert::toCInt(_x);
    int y = Convert::toCInt(_y);
    unsigned int width = Convert::toCInt(_width);
    unsigned int height = Convert::toCInt(_height);

    Eng::Bitmap *inst = getObjectValue(self);
    Eng::Color *color = Color::getObjectValue(_color);

    inst->fill_rect(x, y, width, height, color);

    return self;
  }

  static VALUE overload_fill_rect2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _rect, _color;
    rb_scan_args(argc, argv, "2", &_rect, &_color);

    if (!Rect::isInstance(_rect)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_rect),
        Rect::getRbClass()
      );
      return Qnil;
    }

    if (!Color::isInstance(_color)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_color),
        Color::getRbClass()
      );
      return Qnil;
    }

    Eng::Rect *rect = Rect::getObjectValue(_rect);
    Eng::Color *color = Color::getObjectValue(_color);

    Eng::Bitmap *inst = Bitmap::getObjectValue(self);

    inst->fill_rect(rect, color);

    return self;
  }

  static VALUE method_draw_text(int argc, VALUE *argv, VALUE self)
  {
    VALUE _x, _y, _width, _height, _rect, _str, _align;
    Eng::Bitmap *inst = getObjectValue(self);

    switch (argc) {
      case 2: {
        rb_scan_args(argc, argv, "2", &_rect, &_str);
        Eng::Rect *rect = Rect::getObjectValue(_rect);
        app::CStr str = Convert::toCStr(_str);
        inst->draw_text(rect, str);
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_rect, &_str, &_align);
        Eng::Rect *rect = Rect::getObjectValue(_rect);
        app::CStr str = Convert::toCStr(_str);
        inst->draw_text(rect, str);
        break;
      }
      case 5: {
        rb_scan_args(argc, argv, "5", &_x, &_y, &_width, &_height, &_str);
        int x = Convert::toCInt(_x);
        int y = Convert::toCInt(_y);
        int width = Convert::toCInt(_width);
        int height = Convert::toCInt(_height);
        app::CStr str = Convert::toCStr(_str);
        inst->draw_text(x, y, width, height, str);
        break;
      }
      case 6: {
        rb_scan_args(argc, argv, "6", &_x, &_y, &_width, &_height, &_str, &_align);
        int x = Convert::toCInt(_x);
        int y = Convert::toCInt(_y);
        int width = Convert::toCInt(_width);
        int height = Convert::toCInt(_height);
        app::CStr str = Convert::toCStr(_str);
        inst->draw_text(x, y, width, height, str);
        break;
      }
      default: {
        RbUtils::raiseRuntimeException("Bad number of arguments was receive.");
        return Qnil;
      }
    }

    return Qnil;
  }

  static VALUE method_text_size(VALUE self, VALUE _str)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    app::CStr str = Convert::toCStr(_str);
    Eng::Rect *rect = inst->get_text_size(str);
    VALUE rectObj = It::Rect::getRubyObject(rect);
    return rectObj;
  }

  static VALUE method_blt(int argc, VALUE *argv, VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    VALUE _x, _y, _src_bitmap, _src_rect, _opacity;

    if (argc == 4) {
      rb_scan_args(argc, argv, "4", &_x, &_y, &_src_bitmap, &_src_rect);

      int x = Convert::toCInt(_x);
      int y = Convert::toCInt(_y);
      Eng::Bitmap *src_bitmap = Bitmap::getObjectValue(_src_bitmap);
      Eng::Rect *src_rect = Rect::getObjectValue(_src_rect);

      inst->blt(x, y, src_bitmap, src_rect);
      return Qnil;
    }

    if (argc == 5) {
      rb_scan_args(argc, argv, "5", &_x, &_y, &_src_bitmap, &_src_rect, &_opacity);

      int x = Convert::toCInt(_x);
      int y = Convert::toCInt(_y);
      int opacity = Convert::toCInt(_opacity);
      Eng::Bitmap *src_bitmap = Bitmap::getObjectValue(_src_bitmap);
      Eng::Rect *src_rect = Rect::getObjectValue(_src_rect);

      inst->blt(x, y, src_bitmap, src_rect, opacity);

      return Qnil;
    }

    RbUtils::raiseRuntimeException("Bad number of arguments was receive.");

    return Qnil;
  }

  static VALUE method_stretch_blt(int argc, VALUE *argv, VALUE self)
  {
    Eng::Bitmap *inst = getObjectValue(self);
    VALUE _dst_rect, _src_bitmap, _src_rect, _opacity;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &_dst_rect, &_src_bitmap, &_src_rect);

      Eng::Rect *dst_rect = Rect::getObjectValue(_dst_rect);
      Eng::Bitmap *src_bitmap = Bitmap::getObjectValue(_src_bitmap);
      Eng::Rect *src_rect = Rect::getObjectValue(_src_rect);

      inst->stretch_blt(dst_rect, src_bitmap, src_rect);
      return Qnil;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "4", &_dst_rect, &_src_bitmap, &_src_rect, &_opacity);

      Eng::Rect *dst_rect = Rect::getObjectValue(_dst_rect);
      int opacity = Convert::toCInt(_opacity);
      Eng::Bitmap *src_bitmap = Bitmap::getObjectValue(_src_bitmap);
      Eng::Rect *src_rect = Rect::getObjectValue(_src_rect);

      inst->stretch_blt(dst_rect, src_bitmap, src_rect, opacity);

      return Qnil;
    }

    RbUtils::raiseRuntimeException("Bad number of arguments was receive.");

    return Qnil;
  }
};

}  // namespace It