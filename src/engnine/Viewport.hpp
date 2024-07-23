#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/RGSSViewport.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"

namespace Eng {

class Viewport : public EngineBase {
 public:

  Viewport(int x, int y, int width, int height) :
      rect(x, y, width, height),
      rgssVp(x, y, width, height)
  {
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;
  }

  Viewport(Rect* _rect) :
      rect(_rect),
      rgssVp(rect)
  {
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;
  }

  RGSS::Viewport& getRgssViewport()
  {
    return rgssVp;
  }

  /*
    Attr rect
  */
  Rect* getRect() { return &rect; }
  void setRect(Rect* _rect)
  {
    rect = *_rect;
  }

  /*
    Attr visible
  */
  int getVisible() { return visible; }
  void setVisible(int _visible)
  {
    visible = _visible;
  }

  /*
    Attr z
  */
  int getZ() { return z; }
  void setZ(int _z)
  {
    z = _z;
  }

  /*
    Attr ox
  */
  int getOx() { return ox; }
  void setOx(int _ox)
  {
    ox = _ox;
  }

  /*
    Attr oy
  */
  int getOy() { return oy; }
  void setOy(int _oy)
  {
    oy = _oy;
  }

  /* --------------------------------------------------- */

  // Getter color

  Color* getter_color()
  {
    return color;
  }

  // Setter color

  void setter_color(Color* value)
  {
    color = value;
  }

  /* --------------------------------------------------- */

  // Getter tone

  Tone* getter_tone()
  {
    return tone;
  }

  // Setter tone

  void setter_tone(Tone* value)
  {
    tone = value;
  }

  /* --------------------------------------------------- */

  /*
      RGSS Methods
  */

  void method_dispose()
  {
    isDisposed = true;
  }

  bool method_disposed()
  {
    return isDisposed;
  }

  void method_flash(Color* color, int time)
  {
    Log::out() << "Viewport method flash was called but it's not implemented yet.";
  }

  void method_update()
  {
    // Log::out() << "ViewportViewport method update was called but it's not implemented yet.";
  }

 private:
  Rect rect;
  bool visible = true;
  int z = 0;
  int ox = 0;
  int oy = 0;
  Color* color;
  Tone* tone;

  bool isDisposed;

  RGSS::Viewport rgssVp;
};

}  // namespace Eng