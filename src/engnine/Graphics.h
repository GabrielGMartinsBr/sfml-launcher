#pragma once

namespace Eng {

class Graphics {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Graphics& GetInstance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  unsigned long frame_count;

  unsigned int getFrameRate();

  void setFrameRate(unsigned int v);

  void update();

  void freeze();  // TODO: Implement this method

  // TODO: Implement this method
  void transition();

  // TODO: Implement this method
  void frame_reset();

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  unsigned int frame_rate;

  Graphics();

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);
};

}  // namespace Eng