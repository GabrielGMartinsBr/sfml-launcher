#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <queue>

#include "AppDefs.h"
#include "aeon/socket/AeonSocket.hpp"

namespace ae {

using app::CStr;
using app::SPtr;
using app::String;
using app::Thread;
using boost::asio::ip::tcp;
using BoostIoContext = boost::asio::io_context;

struct AeonSocketWorker {
  AeonSocketWorker();

  void startWorker();
  void stopWorker();
  void enqueueAsyncTask(const std::function<void(SPtr<ae::AeonSocket>&)>& task);

 protected:
  Thread thread_;
  BoostIoContext io_context;
  SPtr<ae::AeonSocket> socket;
  std::queue<std::function<void()>> tasks;
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool running;

  void run();
};

}  // namespace ae