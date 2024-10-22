#include "aeon/socket/integration/AeonSocketIntegrator.h"

#include "AppDefs.h"
#include "ValueType.hpp"
#include "aeon/socket/AeonSocket.hpp"
#include "aeon/socket/AeonSocketManager.hpp"
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
  rb_define_method(classObject, "disconnect", RUBY_METHOD_FUNC(disconnect), 0);

  rb_define_method(classObject, "send", RUBY_METHOD_FUNC(sendMessage), 1);
  rb_define_method(classObject, "onMessage", RUBY_METHOD_FUNC(setOnMessageCallback), 1);
  rb_define_method(classObject, "pollMessages", RUBY_METHOD_FUNC(pollMessages), 0);

  rb_define_method(classObject, "isConnected?", RUBY_METHOD_FUNC(isConnected), 0);
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
    static_cast<AeonSocket *>(ptr)
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
  AeonSocket *instance = AeonSocketManager::Instance().create();

  DATA_PTR(self) = instance;
  instance->handleInitialize(self);

  return self;
}

/*
    Open connection
*/
VALUE AeonSocketIntegrator::connect(VALUE self, VALUE hostRb, VALUE portRb)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);

  if (!ValueTypeUtils::isString(hostRb)) {
    return raiseException("Invalid host param");
  }

  String host = Convert::toCStr(hostRb);
  String port;
  if (ValueTypeUtils::isInteger(portRb)) {
    char buffer[64];
    long num = NUM2LONG(portRb);
    snprintf(buffer, sizeof(buffer), "%ld", num);
    port = std::string(buffer);
  } else if (ValueTypeUtils::isString(portRb)) {
    port = Convert::toCString(portRb);
  } else {
    return raiseException("Invalid port param");
  }

  VALUE cbBlock = rb_block_given_p() ? rb_block_proc() : Qnil;

  inst.connect(host, port, cbBlock);

  return Qnil;
}

/*
    Disconnect
*/
VALUE AeonSocketIntegrator::disconnect(VALUE self)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.close();
  return Qnil;
}

/*
    Send message
*/
VALUE AeonSocketIntegrator::sendMessage(VALUE self, VALUE message)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.send(Convert::toCStr(message));
  return Qnil;
}

/*
    Set message received callback
*/
VALUE AeonSocketIntegrator::setOnMessageCallback(VALUE self, VALUE callback)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.setOnMessageCallback(callback);
  return Qnil;
}

/*
    Poll messages
*/
VALUE AeonSocketIntegrator::pollMessages(VALUE self)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.processMessages();
  return Qnil;
}

VALUE AeonSocketIntegrator::isConnected(VALUE self)
{
  AeonSocket &inst = AeonIntegratorBase::getWrappedObject(self);
  return Convert::toRubyBool(inst.isConnected());
}

}  // namespace ae