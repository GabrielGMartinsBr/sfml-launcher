#pragma once

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <string>

#include "AppDefs.h"
#include "Breakpoints.h"

namespace dbg {

using namespace boost::asio::ip;
using app::String;

struct TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  bool connected;

  TcpConnection(std::shared_ptr<tcp::socket> socket);

  void start();

  void close();

  void doRead();

  void handleRead(const boost::system::error_code& error, size_t bytes_transferred);

  void doWrite(std::string str);

  void handleWrite(const boost::system::error_code& error);

 private:
  Breakpoints& breakpoints;

  std::shared_ptr<tcp::socket> socket_;
  std::shared_ptr<TcpConnection> connection;
  std::string data_;

  void handleReceivedMsg();

  void handleBreakpointsMsg(const std::string& msg);

  void handleAddBreakpointMsg(const std::string& msg);

  void handleRemoveBreakpointMsg(const std::string& msg);

  void handleContinue();

  void handlePause();

  void handleStop();

  void setBreakpoints(const std::string& msg);

  void setAttached();

  std::pair<std::string, std::string> splitMsg(const std::string& str, char delimiter);
  std::vector<std::string> split(const std::string& str, char delimiter);
};

struct TcpServer {
  std::shared_ptr<TcpConnection> connection;
  std::shared_ptr<tcp::socket> socket_;

  tcp::acceptor acceptor_;

  TcpServer(boost::asio::io_context& io_context, short port);

  void startAccept();

  void handleAccept(std::shared_ptr<tcp::socket> socket);

  void sendIsPaused(bool value);

  void sendCurrentLine(UInt line);

  void sendDebugState(String& data);
};

}  // namespace dbg
