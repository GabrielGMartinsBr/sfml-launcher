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
using AsyncReadCallback = std::function<void(const boost::system::error_code&, const String&)>;

struct AeonSocket : public std::enable_shared_from_this<AeonSocket> {
  boost::asio::io_context& io_context;
  tcp::resolver resolver;
  tcp::socket socket;

  AeonSocket(boost::asio::io_context& io_context) :
      io_context(io_context),
      resolver(io_context),
      socket(io_context)
  {
    running = true;
  }

  ~AeonSocket()
  {
  }

  bool isConnected()
  {
    return socket.is_open();
  }

  void connectAsync(const String& _host, const String& _port, AsyncConnectCallback callback)
  {
    if (socket.is_open()) {
      disconnect();
      std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    host = _host;
    port = _port;
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    auto self(shared_from_this());
    boost::asio::async_connect(
      socket,
      endpoints,
      [callback, this](const boost::system::error_code& ec, const tcp::endpoint& endpoint) {
        if (ec) {
          socket.close();
        }
        callback(ec, endpoint);
      }
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

    boost::asio::async_read_until(
      socket,
      boost::asio::dynamic_buffer(inData),
      delim,  // Read until we find a newline
      [this, self, delim, callback](const boost::system::error_code& ec, std::size_t length) {
        if (ec) {
          callback(ec, String());
        } else {
          String message = inData.substr(0, length);
          inData.erase(0, length);
          callback(ec, std::move(message));
          asyncReadUntil(delim, callback);
        }
      }
    );
  }


 private:
  String host;
  String port;
  String inData;
  bool running;

  void connect()
  {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    boost::asio::connect(socket, endpoints);
  }
};

}  // namespace ae