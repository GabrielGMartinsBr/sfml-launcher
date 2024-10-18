#include "./AeonTextBoxElement.h"

#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonStyleSheet.h"
#include "engnine/base/Fonts.h"

namespace ae {

AeonTextBoxElement::AeonTextBoxElement() :
    AeonElement(ElementBounds(), textBoxDefaultStyle),
    textFont(nullptr),
    fontSize(16)
{
  applyStyle();
  applyBounds();
  text.setString("TextBox works!");
}

void AeonTextBoxElement::drawTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
  target.draw(text);
}

void AeonTextBoxElement::flush()
{
  refreshValues();
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonTextBoxElement::refreshValues()
{
  if (dirtyState) {
    applyState();
    alignText();
    dirtyState = false;
  }
  if (dirtyStyle) {
    applyStyle();
    dirtyStyle = false;
  }
  if (dirtyBounds) {
    applyBounds();
    dirtyBounds = false;
  }
}

void AeonTextBoxElement::applyBounds()
{
  shape.position(bounds.position());
  shape.size(bounds.size());
  alignText();
}

void AeonTextBoxElement::applyState()
{
  applyStyle();
  applyStateStyle(AeonElementState::HOVER);
  applyStateStyle(AeonElementState::FOCUS);
  applyStateStyle(AeonElementState::CLICKED);
}

void AeonTextBoxElement::applyStyle()
{
  applyStyle(defaultStyle);
}

void AeonTextBoxElement::applyStyle(const AeonStyleSheet& style)
{
  if (style.radius.has_value()) shape.radius(style.radius.value());
  if (style.borderSize.has_value()) shape.borderSize(style.borderSize.value());
  if (style.borderColor.has_value()) shape.borderColor(style.borderColor.value());
  if (style.fillColor.has_value()) shape.fillColor(style.fillColor.value());
  if (style.textColor.has_value()) {
    text.setFillColor(style.textColor.value().getSfColor());
  }
  if (style.fontSize.has_value()) {
    fontSize = style.fontSize.value();
    text.setCharacterSize(fontSize);
  }
  if (style.fontName.has_value()) {
    textFont = Eng::Fonts::Instance().getFont(style.fontName.value());
    if (textFont) {
      text.setFont(*textFont);
    }
  }
}

void AeonTextBoxElement::applyStateStyle(AeonElementState state)
{
  if (!hasState(state)) {
    return;
  }
  applyStyle(getStateStyle(state));
}

void AeonTextBoxElement::alignText()
{
  if (!textFont) return;

  text.setCharacterSize(fontSize);

  float leading = textFont->getLineSpacing(fontSize);
  const sf::FloatRect& globalBounds = text.getGlobalBounds();
  const sf::FloatRect& localBounds = text.getLocalBounds();

  const Vector2f padding = getCurrentPadding();

  Vector2f textOrigin(
    -padding.x,
    -(bounds.height() - leading - localBounds.top) / 2
  );

  text.setOrigin(textOrigin);
  text.setPosition(bounds.position());
}

const Vector2f AeonTextBoxElement::getCurrentPadding()
{
  Vector2f padding = defaultStyle.padding.value_or(Vector2f(0, 0));
  if (hasState(AeonElementState::HOVER)) {
    const AeonStyleSheet& style = getStateStyle(AeonElementState::HOVER);
    if (style.padding.has_value()) {
      padding = style.padding.value();
    }
  }
  return padding;
}

}  // namespace ae