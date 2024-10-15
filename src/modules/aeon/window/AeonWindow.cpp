#include "aeon/window/AeonWindow.h"

#include <SFML/Graphics/Color.hpp>

#include "aeon/toolkit/ColorParser.hpp"
#include "aeon/window/AeonWindowManager.h"
#include "engnine/Lists.hpp"
#include "engnine/Window.h"

namespace ae {

using Eng::Lists;

AeonWindow::AeonWindow(VALUE rbObj, Eng::Viewport* viewport) :
    Eng::Window(rbObj, viewport),
    hitBox(),
    ring(3),
    isRingVisible(false),
    timestamp(0)
{
  addedToEngineCycles = false;
  ring.fillColor("#0000");
  ring.borderColor("#fa0d");
  ring.borderSize(6);
  addToEngineCycles();
}

AeonWindow::~AeonWindow()
{
  removeFromEngineCycles();
}

/*
  ⇩⇩⇩ Aeon lifecycle methods ⇩⇩⇩
*/

void AeonWindow::handleAeonUpdate(UInt ts)
{
  timestamp = ts;
}

/*
  ⇩⇩⇩ Nrgss lifecycle methods ⇩⇩⇩
*/

void AeonWindow::onRender(sf::RenderTexture& renderTexture)
{
  ring.drawTo(renderTexture);
}

bool AeonWindow::shouldRender() const
{
  return !isDisposed && visible && isRingVisible;
}

int AeonWindow::getZIndex() const
{
  return z + 99;
}

/*
  ⇩⇩⇩ Self ⇩⇩⇩
*/

bool AeonWindow::intersects(float x, float y)
{
  return hitBox.intersects(x, y);
}

void AeonWindow::setRingVisibility(bool value)
{
  isRingVisible = value;
}

VALUE AeonWindow::setter_x(VALUE v)
{
  v = Window::setter_x(v);
  hitBox.updateX(x);
  ring.x(Window::x - 6);
  return v;
}

VALUE AeonWindow::setter_y(VALUE v)
{
  v = Window::setter_y(v);
  hitBox.updateY(y);
  ring.y(y - 6);
  return v;
}

VALUE AeonWindow::setter_width(VALUE v)
{
  v = Window::setter_width(v);
  hitBox.updateWidth(width);
  ring.width(width + 12);
  return v;
}

VALUE AeonWindow::setter_height(VALUE v)
{
  v = Window::setter_height(v);
  hitBox.updateHeight(height);
  ring.height(height + 12);
  return v;
}

void AeonWindow::method_dispose()
{
  Window::method_dispose();
  removeFromEngineCycles();
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonWindow::addToEngineCycles()
{
  if (addedToEngineCycles) return;
  Lists::Instance().addRenderEntry(this);
  AeonWindowManager::Instance().addEntry(this);
  addedToEngineCycles = true;
}

void AeonWindow::removeFromEngineCycles()
{
  if (!addedToEngineCycles) return;
  Lists::Instance().removeRenderEntry(this);
  AeonWindowManager::Instance().removeEntry(this);
  addedToEngineCycles = false;
}

}  // namespace ae