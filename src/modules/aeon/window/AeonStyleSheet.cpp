
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
    fillColor(ColorParser::hexToNrgssColor(style.fillColor)),
    textColor(ColorParser::hexToNrgssColor(style.textColor))
{
}

AeonStyleSheetBase buttonDefaultStyle = {
  .ringSize = 0,
  .ringOffset = 0,
  .borderSize = 2.5,
  .radius = 2,
  .fontSize = 16,
  .padding = { 24, 9 },
  .fontName = "Arial",
  .ringColor = "#fcbf8199",
  .borderColor = "#fff",
  .fillColor = "#34ed",
  .textColor = "#fff"
};

AeonStyleSheetBase textBoxDefaultStyle = {
  .ringSize = 0,
  .ringOffset = 0,
  .borderSize = 2.5,
  .radius = 2,
  .fontSize = 16,
  .padding = { 9, 6 },
  .fontName = "Arial",
  .ringColor = "#fcbf8199",
  .borderColor = "#fff",
  .fillColor = "#fff6",
  .textColor = "#fff"
};

}  // namespace ae