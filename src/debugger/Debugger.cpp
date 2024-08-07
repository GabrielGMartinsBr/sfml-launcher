#include "Debugger.h"

#include <boost/asio/io_context.hpp>

#include "Log.hpp"
#include "TcpServer.hpp"
#include "debugger/Breakpoints.h"

namespace dbg {

constexpr int PORT = 3333;

#define rb_sourceFile() (ruby_current_node ? ruby_current_node->nd_file : 0)
#define rb_sourceLine() (ruby_current_node ? nd_line(ruby_current_node) : 0)

Debugger& Debugger::getInstance()
{
  static Debugger instance;
  return instance;
}

Debugger::Debugger() :
    breakpoints(Breakpoints::getInstance())
{
  isRunning = false;
}

void Debugger::start()
{
  serverThread = std::make_unique<std::thread>(&Debugger::startServerThread, this);
  rb_add_event_hook(trace_function, RUBY_EVENT_LINE);

  breakpoints.add(1);
  breakpoints.add(19);

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

void Debugger::trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj)
{
  static Breakpoints& breakpoints = Breakpoints::getInstance();

  if (breakpoints.isEmpty()) {
    return;
  }

  auto fileName = rb_sourceFile();
  if (strcmp(fileName, "(eval)") != 0) {
    // Ignore events if line is not from scripts code
    return;
  }

  UInt currLine = rb_sourceLine() + 1;

  if (breakpoints.contains(currLine)) {
    Log::out() << "stop at: " << fileName << "\n";

    std::cin.get();
  }
}

}  // namespace dbg