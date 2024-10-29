#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include "engnine/IOnRender.h"
#include "engnine/Lists.hpp"

namespace Eng {

class EngineRenderEntity : public IOnRender {
 public:

  EngineRenderEntity()
  {
    addedToRenderList = false;
    addToRenderList();
  }

  ~EngineRenderEntity()
  {
    removeFromRenderList();
  }

  virtual void onRender(sf::RenderTexture& renderTexture) = 0;

  virtual bool shouldRender() const = 0;

  virtual int getZIndex() const = 0;

 protected:
  bool addedToRenderList;

  void addToRenderList()
  {
    if (addedToRenderList) return;
    Lists::Instance().addRenderEntry(this);
    addedToRenderList = true;
  }

  void removeFromRenderList()
  {
    if (!addedToRenderList) return;
    Lists::Instance().removeRenderEntry(this);
    addedToRenderList = false;
  }
};

}  // namespace Eng