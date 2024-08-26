#pragma once

#include <algorithm>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/IViewportChild.h"
#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"
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
  Vector<OnUpdate*> updateList;
  Vector<OnRender*> renderList;

  /*
    Viewport List
  */

  void addViewport(Viewport* viewport)
  {
    viewports.push_back(viewport);
    renderList.push_back(viewport);
    Log::out() << "(add)viewports: " << viewports.size();
  }

  void removeViewport(Viewport* viewport)
  {
    removeFromViewportList(viewport);
    removeRenderEntry(viewport);
    Log::out() << "(remove)viewports: " << viewports.size();
  }

  /*
    Update List
  */

  void addUpdateEntry(OnUpdate* entry)
  {
    updateList.push_back(entry);
  }

  void removeUpdateEntry(OnUpdate* entry)
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
      addRenderEntry(static_cast<OnRender*>(instance));
    } else {
      viewport->addChild(instance);
    }
  }

  void removeRenderEntry(IViewportChild* instance)
  {
    Viewport* viewport = instance->getViewport();
    if (viewport == nullptr) {
      removeRenderEntry(static_cast<OnRender*>(instance));
    } else {
      viewport->removeChild(instance);
    }
  }

  void addRenderEntry(OnRender* instance)
  {
    renderList.push_back(instance);
  }

  void removeRenderEntry(OnRender* instance)
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

  static bool compareZ(const OnRender* a, const OnRender* b);
};

}  // namespace Eng