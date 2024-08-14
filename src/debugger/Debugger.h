#pragma once

#include <ruby.h>

#include <boost/asio/io_context.hpp>
#include <memory>
#include <thread>

#include "AppDefs.h"
#include "Breakpoints.h"
#include "TcpServer.h"
#include "node.h"

namespace dbg {

using app::String;
using app::UInt;

struct Debugger {
  static Debugger& getInstance();

  std::unique_ptr<std::thread> serverThread;

  void start();

  void stop();

  bool isRunning();

  bool isAttached();

  void attach();

  void handleContinue();

  void handlePause();

  void handleStop();

  void sendIsPaused();

  void sendCurrentLine(UInt line);

  void sendDebugState(VALUE self, VALUE mid, VALUE classObj);

 private:
  std::unique_ptr<TcpServer> server;
  Breakpoints& breakpoints;
  boost::asio::io_context io_context;

  bool running;
  bool attached;
  bool isPaused;
  bool sentIsPaused;
  bool sentCurrentLine;
  bool shouldPause;
  bool shouldContinue;
  bool shouldStop;

  Debugger();

  Debugger(const Debugger&);
  Debugger& operator=(const Debugger&);

  void startServerThread();

  static void trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj);
};

}  // namespace dbg