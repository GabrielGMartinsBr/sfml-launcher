#pragma once

#include <ruby.h>

#include <boost/asio/io_context.hpp>
#include <memory>
#include <thread>

#include "AppDefs.h"
#include "Breakpoints.h"
#include "ParentVarLocation.hpp"
#include "TcpServer.h"
#include "debugger/DebugVariableScope.h"
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

  void handleFetchVariable(VALUE var);

  void handleSetVariableValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr value);

  void sendIsPaused();

  void sendCurrentLine(UInt line);

  void sendDebugState(VALUE self, VALUE mid, VALUE classObj);

  void sendDebugVariable(String& data);

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

  static void trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj);

  void startServerThread();

  VALUE setVariableValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr value);

  void emitVariableChangedValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr value);
};

}  // namespace dbg