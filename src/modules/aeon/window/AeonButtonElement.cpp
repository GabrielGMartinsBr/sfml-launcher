
#include "aeon/window/AeonButtonElement.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "aeon/enums/AeonElementState.h"

namespace ae {

// Constructor

AeonButtonElement::AeonButtonElement(
  const ElementBounds& bounds,
  const AeonStyleSheet& style
) :
    AeonElement(bounds, style)
{
  applyBounds();
  applyStyle();
}

void AeonButtonElement::drawTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
}

/*
  ⇩⇩⇩ Window Elements Methods ⇩⇩⇩
*/

const sf::String& AeonButtonElement::setText(const sf::String& value)
{
  text = value;
  return text;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonButtonElement::refreshValues()
{
  if (dirtyBounds) {
    applyBounds();
    dirtyBounds = false;
  }
  if (dirtyState) {
    applyState();
    dirtyState = false;
  }
  if (dirtyStyle) {
    applyStyle();
    dirtyStyle = false;
  }
}

void AeonButtonElement::applyBounds()
{
  shape.position(bounds.position());
  shape.size(bounds.size());
}

void AeonButtonElement::applyStyle()
{
  shape.borderSize(defaultStyle.borderSize);
  shape.borderColor(defaultStyle.borderColor.getSfColor());
  shape.fillColor(defaultStyle.bgColor.getSfColor());
}

void AeonButtonElement::applyState()
{
  applyStyle();
  applyStateStyle(AeonElementState::HOVER);
  applyStateStyle(AeonElementState::FOCUS);
  applyStateStyle(AeonElementState::CLICKED);
}

void AeonButtonElement::applyStateStyle(AeonElementState state)
{
  if (!hasState(state)) {
    return;
  }
  const AeonPartialStyleSheet& style = getStateStyle(state);
  if (style.borderSize.has_value()) shape.borderSize(style.borderSize.value());
  if (style.borderColor.has_value()) shape.borderColor(style.borderColor.value());
  if (style.bgColor.has_value()) shape.fillColor(style.bgColor.value());
}

}  // namespace ae