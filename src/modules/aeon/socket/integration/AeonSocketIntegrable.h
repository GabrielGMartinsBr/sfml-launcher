#pragma once

#include "AppDefs.h"
#include "aeon/base/AeonIntegrable.h"
#include "aeon/socket/AeonSocketWorker.h"
#include "engnine/IOnUpdate.h"

namespace ae {

using app::Queue;
using app::String;

struct AeonSocketIntegrable : public AeonSocketWorker, public AeonIntegrable, public Eng::IOnUpdate {
  AeonSocketIntegrable(VALUE rbId);
  ~AeonSocketIntegrable();

  void onUpdate() override;

  void connect(const String& host, const String& port, VALUE yieldBlock = Qnil);

  void sendMessage(const String& message);
  void readMessagesByDelimiter(char delim);

  void close();
  void shutdown();
  void disconnect();

  bool isConnected();

  void setConnectHandler(VALUE handler);
  void setMessageHandler(VALUE handler);
  void setConnectYieldBlock(VALUE block);

 private:
  VALUE connectHandler;
  VALUE messageHandler;
  VALUE connectYieldBlock;
  VALUE connectHandlerResult;
  std::deque<String> messageQueue;

  bool addedToEngineCycles;

  void handleConnectResult(bool success);
  void handleMessage(const String& message);

  void addToEngineCycles();
  void removeFromEngineCycles();

  void addMessage(const String& message);
  void processMessages();
};

}  // namespace ae