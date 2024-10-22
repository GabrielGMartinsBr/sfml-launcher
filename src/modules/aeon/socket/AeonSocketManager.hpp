#pragma once

#include <memory>

#include "AppDefs.h"
#include "Log.hpp"
#include "aeon/socket/AeonSocket.hpp"

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

  AeonSocket* create()
  {
    UPtr<AeonSocket> socketPtr = std::make_unique<AeonSocket>();
    AeonSocket* rawPtr = socketPtr.get();
    clients.push_back(std::move(socketPtr));
    return rawPtr;
  }

  void destroy(AeonSocket* socket)
  {
    auto it = std::remove_if(
      clients.begin(),
      clients.end(),
      [socket](UPtr<AeonSocket>& ptr) {
        return ptr.get() == socket;
      }
    );
    if (it != clients.end()) {
      clients.erase(it, clients.end());
    }
  }

  void destroyAll()
  {
    for (UPtr<AeonSocket>& socket : clients) {
      socket->close();
    }
    clients.clear();
  }

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  Vector<UPtr<AeonSocket>> clients;

  AeonSocketManager();

  AeonSocketManager(const AeonSocketManager&);
  AeonSocketManager& operator=(const AeonSocketManager&);
};

}  // namespace ae