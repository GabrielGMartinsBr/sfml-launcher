#include "Debugger.h"

#include <boost/asio/io_context.hpp>
#include <memory>

#include "Log.hpp"
#include "TcpServer.hpp"
#include "debugger/Breakpoints.h"

namespace dbg {

constexpr int PORT = 3333;

Debugger& Debugger::getInstance()
{
  static Debugger instance;
  return instance;
}

Debugger::Debugger() :
    breakpoints(Breakpoints::getInstance())
{
  isRunning = false;
  Log::out() << "Debugger constructor()";
}

void Debugger::start()
{
  serverThread = std::make_unique<std::thread>(&Debugger::startServerThread, this);
  isRunning = true;
}

void Debugger::stop()
{
  io_context.stop();
}

void Debugger::startServerThread()
{
  try {
    TcpServer server(io_context, PORT);
    io_context.run();
  } catch (std::exception& e) {
    Log::err() << "Debugger server error: " << e.what();
  }
}

}  // namespace dbg