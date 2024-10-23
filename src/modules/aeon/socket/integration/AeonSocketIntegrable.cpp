#include "./AeonSocketIntegrable.h"

#include "Log.hpp"
#include "aeon/base/AeonIntegrable.h"
#include "engnine/Lists.hpp"
#include "integrator/Convert.hpp"

namespace ae {

/*
  Constructor
*/
AeonSocketIntegrable::AeonSocketIntegrable(VALUE rbId) :
    AeonSocketWorker(),
    AeonIntegrable(rbId)
{
  connectHandler = Qnil;
  messageHandler = Qnil;
  connectYieldBlock = Qnil;
  connectHandlerResult = Qnil;
  addedToEngineCycles = false;
  addToEngineCycles();
}

/*
  Destructor
*/
AeonSocketIntegrable::~AeonSocketIntegrable()
{
  setConnectHandler(Qnil);
  setMessageHandler(Qnil);
  setConnectYieldBlock(Qnil);
  removeFromEngineCycles();
}

void AeonSocketIntegrable::onUpdate()
{
  if (connectHandlerResult != Qnil) {
    if (connectHandler != Qnil) {
      rb_funcall(connectHandler, rb_intern("call"), 1, connectHandlerResult);
    }
    if (connectYieldBlock != Qnil) {
      rb_funcall(connectYieldBlock, rb_intern("call"), 1, connectHandlerResult);
    }
    connectHandlerResult = Qnil;
  }
  processMessages();
}

/*
  Open socket connection
*/
void AeonSocketIntegrable::connect(const String &host, const String &port, VALUE yieldBlock)
{
  if (yieldBlock != Qnil) {
    setConnectYieldBlock(yieldBlock);
  }

  if (socket && socket->socket.is_open()) {
    Log::err() << "Socket is already connected.";
    handleConnectResult(false);
    return;
  }

  enqueueAsyncTask([this, host, port, yieldBlock](SPtr<AeonSocket> &socket) {
    socket->connectAsync(
      host,
      port,
      [this, yieldBlock](const boost::system::error_code &ec, const tcp::endpoint &endpoint) {
        if (!ec) {
          handleConnectResult(true);
        } else {
          handleConnectResult(false);
          // Log::err() << "Error during connect: " << ec.message();
        }
      }
    );
  });
}

/*
  Read data from socket connection using a char as chunk delimiter
*/
void AeonSocketIntegrable::readMessagesByDelimiter(char delim)
{
  enqueueAsyncTask([delim, this](SPtr<AeonSocket> &socket) {
    if (socket) {
      socket->asyncReadUntil(
        delim,
        [this](const boost::system::error_code &ec, const String &msg) {
          if (ec) {
            if (ec != boost::asio::error::operation_aborted) {
              Log::err() << "Error reading message: " << ec.message();
            }
          } else {
            addMessage(std::move(msg));
          }
        }
      );
    }
  });
}

/*
  Write data to socket connection
*/
void AeonSocketIntegrable::sendMessage(const String &message)
{
  enqueueAsyncTask([message](SPtr<AeonSocket> &socket) {
    if (socket) {
      socket->asyncSend(message, [](const boost::system::error_code &ec, std::size_t) {
        if (ec) {
          Log::err() << "Error sending message: " << ec.message();
        }
      });
    }
  });
}

/*
    Close socket connection
*/
void AeonSocketIntegrable::close()
{
  socket->close();
}

void AeonSocketIntegrable::shutdown()
{
  Log::out() << "shutdown";
  socket->disconnect();
}

/*
    Close and shutdown socket connection
*/
void AeonSocketIntegrable::disconnect()
{
  enqueueAsyncTask([](std::shared_ptr<ae::AeonSocket> &socket) {
    socket->disconnect();
  });
}

/*
  Getters
*/

bool AeonSocketIntegrable::isConnected()
{
  return socket->isConnected();
}

/*
  Setters
*/

void AeonSocketIntegrable::setConnectHandler(VALUE handler)
{
  if (connectHandler == handler) {
    return;
  } else if (connectHandler != Qnil) {
    rb_gc_unregister_address(&connectHandler);
  }
  connectHandler = handler;
  if (connectHandler != Qnil) {
    rb_gc_register_address(&connectHandler);
  }
}

void AeonSocketIntegrable::setMessageHandler(VALUE handler)
{
  if (messageHandler == handler) {
    return;
  } else if (messageHandler != Qnil) {
    rb_gc_unregister_address(&messageHandler);
  }
  messageHandler = handler;
  if (messageHandler != Qnil) {
    rb_gc_register_address(&messageHandler);
  }
}

void AeonSocketIntegrable::setConnectYieldBlock(VALUE block)
{
  if (connectYieldBlock == block) {
    return;
  } else if (connectYieldBlock != Qnil) {
    rb_gc_unregister_address(&connectYieldBlock);
  }
  connectYieldBlock = block;
  if (connectYieldBlock != Qnil) {
    rb_gc_register_address(&connectYieldBlock);
  }
}

/*
    ⇩⇩⇩ Private ⇩⇩⇩
*/

/*
  Handlers
*/

static VALUE safe_call_handler(VALUE args)
{
  VALUE *arr = (VALUE *)args;
  VALUE handler = arr[0];
  VALUE result = arr[1];

  return rb_funcall(handler, rb_intern("call"), 1, result);
}

void AeonSocketIntegrable::handleConnectResult(bool success)
{
  connectHandlerResult = Convert::toRubyBool(success);
  if (success) {
    readMessagesByDelimiter('\n');
  }
}

void AeonSocketIntegrable::handleMessage(const String &message)
{
  // addMessage(message);
}

void AeonSocketIntegrable::addToEngineCycles()
{
  if (addedToEngineCycles) return;
  Eng::Lists::Instance().addUpdateEntry(this);
  addedToEngineCycles = true;
}

void AeonSocketIntegrable::removeFromEngineCycles()
{
  if (!addedToEngineCycles) return;
  Eng::Lists::Instance().removeUpdateEntry(this);
  addedToEngineCycles = false;
}

void AeonSocketIntegrable::addMessage(const String &message)
{
  messageQueue.push_back(std::move(message));
}

void AeonSocketIntegrable::processMessages()
{
  if (messageHandler == Qnil) {
    return;
  }

  while (!messageQueue.empty()) {
    String message = std::move(messageQueue.front());
    messageQueue.pop_front();
    rb_funcall(messageHandler, rb_intern("call"), 1, Convert::toRubyString(message));
  }
}

}  // namespace ae