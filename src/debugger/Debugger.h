#pragma once

#include <boost/asio/io_context.hpp>
#include <memory>
#include <thread>

#include "debugger/Breakpoints.h"

namespace dbg {

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
};

}  // namespace dbg