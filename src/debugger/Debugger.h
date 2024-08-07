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

  bool isRunning;
  std::unique_ptr<std::thread> serverThread;

  void start();

  void stop();

 private:
  Breakpoints& breakpoints;
  boost::asio::io_context io_context;

  Debugger();

  Debugger(const Debugger&);
  Debugger& operator=(const Debugger&);

  void startServerThread();

  static void trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj);
};

}  // namespace dbg