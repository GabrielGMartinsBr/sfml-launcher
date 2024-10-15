#include "./AeWindowIntegrator.h"

#include <ruby.h>

#include "RbUtils.hpp"
#include "aeon/window/AeonWindow.h"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Viewport.hpp"

namespace ae {

VALUE AeWindowIntegrator::windowClass;

void AeWindowIntegrator::integrate(VALUE aeonModule)
{
  windowClass = rb_define_class_under(aeonModule, "Window", rb_cObject);

  rb_define_alloc_func(windowClass, instance_allocator);

  rb_define_method(windowClass, "initialize", RUBY_METHOD_FUNC(meth_initialize), -1);

  rb_define_method(windowClass, "windowskin", RUBY_METHOD_FUNC(getter_windowSkin), 0);
  rb_define_method(windowClass, "windowskin=", RUBY_METHOD_FUNC(setter_windowSkin), 1);

  rb_define_method(windowClass, "contents", RUBY_METHOD_FUNC(getter_contents), 0);
  rb_define_method(windowClass, "contents=", RUBY_METHOD_FUNC(setter_contents), 1);

  rb_define_method(windowClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
  rb_define_method(windowClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

  rb_define_method(windowClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
  rb_define_method(windowClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

  rb_define_method(windowClass, "width", RUBY_METHOD_FUNC(getter_width), 0);
  rb_define_method(windowClass, "width=", RUBY_METHOD_FUNC(setter_width), 1);

  rb_define_method(windowClass, "height", RUBY_METHOD_FUNC(getter_height), 0);
  rb_define_method(windowClass, "height=", RUBY_METHOD_FUNC(setter_height), 1);
}

/*
  Allocator
*/

VALUE AeWindowIntegrator::instance_allocator(VALUE instanceClass)
{
  return Data_Wrap_Struct(instanceClass, instance_mark, instance_free, nullptr);
}

/*
  Deallocator
*/

void AeWindowIntegrator::instance_free(void *ptr)
{
  delete static_cast<AeonWindow *>(ptr);
}

void AeWindowIntegrator::instance_mark(void *ptr)
{
}

/*
  Method initialize
*/

VALUE AeWindowIntegrator::meth_initialize(int argc, VALUE *argv, VALUE self)
{
  if (argc == 0) {
    AeonWindow *inst = new AeonWindow(self);
    DATA_PTR(self) = inst;
    return self;
  }

  if (argc == 1) {
    VALUE _viewport = argv[0];

    if (!It::Viewport::isInstance(_viewport)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_viewport),
        It::Viewport::getRbClass()
      );
    }

    Eng::Viewport *viewport = It::Viewport::getObjectValue(_viewport);
    AeonWindow *inst = new AeonWindow(self, viewport);
    DATA_PTR(self) = inst;
    return self;
  }

  RbUtils::raiseRuntimeException(
    "Window initialize takes 0 or 1 argument, but " + std::to_string(argc) + " were received."
  );
  return Qnil;
}

/*
    Get windowSkin
  */

VALUE AeWindowIntegrator::getter_windowSkin(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return It::Bitmap::getRubyObject(
    inst->getter_windowskin()
  );
}

/*
  Set windowSkin
*/

VALUE AeWindowIntegrator::setter_windowSkin(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  Eng::Bitmap *bp = It::Bitmap::getObjectValue(value);
  inst->setter_windowskin(bp);
  return value;
}

/*
    Get contents
  */
VALUE AeWindowIntegrator::getter_contents(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return It::Bitmap::getRubyObject(
    inst->getter_contents()
  );
}

/*
  Set contents
*/
VALUE AeWindowIntegrator::setter_contents(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  Eng::Bitmap *bp = It::Bitmap::getObjectValue(value);
  inst->setter_contents(bp);
  return value;
}

/*
    Get x
  */
VALUE AeWindowIntegrator::getter_x(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return Convert::toRubyNumber(inst->getX());
}

/*
  Set y
*/
VALUE AeWindowIntegrator::setter_x(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  return inst->setter_x(value);
}

/*
  Get y
*/
VALUE AeWindowIntegrator::getter_y(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return Convert::toRubyNumber(inst->getY());
}

/*
  Set y
*/
VALUE AeWindowIntegrator::setter_y(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  return inst->setter_y(value);
}

/*
  Get width
*/
VALUE AeWindowIntegrator::getter_width(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return Convert::toRubyNumber(inst->getWidth());
}

/*
  Set width
*/
VALUE AeWindowIntegrator::setter_width(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  return inst->setter_width(value);
}

/*
  Get height
*/
VALUE AeWindowIntegrator::getter_height(VALUE self)
{
  AeonWindow *inst = getObjectValue(self);
  return Convert::toRubyNumber(inst->getHeight());
}

/*
  Set height
*/
VALUE AeWindowIntegrator::setter_height(VALUE self, VALUE value)
{
  AeonWindow *inst = AeWindowIntegrator::getObjectValue(self);
  return inst->setter_height(value);
}

/*
  Utils
*/

AeonWindow *AeWindowIntegrator::getObjectValue(VALUE rbObj)
{
  return (AeonWindow *)DATA_PTR(rbObj);
}

}  // namespace ae