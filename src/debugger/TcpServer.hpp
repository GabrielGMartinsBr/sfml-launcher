#pragma once

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <string>

#include "Log.hpp"
#include "debugger/Breakpoints.h"
#include "debugger/Debugger.h"

namespace dbg {

using namespace boost::asio::ip;

struct TcpServer {
  tcp::acceptor acceptor_;
  std::shared_ptr<tcp::socket> socket_;

  TcpServer(boost::asio::io_context& io_context, short port) :
      acceptor_(
        io_context,
        tcp::endpoint(tcp::v4(), port)
      )
  {
    socket_ = std::make_shared<tcp::socket>(io_context);

    Log::out() << "socket_->is_open(): " << socket_->is_open();

    startAccept();
  }

  void startAccept()
  {
    acceptor_.async_accept(
      *socket_,
      [this](const boost::system::error_code& error) {
        if (!error) {
          handleAccept(socket_);
        } else {
          startAccept();
        }
      }
    );
  }

  void handleAccept(std::shared_ptr<tcp::socket> socket)
  {
    if (connection->connected) {
      connection->close();
    }
    connection.reset();
    connection = std::make_shared<TcpConnection>(socket);
    connection->start();
  }

  void sendCurrentLine(UInt line)
  {
    std::string msg = "currentLine:" + std::to_string(line);
    connection->doWrite(msg);
  }

  struct TcpConnection : public std::enable_shared_from_this<TcpConnection> {
    bool connected;

    TcpConnection(std::shared_ptr<tcp::socket> socket) :
        socket_(socket),
        breakpoints(Breakpoints::getInstance())
    {
      connected = false;
    }

    void start()
    {
      connected = true;
      Log::out() << "connected";
      doRead();
    }

    void close()
    {
      socket_->close();
      connected = false;
    }

    void doRead()
    {
      auto self(shared_from_this());
      boost::asio::async_read_until(
        *socket_,
        boost::asio::dynamic_buffer(data_), '\n',
        boost::bind(&TcpConnection::handleRead, self, std::placeholders::_1, std::placeholders::_2)
      );
    }

    void handleRead(const boost::system::error_code& error, size_t bytes_transferred)
    {
      if (!error) {
        handleReceivedMsg();
        data_.erase(0, data_.length());
        doRead();
      } else {
        Log::err() << "Failed to read msg!";
        Log::err() << error.message();
      }
    }

    void doWrite(std::string str)
    {
      auto self(shared_from_this());
      boost::asio::async_write(
        *socket_,
        boost::asio::buffer(str),
        boost::bind(&TcpConnection::handleWrite, self, boost::asio::placeholders::error)
      );
    }

    void doWrite(size_t bytes_transferred)
    {
      auto self(shared_from_this());
      boost::asio::async_write(
        *socket_,
        boost::asio::buffer(data_, bytes_transferred),
        boost::bind(&TcpConnection::handleWrite, self, boost::asio::placeholders::error)
      );
    }

    void handleWrite(const boost::system::error_code& error)
    {
      if (!error) {
        doRead();
      }
    }

   private:

    std::shared_ptr<tcp::socket> socket_;
    std::string data_;
    Breakpoints& breakpoints;

    std::pair<std::string, std::string> splitMsg(const std::string& str, char delimiter)
    {
      size_t pos = str.find(delimiter);
      if (pos == std::string::npos) {
        return { str, "" };
      }

      std::string first_part = str.substr(0, pos);
      std::string second_part = str.substr(pos + 1);

      return { first_part, second_part };
    }

    std::vector<std::string> split(const std::string& str, char delimiter)
    {
      std::vector<std::string> tokens;
      std::string token;
      std::stringstream ss(str);

      while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
      }

      return tokens;
    }

    void handleReceivedMsg()
    {
      if (data_.compare("continue\n") == 0) {
        handleContinue();
        return;
      }

      std::pair<std::string, std::string> msg = splitMsg(data_, ':');

      if (msg.first.compare("breakpoints") == 0) {
        handleBreakpointsMsg(msg.second);
        return;
      }
    }

    void handleBreakpointsMsg(const std::string& msg)
    {
      setBreakpoints(msg);
      setAttached();
    }

    void handleContinue()
    {
      Debugger::getInstance().handleContinue();
    }

    void setBreakpoints(const std::string& msg)
    {
      std::vector<std::string> tokens = split(msg, ',');
      UInt line;
      breakpoints.clear();
      for (const std::string& lineStr : tokens) {
        line = static_cast<UInt>(std::stoul(lineStr));
        breakpoints.add(line);
      }
      Log::out() << "breakpoints set!";
    }

    void setAttached()
    {
      Debugger::getInstance().attach();
      Log::out() << "debugger attached!";
    }
  };

  std::shared_ptr<TcpConnection> connection;
};

}  // namespace dbg
