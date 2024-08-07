#pragma once

#include <boost/asio/io_context.hpp>
#include <memory>
#include <thread>

namespace dbg {

struct Debugger {
  static Debugger& getInstance();

  bool isRunning;
  std::unique_ptr<std::thread> serverThread;

  void start();

  void stop();

 private:
  boost::asio::io_context io_context;

  Debugger();

  Debugger(const Debugger&);
  Debugger& operator=(const Debugger&);

  void startServerThread();
};

}  // namespace dbg