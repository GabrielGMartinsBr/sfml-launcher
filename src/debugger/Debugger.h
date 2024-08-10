#pragma once

#include <ruby.h>

#include <boost/asio/io_context.hpp>
#include <memory>
#include <thread>

#include "AppDefs.h"
#include "debugger/Breakpoints.h"
#include "node.h"

namespace dbg {

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

  void sendCurrentLine(UInt line);

 private:

  Breakpoints& breakpoints;
  boost::asio::io_context io_context;

  bool running;
  bool attached;
  bool sentCurrentLine;
  bool shouldContinue;

  Debugger();

  Debugger(const Debugger&);
  Debugger& operator=(const Debugger&);

  void startServerThread();

  static void trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj);
};

}  // namespace dbg