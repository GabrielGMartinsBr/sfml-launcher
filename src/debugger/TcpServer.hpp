#pragma once

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <memory>

#include "Log.hpp"
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
        }
        startAccept();
      }
    );
  }

  void handleAccept(std::shared_ptr<tcp::socket> socket)
  {
    auto connection = std::make_shared<TcpConnection>(socket);
    connection->start();
  }

  struct TcpConnection : public std::enable_shared_from_this<TcpConnection> {
    TcpConnection(std::shared_ptr<tcp::socket> socket) :
        socket_(socket) { }

    void start()
    {
      Log::out() << "connected";
      Debugger::getInstance().attach();
      doRead();
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
        // doWrite(bytes_transferred);
        // Log::out() << bytes_transferred;
        Log::out() << data_;
        data_.erase(0, data_.length());
        doRead();
      }
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

    std::shared_ptr<tcp::socket> socket_;
    std::string data_;
  };
};

}  // namespace dbg
