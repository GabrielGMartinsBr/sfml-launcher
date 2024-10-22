#pragma once

#include <boost/asio.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/bind/bind.hpp>
#include <queue>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/base/AeonIntegrable.h"
#include "integrator/Convert.hpp"

namespace ae {

using app::CStr;
using app::String;
using app::Thread;
using boost::asio::ip::tcp;

struct AeonSocket : public AeonIntegrable, public std::enable_shared_from_this<AeonSocket> {
  boost::asio::io_context io_context;
  tcp::resolver resolver;
  tcp::socket socket;
  VALUE rubyCallback = Qnil;

  AeonSocket() :
      resolver(io_context),
      socket(io_context)
  {
    running = false;
    _isConnected = false;
    io_context.run();
  }

  ~AeonSocket()
  {
    io_context.stop();
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  bool isConnected()
  {
    return socket.is_open();
  }

  void connect(const String& _host, const String& _port, VALUE cbBlock = Qnil)
  {
    host = _host;
    port = _port;
    thread_ = Thread(&AeonSocket::tryConnect, this, cbBlock);
  }

  void send(const String& message)
  {
    if (socket.is_open()) {
      boost::asio::write(socket, boost::asio::buffer(message));
    } else {
      Log::err() << "Message not sent, server is closed.";
    }
  }

  void processMessages()
  {
    while (!messageQueue.empty()) {
      if (!running) break;
      std::lock_guard<std::mutex> lock(queueMutex);

      std::string message = messageQueue.front();
      messageQueue.pop();

      if (rubyCallback != Qnil) {
        VALUE rb_message = Convert::toRubyString(message);
        rb_funcall(rubyCallback, rb_intern("call"), 1, rb_message);
      }
    }
  }

  void setOnMessageCallback(VALUE callback)
  {
    rubyCallback = callback;
    rb_gc_register_address(&rubyCallback);
  }

  void close()
  {
    running = false;
    if (socket.is_open()) {
      try {
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket.close();
        Log::out() << "Connection closed.";
      } catch (const std::exception& e) {
        Log::err() << "Error closing socket: " << e.what();
      }
    }
  }


 private:
  Thread thread_;
  String host;
  String port;
  std::queue<String> messageQueue;
  std::mutex queueMutex;
  std::mutex runningMutex;
  std::condition_variable condition;
  bool running;
  bool _isConnected;

  void tryConnect(VALUE cbBlock = Qnil)
  {
    bool result = false;
    try {
      connect();
      running = true;
      result = true;
    } catch (std::exception& e) {
      result = false;
      Log::err() << "Erro: " << e.what();
    }

    if (cbBlock != Qnil) {
      rb_funcall(cbBlock, rb_intern("call"), 1, Convert::toRubyBool(result));
    }

    if (running) {
      startReceive();
    }
  }

  void connect()
  {
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket, endpoints);
  }

  void startReceive()
  {
    try {
      while (running) {
        std::string msg;
        size_t length = boost::asio::read_until(
          socket,
          boost::asio::dynamic_buffer(msg),
          '\n'
        );

        if (length > 0) {
          std::string message = msg.substr(0, length);
          msg.erase(0, length);
          addMessage(message);
        }
      }
    } catch (const std::exception& e) {
      Log::err() << "Error while receiving: " << e.what();
    }
  }

  void addMessage(const std::string& message)
  {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(message);
    condition.notify_one();
  }
};

}  // namespace ae