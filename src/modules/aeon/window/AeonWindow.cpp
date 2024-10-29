#include "aeon/window/AeonWindow.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "aeon/enums/AeonElementState.h"
#include "aeon/enums/AeonElementType.h"
#include "aeon/toolkit/ColorParser.hpp"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonElement.h"
#include "aeon/window/AeonTextBoxElement.h"
#include "aeon/window/AeonWindowManager.h"
#include "engnine/Engine.h"
#include "engnine/Lists.hpp"
#include "engnine/Window.h"
#include "integrator/Convert.hpp"

namespace ae {

using Eng::Engine;
using Eng::Lists;
using sf::Vector2i;

AeonWindow::AeonWindow(VALUE rbObj, Eng::Viewport* viewport) :
    Eng::Window(rbObj, viewport),
    textBoxView(AeonWindowManager::Instance().getTextBoxView()),
    focusedElement(nullptr),
    clickedElement(nullptr),
    focusedElementIndex(-1),
    ring(3),
    isRingVisible(false),
    isHover(false),
    isFocused(false),
    lockedFocus(false)
{
  addedToEngineCycles = false;
  ring.fillColor("#0000");
  ring.borderColor("#fa0c");
  ring.borderSize(4);
  addToEngineCycles();
}

AeonWindow::~AeonWindow()
{
  removeFromEngineCycles();
}

/*
  ⇩⇩⇩ Aeon lifecycle methods ⇩⇩⇩
*/

void AeonWindow::handleAeonUpdate(ULong ts)
{
  ULong timestamp = ts;
  for (AeonElement* element : elements) {
    element->handleAeonUpdate(ts);
  }
  if (triggeredElement && !triggeredElement->isTriggered()) {
    setTriggeredElement(nullptr);
  }
}

void AeonWindow::handleMouseMoved(const AeMouseMoveEvent& event)
{
  float x = bounds.x();
  float y = bounds.y();
  float evX = event.x - x - 4;
  float evY = event.y - y - 4;
  for (AeonElement* element : elements) {
    if (element->intersects(evX, evY)) {
      setHoverElement(element);
      return;
    }
  }
  setHoverElement(nullptr);
}

void AeonWindow::handleMousePressed(const AeMouseButtonEvent& event)
{
  float x = bounds.x();
  float y = bounds.y();
  float evX = event.x - x - 4;
  float evY = event.y - y - 4;
  for (AeonElement* element : elements) {
    if (element->intersects(evX, evY)) {
      setFocusedElement(element);
      setClickElement(element);
      return;
    }
  }
  setFocusedElement(nullptr);
  setClickElement(nullptr);
}

void AeonWindow::handleMouseReleased(const AeMouseButtonEvent& event)
{
  if (clickedElement) {
    setClickElement(nullptr);
  }
}

void AeonWindow::handleKeyPressed(const AeKeyEvent& event)
{
  if (event.code == sf::Keyboard::Tab) {
    if (event.shift) {
      focusPrevious();
    } else {
      focusNext();
    }
  } else if (event.code == sf::Keyboard::Up) {
    focusPrevious();
  } else if (event.code == sf::Keyboard::Down) {
    focusNext();
  } else if (event.code == sf::Keyboard::Enter) {
    setClickElement(focusedElement);
    return;
  }

  if (!focusedElement) {
    return;
  }

  if (focusedElement->getType() == AeonElementType::TEXT_BOX) {
    AeonTextBoxElement* textBox = static_cast<AeonTextBoxElement*>(focusedElement);

    if (event.code == sf::Keyboard::Left && textBox->isCursorOnBegin()) {
      focusPrevious();
      return;
    } else if (event.code == sf::Keyboard::Right && textBox->isCursorOnEnd()) {
      focusNext();
      return;
    }

    textBox->handleKeyPressed(event);
    return;
  }

  if (focusedElement->getType() == AeonElementType::BUTTON) {
    if (event.code == sf::Keyboard::Left) {
      focusPrevious();
    } else if (event.code == sf::Keyboard::Right) {
      focusNext();
    }
  }
}

void AeonWindow::handleTextEntered(const AeTextEvent& event)
{
  if (!focusedElement || focusedElement->getType() != AeonElementType::TEXT_BOX) {
    return;
  }
  AeonTextBoxElement* textBox = static_cast<AeonTextBoxElement*>(focusedElement);
  textBox->handleTextEntered(event);
}

/*

*/

void AeonWindow::setIsHover(bool value)
{
  isHover = value;
}

void AeonWindow::setIsFocused(bool value)
{
  isFocused = value;
  // isRingVisible = value;
  if (!isFocused) {
    setFocusedElement(nullptr);
  }
}

bool AeonWindow::focus()
{
  return isFocused;
}

void AeonWindow::focus(bool value)
{
  if (value) {
    AeonWindowManager::Instance().setFocusOn(this);
  } else if (isFocused) {
    AeonWindowManager::Instance().setFocusOn(nullptr);
  }
}

bool AeonWindow::lockFocus()
{
  return lockedFocus;
}

void AeonWindow::lockFocus(bool value)
{
  lockedFocus = value;
}

/*
  ⇩⇩⇩ Nrgss lifecycle methods ⇩⇩⇩
*/

void AeonWindow::onRender(sf::RenderTexture& renderTexture)
{
  // if (isRingVisible) {
  //   ring.drawTo(renderTexture);
  // }
  renderTexture.setView(windowView);
  drawElements(renderTexture);
  renderTexture.setView(renderTexture.getDefaultView());
}

bool AeonWindow::shouldRender() const
{
  return !isDisposed && visible;
}

int AeonWindow::getZIndex() const
{
  return z + 2;
}

/*
  ⇩⇩⇩ Self ⇩⇩⇩
*/

bool AeonWindow::intersects(float x, float y)
{
  return bounds.intersects(x, y);
}

void AeonWindow::setRingVisibility(bool value)
{
  isRingVisible = value;
}

VALUE AeonWindow::setter_x(VALUE v)
{
  float x = Convert::toCDouble2(v);
  Window::setX(x);
  ring.x(x - 6);
  updateContentPosition();
  return v;
}

VALUE AeonWindow::setter_y(VALUE v)
{
  float y = Convert::toCDouble2(v);
  Window::setY(y);
  ring.y(y - 6);
  updateContentPosition();
  return v;
}

VALUE AeonWindow::setter_width(VALUE v)
{
  float value = Convert::toCDouble2(v);
  Window::setWidth(value);
  ring.width(value + 12);
  updateContentDimension();
  return v;
}

VALUE AeonWindow::setter_height(VALUE v)
{
  float value = Convert::toCDouble2(v);
  Window::setHeight(value);
  ring.height(value + 12);
  updateContentDimension();
  return v;
}

void AeonWindow::method_dispose()
{
  Window::dispose();
  removeFromEngineCycles();
}

/*
  ⇩⇩⇩ Window Elements Methods ⇩⇩⇩
*/

void AeonWindow::addElement(AeonElement* element)
{
  elements.push_back(element);
  if (element->getType() == AeonElementType::TEXT_BOX) {
    AeonTextBoxElement* textBox = static_cast<AeonTextBoxElement*>(element);
  }
}

void AeonWindow::drawElements(sf::RenderTarget& target)
{
  if (bounds.isEmpty()) return;

  for (AeonElement* element : elements) {
    if (!element) continue;
    element->drawShapesTo(target);
  }

  for (AeonElement* element : elements) {
    if (!element || element->getType() != AeonElementType::TEXT_BOX) {
      continue;
    }
    updateTextBoxViewBounds(*static_cast<AeonTextBoxElement*>(element));
    target.setView(textBoxView);
    element->drawContentsTo(target);
  }
  target.setView(windowView);
}

CStr AeonWindow::getHoverElementKey()
{
  return hoveredElement ? hoveredElement->getKey().c_str() : "";
}

CStr AeonWindow::getFocusElementKey()
{
  return focusedElement ? focusedElement->getKey().c_str() : "";
}

CStr AeonWindow::getClickElementKey()
{
  return clickedElement ? clickedElement->getKey().c_str() : "";
}

CStr AeonWindow::getTriggerElementKey()
{
  return triggeredElement ? triggeredElement->getKey().c_str() : "";
}

void AeonWindow::focusNext()
{
  focusedElementIndex = (focusedElementIndex + 1) % elements.size();
  setFocusedElement(elements[focusedElementIndex]);
}

void AeonWindow::focusPrevious()
{
  if (focusedElementIndex <= 0) {
    focusedElementIndex = elements.size() - 1;
  } else {
    focusedElementIndex -= 1;
  }
  setFocusedElement(elements[focusedElementIndex]);
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

void AeonWindow::updateContentPosition()
{
  if (!bounds.isEmpty()) {
    updateViewBounds();
  }
}

void AeonWindow::updateContentDimension()
{
  if (!bounds.isEmpty()) {
    updateViewBounds();
  }
}

void AeonWindow::updateViewBounds()
{
  const Vector2i& dimensions = Engine::getInstance().getDimensions();
  ElementBounds vb = bounds + ElementBounds{ 4, 4, -8, -8 };

  windowView.setSize(vb.size());
  windowView.setCenter(vb.width() / 2, vb.height() / 2);
  windowView.setViewport(sf::FloatRect(
    vb.x() / dimensions.x,
    vb.y() / dimensions.y,
    vb.width() / dimensions.x,
    vb.height() / dimensions.y
  ));
}

void AeonWindow::updateTextBoxViewBounds(const AeonTextBoxElement& element)
{
  const sf::Vector2i& dimensions = Engine::getInstance().getDimensions();
  float border = element.getStyle().borderSize.value_or(0);
  float totalBorder = border * 2;
  ElementBounds vb = element.getBounds() + ElementBounds{ border, border, -totalBorder, -totalBorder };
  vb.x(bounds.x() + vb.x() + 4);
  vb.y(bounds.y() + vb.y() + 4);

  textBoxView.setSize(vb.size());
  textBoxView.setCenter(vb.width() / 2, vb.height() / 2);
  textBoxView.setViewport(sf::FloatRect(
    vb.x() / dimensions.x,
    vb.y() / dimensions.y,
    vb.width() / dimensions.x,
    vb.height() / dimensions.y
  ));
}

int AeonWindow::getElementIndex(AeonElement* focusedElement) const
{
  auto it = std::find(elements.begin(), elements.end(), focusedElement);
  if (it != elements.end()) {
    size_t index = std::distance(elements.begin(), it);  // Get the index
    return index;
  }
  return -1;
}

void AeonWindow::setHoverElement(AeonElement* element)
{
  if (hoveredElement == element) return;
  if (hoveredElement != nullptr) {
    hoveredElement->removeState(AeonElementState::HOVER);
  }
  hoveredElement = element;
  if (hoveredElement) {
    hoveredElement->addState(AeonElementState::HOVER);
  }
}

void AeonWindow::setFocusedElement(AeonElement* element)
{
  if (focusedElement == element) return;
  if (focusedElement) {
    focusedElement->setFocus(false);
  }
  if (element) {
    focusedElement = element;
    focusedElementIndex = getElementIndex(focusedElement);
    focusedElement->setFocus(true);
  } else {
    focusedElement = nullptr;
    focusedElementIndex = -1;
  }
}

void AeonWindow::setClickElement(AeonElement* element)
{
  if (clickedElement == element) return;
  if (clickedElement) {
    clickedElement->handleClickRelease();
  }
  clickedElement = element;
  if (element) {
    clickedElement->handleClick();
  }
  setTriggeredElement(element);
}

void AeonWindow::setTriggeredElement(AeonElement* element)
{
  if (element && element->isTriggered()) {
    triggeredElement = element;
  } else {
    triggeredElement = nullptr;
  }
}

}  // namespace ae