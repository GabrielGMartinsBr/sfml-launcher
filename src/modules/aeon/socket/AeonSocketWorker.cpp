#include "aeon/socket/AeonSocketWorker.h"

namespace ae {

AeonSocketWorker::AeonSocketWorker() :
    running(false),
    socket(nullptr)
{
}

/*
    Start worker thread
*/
void AeonSocketWorker::startWorker()
{
  running = true;
  thread_ = Thread(&AeonSocketWorker::run, this);
}

/*
    Stop worker thread
*/
void AeonSocketWorker::stopWorker()
{
  if (!running) return;
  {
    std::lock_guard<std::mutex> lock(queue_mutex);
    running = false;
    io_context.stop();
  }
  condition.notify_all();
  if (thread_.joinable()) {
    thread_.join();
  }
  if (socket) {
    if (socket->socket.is_open()) {
      socket->disconnect();
    }
    socket.reset();
  }
}

/*
    Add task to queue
*/
void AeonSocketWorker::enqueueAsyncTask(const std::function<void(SPtr<AeonSocket> &)> &task)
{
  {
    std::lock_guard<std::mutex> lock(queue_mutex);
    tasks.push([task, this] { task(socket); });  // Capture socket and task
  }
  condition.notify_one();  // Notify the worker thread
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonSocketWorker::run()
{
  boost::asio::executor_work_guard<BoostIoContext::executor_type> work_guard(io_context.get_executor());

  socket = std::make_shared<AeonSocket>(io_context);

  std::thread io_thread([&]() {
    io_context.run();
  });

  while (running) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      condition.wait(lock, [this] { return !tasks.empty() || !running; });  // Wait for tasks or stop

      if (!running) break;
      if (tasks.empty()) continue;

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

  io_context.stop();
  if (io_thread.joinable()) {
    io_thread.join();
  }
}

}  // namespace ae