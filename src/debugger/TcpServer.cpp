#include "TcpServer.h"

#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "AppDefs.h"
#include "Log.hpp"
#include "StringUtils.hpp"
#include "debugger/Breakpoints.h"
#include "debugger/Debugger.h"
#include "debugger/ParentVarLocation.hpp"
#include "debugger/UnserializeUtils.hpp"

namespace dbg {

using namespace boost::asio::ip;
using json = nlohmann::json;
using app::String;
using app::StrStream;
using app::StrVector;
using app::Vector;
using std::to_string;

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ TcpServer ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

TcpServer::TcpServer(boost::asio::io_context& io_context, short port) :
    acceptor_(
      io_context,
      tcp::endpoint(tcp::v4(), port)
    )
{
  socket_ = std::make_shared<tcp::socket>(io_context);

  startAccept();
}

void TcpServer::startAccept()
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

void TcpServer::handleAccept(std::shared_ptr<tcp::socket> socket)
{
  if (connection != nullptr && connection->connected) {
    connection->close();
  }
  connection.reset();
  connection = std::make_shared<TcpConnection>(socket);
  connection->start();
}

void TcpServer::sendIsPaused(bool value)
{
  std::string msg = "isPaused:" + to_string(value) + '\n';
  connection->doWrite(msg);
}

void TcpServer::sendCurrentLine(UInt line)
{
  std::string msg = "currentLine:" + to_string(line) + '\n';
  connection->doWrite(msg);
}

void TcpServer::sendDebugState(const String& data)
{
  std::string msg = "debugState:" + StringUtils::lengthStr(data) + "|" + data + '\n';
  connection->doWrite(msg);
}

void TcpServer::sendDebugVariable(const String& data)
{
  std::string msg = "debugVariable:" + StringUtils::lengthStr(data) + "|" + data + '\n';
  connection->doWrite(msg);
}

void TcpServer::sendVariableChangedValue(const String& data)
{
  std::string msg = "valueChangedVariable:" + StringUtils::lengthStr(data) + "|" + data + '\n';
  connection->doWrite(msg);
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ TcpConnection ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

TcpConnection::TcpConnection(std::shared_ptr<tcp::socket> socket) :
    socket_(socket),
    breakpoints(Breakpoints::getInstance())
{
  connected = false;
}

void TcpConnection::start()
{
  connected = true;
  doRead();
}

void TcpConnection::close()
{
  socket_->close();
  connected = false;
}

void TcpConnection::doRead()
{
  auto self(shared_from_this());
  boost::asio::async_read_until(
    *socket_,
    boost::asio::dynamic_buffer(data_), '\n',
    boost::bind(&TcpConnection::handleRead, self, std::placeholders::_1, std::placeholders::_2)
  );
}

void TcpConnection::handleRead(const boost::system::error_code& error, size_t bytes_transferred)
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

void TcpConnection::doWrite(std::string str)
{
  auto self(shared_from_this());
  boost::asio::async_write(
    *socket_,
    boost::asio::buffer(str, str.size()),
    boost::bind(&TcpConnection::handleWrite, self, boost::asio::placeholders::error)
  );
}

void TcpConnection::handleWrite(const boost::system::error_code& error)
{
  if (error) {
    Log::err() << "Write tcp message error: " << error.message();
    Log::err() << error.what();
    Log::err() << error.value();
  }
}

std::pair<std::string, std::string> TcpConnection::splitMsg(const std::string& str, char delimiter)
{
  size_t pos = str.find(delimiter);
  if (pos == std::string::npos) {
    return { str, "" };
  }

  std::string first_part = str.substr(0, pos);
  if (pos + 2 >= str.size()) {
    return { first_part, "" };
  }

  std::string second_part = str.substr(pos + 1);

  return { first_part, second_part };
}

Vector<String> TcpConnection::split(const String& str, char delimiter)
{
  Vector<String> tokens;
  String token;
  StrStream stream(str);

  while (std::getline(stream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

void TcpConnection::handleReceivedMsg()
{
  if (data_.compare("continue\n") == 0) {
    handleContinue();
    return;
  }

  if (data_.compare("pause\n") == 0) {
    handlePause();
    return;
  }

  if (data_.compare("stepOver\n") == 0) {
    handleStepOver();
    return;
  }

  if (data_.compare("stop\n") == 0) {
    handleStop();
    return;
  }

  data_.pop_back();
  std::pair<std::string, std::string> msg = splitMsg(data_, ':');

  if (msg.first.compare("breakpoints") == 0) {
    handleBreakpointsMsg(msg.second);
    return;
  }

  if (msg.first.compare("addBreakpoint") == 0) {
    handleAddBreakpointMsg(msg.second);
    return;
  }

  if (msg.first.compare("removeBreakpoint") == 0) {
    handleRemoveBreakpointMsg(msg.second);
    return;
  }

  if (msg.first.compare("fetchVariable") == 0) {
    handleFetchVariableMsg(msg.second);
    return;
  }

  if (msg.first.compare("setVariableValue") == 0) {
    handleSetVariableValueMsg(msg.second);
    return;
  }
}

void TcpConnection::handleBreakpointsMsg(const std::string& msg)
{
  setBreakpoints(msg);
  setAttached();
}

void TcpConnection::handleAddBreakpointMsg(const std::string& msg)
{
  UInt line = static_cast<UInt>(std::stoul(msg));
  if (line > 0) {
    breakpoints.add(line);
  }
}

void TcpConnection::handleRemoveBreakpointMsg(const std::string& msg)
{
  UInt line = static_cast<UInt>(std::stoul(msg));
  if (line > 0) {
    breakpoints.remove(line);
  }
}

void TcpConnection::handleFetchVariableMsg(const std::string& msg)
{
  VALUE var = static_cast<VALUE>(std::stoul(msg));
  Debugger::getInstance().handleFetchVariable(var);
}

void TcpConnection::handleSetVariableValueMsg(const std::string& msg)
{
  json jsonObject = json::parse(msg);

  ParentVarLocation location = jsonObject["parentLocation"];
  VALUE parent = jsonObject["parentVar"];
  String name = jsonObject["name"];
  String type = jsonObject["type"];
  String value = jsonObject["value"];

  Debugger::getInstance().handleSetVariableValue(
    location,
    parent,
    name.c_str(),
    type.c_str(),
    value.c_str()
  );
}

void TcpConnection::handleContinue()
{
  Debugger::getInstance().handleContinue();
}

void TcpConnection::handlePause()
{
  Debugger::getInstance().handlePause();
}

void TcpConnection::handleStepOver()
{
  Debugger::getInstance().handleStepOver();
}

void TcpConnection::handleStop()
{
  Debugger::getInstance().handleStop();
}

void TcpConnection::setBreakpoints(const std::string& msg)
{
  std::vector<std::string> tokens = split(msg, ',');
  UInt line;
  breakpoints.clear();
  for (const std::string& lineStr : tokens) {
    line = static_cast<UInt>(std::stoul(lineStr));
    breakpoints.add(line);
  }
}

void TcpConnection::setAttached()
{
  Debugger::getInstance().attach();
}

}  // namespace dbg
