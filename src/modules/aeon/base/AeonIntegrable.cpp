#include "aeon/base/AeonIntegrable.h"

#include <SFML/System/String.hpp>
#include <cassert>

#include "AppDefs.h"
#include "integrator/Convert.hpp"

namespace ae {

AeonIntegrable::AeonIntegrable(VALUE rbId) :
    rbId(rbId)
{
}

void AeonIntegrable::handleInitialize(VALUE rbId)
{
  this->rbId = rbId;
}

/*
  ⇩⇩⇩ Set Ruby Variable ⇩⇩⇩
*/

VALUE AeonIntegrable::setInstanceVar(CStr name, VALUE value)
{
  assert(isWrapped());
  rb_iv_set(rbId, name, value);
  return value;
}

VALUE AeonIntegrable::setInstanceVar(CStr name, bool value)
{
  return setInstanceVar(name, toRubyValue(value));
}

VALUE AeonIntegrable::setInstanceVar(CStr name, int value)
{
  return setInstanceVar(name, toRubyValue(value));
}

VALUE AeonIntegrable::setInstanceVar(CStr name, double value)
{
  return setInstanceVar(name, toRubyValue(value));
}

VALUE AeonIntegrable::setInstanceVar(CStr name, CStr value)
{
  return setInstanceVar(name, toRubyValue(value));
}

VALUE AeonIntegrable::setInstanceVar(CStr name, const String& value)
{
  return setInstanceVar(name, toRubyValue(value));
}

VALUE AeonIntegrable::setInstanceVar(CStr name, AeonIntegrable* value)
{
  return setInstanceVar(name, toRubyValue(value));
}

/*
  ⇩⇩⇩ Ruby Value Conversor ⇩⇩⇩
*/

VALUE AeonIntegrable::toRubyValue(bool value)
{
  return Convert::toRubyBool(value);
}

VALUE AeonIntegrable::toRubyValue(int value)
{
  return Convert::toRubyNumber(value);
}

VALUE AeonIntegrable::toRubyValue(long value)
{
  return Convert::toRubyBigNumber(value);
}

VALUE AeonIntegrable::toRubyValue(double value)
{
  return Convert::toRubyDouble(value);
}

VALUE AeonIntegrable::toRubyValue(CStr value)
{
  return Convert::toRubyString(value);
}

VALUE AeonIntegrable::toRubyValue(const String& value)
{
  return Convert::toRubyString(value);
}

VALUE AeonIntegrable::toRubyValue(AeonIntegrable* value)
{
  return value->isWrapped() ? value->rbId : Qnil;
}

}  // namespace ae