
#include "aeon/window/AeonStyleSheet.h"

#include <SFML/System/Vector2.hpp>

#include "aeon/toolkit/ColorParser.hpp"

namespace ae {

AeonStyleSheet::AeonStyleSheet(const AeonStyleSheetBase& style) :
    ringSize(style.ringSize),
    ringOffset(style.ringOffset),
    borderSize(style.borderSize),
    radius(style.radius),
    fontSize(style.fontSize),
    padding(style.padding),
    fontName(style.fontName),
    ringColor(ColorParser::hexToNrgssColor(style.ringColor)),
    borderColor(ColorParser::hexToNrgssColor(style.borderColor)),
    bgColor(ColorParser::hexToNrgssColor(style.bgColor)),
    textColor(ColorParser::hexToNrgssColor(style.textColor))
{
}

AeonStyleSheetBase buttonDefaultStyle = {
  .ringSize = 0,
  .ringOffset = 0,
  .borderSize = 2.5,
  .radius = 2,
  .fontSize = 18,
  .padding = { 9, 3 },
  .fontName = "Arial",
  .ringColor = "#fcbf8199",
  .borderColor = "#fff",
  .bgColor = "#34ed",
  .textColor = "#fff"
};

}  // namespace ae