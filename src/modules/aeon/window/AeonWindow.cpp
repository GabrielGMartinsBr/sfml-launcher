#include "modules/aeon/window/AeonWindow.h"

#include "engnine/Window.h"

namespace ae {

AeonWindow::AeonWindow(VALUE rbObj, Eng::Viewport *viewport) :
    Eng::Window(rbObj, viewport)
{
}

}  // namespace ae