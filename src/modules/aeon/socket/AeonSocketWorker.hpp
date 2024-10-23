#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <queue>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/socket/AeonSocketBase.hpp"

namespace ae {

using app::Thread;
using boost::asio::ip::tcp;

struct AeonSocketWorker {
  AeonSocketWorker() :
      running(false),
      socket(nullptr)
  {
  }

  void start()
  {
    running = true;
    thread_ = Thread(&AeonSocketWorker::run, this);
  }

  void stop()
  {
    {
      std::lock_guard<std::mutex> lock(queue_mutex);
      running = false;
    }
    condition.notify_all();
    if (thread_.joinable()) {
      thread_.join();
    }
    if (socket) {
      socket->disconnect();
      socket.reset();
    }
  }

  void enqueueAsyncTask(const std::function<void(std::shared_ptr<ae::AeonSocketBase>&)>& task)
  {
    {
      std::lock_guard<std::mutex> lock(queue_mutex);
      tasks.push([task, this] { task(socket); });  // Capture socket and task
    }
    condition.notify_one();  // Notify the worker thread
  }

 private:
  Thread thread_;
  bool running;
  // boost::asio::io_context io_context;
  std::shared_ptr<ae::AeonSocketBase> socket;
  std::queue<std::function<void()>> tasks;
  std::mutex queue_mutex;
  std::condition_variable condition;

  void run()
  {
    boost::asio::io_context io_context;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(io_context.get_executor());

    socket = std::make_shared<ae::AeonSocketBase>(io_context);

    std::thread io_thread([&]() {
      io_context.run();
    });

    while (running) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(queue_mutex);
        condition.wait(lock, [this] { return !tasks.empty() || !running; });  // Wait for tasks or stop

        if (!running && tasks.empty()) return;  // Exit if stopping and no tasks left

        task = std::move(tasks.front());
        tasks.pop();
      }

      if (task) {
        task();
      }
    }

    if (socket) {
      socket->disconnect();
      socket.reset();
    }

    Log::out() << "Stop";

    io_context.stop();
    if (io_thread.joinable()) {
      io_thread.join();
    }
  }
};

}  // namespace ae