#pragma once

#include <algorithm>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/IOnRender.h"
#include "engnine/IOnUpdate.h"
#include "engnine/IViewportChild.h"
#include "engnine/Viewport.hpp"

namespace Eng {

using app::Vector;

struct Lists {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Lists& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  Vector<Viewport*> viewports;
  Vector<IOnUpdate*> updateList;
  Vector<IOnRender*> renderList;

  /*
    Viewport List
  */

  void addViewport(Viewport* viewport)
  {
    viewports.push_back(viewport);
    renderList.push_back(viewport);
  }

  void removeViewport(Viewport* viewport)
  {
    removeFromViewportList(viewport);
    removeRenderEntry(viewport);
  }

  /*
    Update List
  */

  void addUpdateEntry(IOnUpdate* entry)
  {
    updateList.push_back(entry);
  }

  void removeUpdateEntry(IOnUpdate* entry)
  {
    auto it = std::find(updateList.begin(), updateList.end(), entry);
    if (it != updateList.end()) {
      updateList.erase(it);
    }
  }

  /*
    Render List
  */

  void addRenderEntry(IViewportChild* instance)
  {
    Viewport* viewport = instance->getViewport();
    if (viewport == nullptr) {
      addRenderEntry(static_cast<IOnRender*>(instance));
    } else {
      viewport->addChild(instance);
    }
  }

  void removeRenderEntry(IViewportChild* instance)
  {
    Viewport* viewport = instance->getViewport();
    if (viewport == nullptr) {
      removeRenderEntry(static_cast<IOnRender*>(instance));
    } else {
      viewport->removeChild(instance);
    }
  }

  void addRenderEntry(IOnRender* instance)
  {
    renderList.push_back(instance);
  }

  void removeRenderEntry(IOnRender* instance)
  {
    auto it = std::find(renderList.begin(), renderList.end(), instance);
    if (it != renderList.end()) {
      renderList.erase(it);
    }
  }

  void markZOrderDirty();

  void sortZ();

 private:
  bool zDirty;

  Lists();

  Lists(const Lists&);
  Lists& operator=(const Lists&);

  void removeFromViewportList(Viewport* viewport)
  {
    auto it = std::find(viewports.begin(), viewports.end(), viewport);
    if (it != viewports.end()) {
      viewports.erase(it);
    }
  }

  static bool compareZ(const IOnRender* a, const IOnRender* b);
};

}  // namespace Eng