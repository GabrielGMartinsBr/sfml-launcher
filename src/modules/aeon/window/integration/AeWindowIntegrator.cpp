#include "./AeWindowIntegrator.h"

#include <ruby.h>

#include "AppDefs.h"
#include "aeon/window/AeonElement.h"
#include "aeon/window/AeonWindow.h"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Viewport.hpp"

namespace ae {

using app::SPtr;

VALUE AeWindowIntegrator::classObject;

void AeWindowIntegrator::integrate(VALUE aeonModule)
{
  classObject = rb_define_class_under(aeonModule, "Window", rb_cObject);
  rb_define_alloc_func(classObject, instance_allocator);

  rb_define_method(classObject, "initialize", RUBY_METHOD_FUNC(meth_initialize), -1);

  rb_define_method(classObject, "getHoverKey", RUBY_METHOD_FUNC(getHoverElementKey), 0);
  rb_define_method(classObject, "getFocusKey", RUBY_METHOD_FUNC(getFocusElementKey), 0);
  rb_define_method(classObject, "getClickKey", RUBY_METHOD_FUNC(getClickElementKey), 0);
  rb_define_method(classObject, "getTriggerKey", RUBY_METHOD_FUNC(getTriggerElementKey), 0);

  rb_define_method(classObject, "disposed", RUBY_METHOD_FUNC(disposed), 0);
  rb_define_method(classObject, "dispose", RUBY_METHOD_FUNC(dispose), 0);

  rb_define_method(classObject, "focus", RUBY_METHOD_FUNC(focus), -1);
  rb_define_method(classObject, "lockFocus", RUBY_METHOD_FUNC(lockFocus), 1);

  rb_define_method(classObject, "addElement", RUBY_METHOD_FUNC(addElement), 1);

  rb_define_method(classObject, "windowskin", RUBY_METHOD_FUNC(getter_windowSkin), 0);
  rb_define_method(classObject, "windowskin=", RUBY_METHOD_FUNC(setter_windowSkin), 1);

  rb_define_method(classObject, "contents", RUBY_METHOD_FUNC(getter_contents), 0);
  rb_define_method(classObject, "contents=", RUBY_METHOD_FUNC(setter_contents), 1);

  rb_define_method(classObject, "x", RUBY_METHOD_FUNC(getter_x), 0);
  rb_define_method(classObject, "x=", RUBY_METHOD_FUNC(setter_x), 1);

  rb_define_method(classObject, "y", RUBY_METHOD_FUNC(getter_y), 0);
  rb_define_method(classObject, "y=", RUBY_METHOD_FUNC(setter_y), 1);

  rb_define_method(classObject, "width", RUBY_METHOD_FUNC(getter_width), 0);
  rb_define_method(classObject, "width=", RUBY_METHOD_FUNC(setter_width), 1);

  rb_define_method(classObject, "height", RUBY_METHOD_FUNC(getter_height), 0);
  rb_define_method(classObject, "height=", RUBY_METHOD_FUNC(setter_height), 1);
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

  return raiseException(
    "Window initialize takes 0 or 1 argument, but " + std::to_string(argc) + " were received."
  );
}

VALUE AeWindowIntegrator::getHoverElementKey(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  return Convert::toRubyString(inst.getHoverElementKey());
}

VALUE AeWindowIntegrator::getFocusElementKey(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  return Convert::toRubyString(inst.getFocusElementKey());
}

VALUE AeWindowIntegrator::getClickElementKey(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  return Convert::toRubyString(inst.getClickElementKey());
}

VALUE AeWindowIntegrator::getTriggerElementKey(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  return Convert::toRubyString(inst.getTriggerElementKey());
}

/*
  Dispose
*/

VALUE AeWindowIntegrator::disposed(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  return Convert::toRubyBool(inst.method_disposed());
}

VALUE AeWindowIntegrator::dispose(VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);
  inst.method_dispose();
  return Qnil;
}

VALUE AeWindowIntegrator::focus(int argc, VALUE *argv, VALUE self)
{
  AeonWindow &inst = getWrappedObject(self);

  if (argc == 0) {
    return Convert::toRubyBool(inst.focus());
  } else if (argc == 1) {
    inst.focus(Convert::toRubyBool(argv[0]));
    return Qnil;
  }

  return raiseArgException("AeonWindow focus", "0 or 1", argc);
}

VALUE AeWindowIntegrator::lockFocus(VALUE self, VALUE value)
{
  AeonWindow &inst = getWrappedObject(self);
  inst.lockFocus(Convert::toRubyBool(value));
  return Qnil;
}

/*
  Add AeonElement to window
*/

VALUE AeWindowIntegrator::addElement(VALUE self, VALUE value)
{
  AeonWindow *inst = getObjectValue(self);
  AeonElement *element = static_cast<AeonElement *>(DATA_PTR(value));

  inst->addElement(element);

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

}  // namespace ae