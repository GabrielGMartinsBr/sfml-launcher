#pragma once

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/bind/bind.hpp>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/base/AeonIntegrable.h"

namespace ae {

using app::CStr;
using app::String;
using boost::asio::ip::tcp;

using AsyncConnectCallback = std::function<void(const boost::system::error_code&, const tcp::endpoint&)>;
using AsyncSendCallback = std::function<void(const boost::system::error_code&, std::size_t)>;
using AsyncReadCallback = std::function<void(const boost::system::error_code&, std::string)>;

struct AeonSocketBase : public std::enable_shared_from_this<AeonSocketBase> {
  boost::asio::io_context& io_context;
  tcp::resolver resolver;
  tcp::socket socket;

  AeonSocketBase(boost::asio::io_context& io_context) :
      io_context(io_context),
      resolver(io_context),
      socket(io_context)
  {
    running = true;
  }

  ~AeonSocketBase()
  {
  }

  bool isConnected()
  {
    return socket.is_open();
  }

  void connectAsync(const String& _host, const String& _port, AsyncConnectCallback callback)
  {
    host = _host;
    port = _port;
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    auto self(shared_from_this());
    boost::asio::async_connect(
      socket,
      endpoints,
      callback
    );
  }

  void disconnect()
  {
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

  void asyncSend(const std::string& message, AsyncSendCallback callback)
  {
    auto self(shared_from_this());
    auto send_buffer = std::make_shared<std::string>(message);
    boost::asio::async_write(
      socket,
      boost::asio::buffer(*send_buffer),
      [this, self, send_buffer, callback](const boost::system::error_code& ec, std::size_t bytes_sent) {
        callback(ec, bytes_sent);
      }
    );
  }

  void asyncReadUntil(char delim, AsyncReadCallback callback)
  {
    auto self(shared_from_this());
    auto read_buffer = std::make_shared<std::string>();
    boost::asio::async_read_until(
      socket,
      boost::asio::dynamic_buffer(*read_buffer), delim,  // Read until we find a newline
      [this, self, read_buffer, delim, callback](const boost::system::error_code& ec, std::size_t length) {
        if (ec) {
          Log::err() << "Read error: " << ec.message();
        } else if (length > 0) {
          callback(ec, std::string(*read_buffer, 0, length));
        }
        asyncReadUntil(delim, callback);
      }
    );
  }


 private:
  String host;
  String port;
  std::string data_;
  bool running;

  void connect()
  {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket, endpoints);
  }
};

}  // namespace ae