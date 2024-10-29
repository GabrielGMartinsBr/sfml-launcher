#pragma once

#include <memory>

#include "AppDefs.h"
#include "aeon/socket/integration/AeonSocketIntegrable.h"

namespace ae {

using app::UnMap;
using app::UPtr;

struct AeonSocketManager {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static AeonSocketManager& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  AeonSocketIntegrable* create(VALUE rbId)
  {
    UPtr<AeonSocketIntegrable> socketPtr = std::make_unique<AeonSocketIntegrable>(rbId);
    AeonSocketIntegrable* rawPtr = socketPtr.get();
    clients[rbId] = std::move(socketPtr);
    return rawPtr;
  }

  void destroy(AeonSocketIntegrable* socket)
  {
    if (socket) {
      socket->stopWorker();
      clients[socket->rbId].reset();
    }
  }

  void destroyAll()
  {
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      VALUE key = it->first;
      UPtr<AeonSocketIntegrable>& value = it->second;
      if (value != nullptr) {
        value->stopWorker();
      }
    }
    clients.clear();
  }

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  UnMap<VALUE, UPtr<AeonSocketIntegrable>> clients;

  AeonSocketManager();

  AeonSocketManager(const AeonSocketManager&);
  AeonSocketManager& operator=(const AeonSocketManager&);
};

}  // namespace ae