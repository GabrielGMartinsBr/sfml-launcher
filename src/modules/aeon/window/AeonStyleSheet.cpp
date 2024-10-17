
#include "aeon/window/AeonStyleSheet.h"

#include <SFML/System/Vector2.hpp>

#include "aeon/toolkit/ColorParser.hpp"

namespace ae {

AeonStyleSheet::AeonStyleSheet() :
    ringSize(0),
    ringOffset(3),
    borderSize(2),
    radius(0),
    fontSize(18),
    padding(9, 3),
    fontName("Arial"),
    ringColor(ColorParser::hexToNrgssColor("#fcbf8199")),
    borderColor(ColorParser::hexToNrgssColor("#bbb")),
    bgColor(ColorParser::hexToNrgssColor("#eee")),
    textColor(ColorParser::hexToNrgssColor("#333"))
{
}
}  // namespace ae