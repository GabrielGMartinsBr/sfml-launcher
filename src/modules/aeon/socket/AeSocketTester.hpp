#pragma once

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <regex>
#include <string>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/socket/AeonSocketWorker.hpp"

struct AeSockTester {
  ae::AeonSocketWorker socketWorker;

  void run()
  {
    Log::out() << "starting...";

    socketWorker.start();

    connect("127.0.0.1", "50001");

    // disconnect();

    // connect("127.0.0.1", "50000");
  }

  void handleConnected()
  {
    readMessagesByDelimiter('\n');
    sendMessage("<mod>'request'</mod>\n");
    sendMessage("<0>'e'</0>\n");
    sendMessage("<login jack>jack</login>\n");
    sendMessage("<1>'req'</1>\n");
    sendMessage("<2>'req'</2>\n");
  }

  void handleTestDisconnect(std::string msg)
  {
    std::regex pattern("^<2>");
    if (std::regex_search(msg, pattern)) {
    //   Log::out() << "Foi!";
      //   disconnect();
      //   connect("127.0.0.1", "50000");
    }
  }

  void stop()
  {
    socketWorker.stop();
  }

  void connect(app::CStr host, app::CStr port)
  {
    using boost::asio::ip::tcp;

    socketWorker.enqueueAsyncTask([this, host, port](std::shared_ptr<ae::AeonSocketBase> &socket) {
      if (socket && socket->isConnected()) {
        Log::err() << "Socket is already connected.";
        return;
      }
      socket->connectAsync(
        host,
        port,
        [this](const boost::system::error_code &ec, const tcp::endpoint &endpoint) {
          if (!ec) {
            Log::out() << "Connected successfully!";
            this->handleConnected();
          } else {
            Log::err() << "Error during connect: " << ec.message();
          }
        }
      );
    });
  }

  void readMessagesByDelimiter(char delim)
  {
    socketWorker.enqueueAsyncTask([delim, this](std::shared_ptr<ae::AeonSocketBase> &socket) {
      if (socket) {
        socket->asyncReadUntil(
          delim,
          [this](const boost::system::error_code &ec, std::string msg) {
            if (ec) {
              std::cerr << "Error reading message: " << ec.message() << std::endl;
            } else {
              Log::out() << "receive: " << msg;
              this->handleTestDisconnect(msg);
            }
          }
        );
      }
    });
  }

  void sendMessage(const std::string message)
  {
    socketWorker.enqueueAsyncTask([message](std::shared_ptr<ae::AeonSocketBase> &socket) {
      if (socket) {
        socket->asyncSend(message, [](const boost::system::error_code &ec, std::size_t) {
          if (ec) {
            std::cerr << "Error sending message: " << ec.message() << std::endl;
          } else {
            // std::cout << "Message sent successfully." << std::endl;
          }
        });
      }
    });
  }

  void disconnect()
  {
    socketWorker.enqueueAsyncTask([](std::shared_ptr<ae::AeonSocketBase> &socket) {
      socket->disconnect();
    });
  }
};