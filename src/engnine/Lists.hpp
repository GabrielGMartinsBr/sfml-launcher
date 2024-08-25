#pragma once

#include <algorithm>
#include <memory>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"
#include "engnine/Viewport.hpp"

namespace Eng {

using app::SPtr;
using app::UPtr;
using app::Vector;
using app::VectorPtr;
using std::make_unique;

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

  UPtr<Viewport> defaultViewport;
  Vector<Viewport*> viewports;
  Vector<OnUpdate*> updateList;
  Vector<OnRender*> renderList;

  /*
    Viewport List
  */

  void addViewport(Viewport* viewport)
  {
    viewports.push_back(viewport);
    Log::out() << "(add)viewports: " << viewports.size();
  }

  void removeViewport(Viewport* viewport)
  {
    auto it = std::find(viewports.begin(), viewports.end(), viewport);
    if (it != viewports.end()) {
      viewports.erase(it);
      Log::out() << "(remove)viewports: " << viewports.size();
    }
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


 private:
  Lists() :
      defaultViewport(nullptr) { }

  Lists(const Lists&);
  Lists& operator=(const Lists&);
};

}  // namespace Eng