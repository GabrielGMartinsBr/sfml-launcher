#pragma once

#include <memory>

#include "AppDefs.h"
#include "aeon/socket/integration/AeonSocketIntegrable.h"

namespace ae {

using app::UPtr;
using app::Vector;

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
    clients.push_back(std::move(socketPtr));
    return rawPtr;
  }

  void destroy(AeonSocketIntegrable* socket)
  {
    auto it = std::remove_if(
      clients.begin(),
      clients.end(),
      [socket](UPtr<AeonSocketIntegrable>& ptr) {
        return ptr.get() == socket;
      }
    );
    if (it != clients.end()) {
      socket->stopWorker();
      clients.erase(it, clients.end());
    }
  }

  void destroyAll()
  {
    for (UPtr<AeonSocketIntegrable>& socket : clients) {
      socket->stopWorker();
    }
    clients.clear();
  }

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  Vector<UPtr<AeonSocketIntegrable>> clients;

  AeonSocketManager();

  AeonSocketManager(const AeonSocketManager&);
  AeonSocketManager& operator=(const AeonSocketManager&);
};

}  // namespace ae