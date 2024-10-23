#include "aeon/socket/integration/AeonSocketIntegrator.h"

#include "AppDefs.h"
#include "ValueType.hpp"
#include "aeon/socket/AeonSocketManager.hpp"
#include "aeon/socket/integration/AeonSocketIntegrable.h"
#include "integrator/Convert.hpp"

namespace ae {

using app::CStr;

VALUE AeonSocketIntegrator::classObject;

void AeonSocketIntegrator::integrate(VALUE aeonModule)
{
  classObject = rb_define_class_under(aeonModule, "Socket", rb_cObject);

  rb_define_alloc_func(classObject, instanceAllocator);

  rb_define_method(classObject, "initialize", RUBY_METHOD_FUNC(initialize), 0);

  rb_define_method(classObject, "connect", RUBY_METHOD_FUNC(connect), 2);
  rb_define_method(classObject, "close", RUBY_METHOD_FUNC(close), 0);
  rb_define_method(classObject, "disconnect", RUBY_METHOD_FUNC(disconnect), 0);

  rb_define_method(classObject, "send", RUBY_METHOD_FUNC(sendMessage), 1);

  rb_define_method(classObject, "isConnected?", RUBY_METHOD_FUNC(isConnected), 0);

  rb_define_method(classObject, "setConnectHandler", RUBY_METHOD_FUNC(setConnectHandler), 1);
  rb_define_method(classObject, "setMessageHandler", RUBY_METHOD_FUNC(setMessageHandler), 1);
}

// Instance allocator

VALUE AeonSocketIntegrator::instanceAllocator(VALUE instanceClass)
{
  return Data_Wrap_Struct(instanceClass, instanceMark, instanceFree, nullptr);
}

// Instance deallocator

void AeonSocketIntegrator::instanceFree(void *ptr)
{
  AeonSocketManager::Instance().destroy(
    static_cast<AeonSocketIntegrable *>(ptr)
  );
}

// Instance mark

void AeonSocketIntegrator::instanceMark(void *ptr) { }

/*
  ⇩⇩⇩ Instance props and methods ⇩⇩⇩
*/

/*
    Initialize
*/
VALUE AeonSocketIntegrator::initialize(VALUE self)
{
  AeonSocketIntegrable *instance = AeonSocketManager::Instance().create(self);
  DATA_PTR(self) = instance;
  instance->startWorker();
  return self;
}

/*
    Open connection
*/
VALUE AeonSocketIntegrator::connect(VALUE self, VALUE hostRb, VALUE portRb)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);

  if (!ValueTypeUtils::isString(hostRb)) {
    return raiseException("Invalid host param");
  }

  String host = Convert::toCStr(hostRb);
  String port;
  if (ValueTypeUtils::isInteger(portRb)) {
    char buffer[64];
    long num = NUM2LONG(portRb);
    snprintf(buffer, sizeof(buffer), "%ld", num);
    port = String(buffer);
  } else if (ValueTypeUtils::isString(portRb)) {
    port = Convert::toCString(portRb);
  } else {
    return raiseException("Invalid port param");
  }

  VALUE yieldBlock = rb_block_given_p() ? rb_block_proc() : Qnil;
  inst.connect(host, port, yieldBlock);

  return Qnil;
}

/*
    Close
*/
VALUE AeonSocketIntegrator::close(VALUE self)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.close();
  return Qnil;
}

/*
    Disconnect
*/
VALUE AeonSocketIntegrator::disconnect(VALUE self)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.disconnect();
  return Qnil;
}

/*
    Send message
*/
VALUE AeonSocketIntegrator::sendMessage(VALUE self, VALUE message)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.sendMessage(Convert::toCStr(message));
  return Qnil;
}

/*
    Set connection result callback
*/
VALUE AeonSocketIntegrator::setConnectHandler(VALUE self, VALUE handler)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.setConnectHandler(handler);
  return Qnil;
}

/*
    Set message received callback
*/
VALUE AeonSocketIntegrator::setMessageHandler(VALUE self, VALUE handler)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.setMessageHandler(handler);
  return Qnil;
}

VALUE AeonSocketIntegrator::isConnected(VALUE self)
{
  AeonSocketIntegrable &inst = AeonIntegratorBase::getWrappedObject(self);
  return Convert::toRubyBool(inst.isConnected());
}

}  // namespace ae