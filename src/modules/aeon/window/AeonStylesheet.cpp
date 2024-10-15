
#include "modules/aeon/window/AeonStylesheet.h"

#include <SFML/System/Vector2.hpp>

#include "modules/aeon/toolkit/ColorParser.hpp"

namespace ae {

AeonStylesheet::AeonStylesheet() :
    ringSize(0),
    ringOffset(3),
    borderSize(1),
    radius(0),
    fontSize(18),
    padding(9, 3),
    fontName("Arial"),
    ringColor(ColorParser::hexToNrgssColor("#fcbf8199")),
    borderColor(ColorParser::hexToNrgssColor("#ddd")),
    bgColor(ColorParser::hexToNrgssColor("#fefefe")),
    textColor(ColorParser::hexToNrgssColor("#333"))
{
}

}  // namespace ae