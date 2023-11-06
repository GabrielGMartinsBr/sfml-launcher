#include "./loadTexture.h"

sf::Texture loadCharsetTexture() {
  //   Load Image
  sf::Image image;
  if (!image.loadFromFile("../assets/Actor1.png")) {
    throw std::runtime_error("could not load image");
  }

  //   Remove Green Bg
  unsigned int width = image.getSize().x;
  unsigned int height = image.getSize().y;
  sf::Color greenBgColor = image.getPixel(0, 0);
  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int y = 0; y < height; ++y) {
      sf::Color pixelColor = image.getPixel(x, y);
      if (pixelColor == greenBgColor) {
        pixelColor.a = 0;
      }
      image.setPixel(x, y, pixelColor);
    }
  }

  //   Create Texture from modified image
  sf::Texture texture;
  if (!texture.loadFromImage(image)) {
    throw std::runtime_error("could not load texture");
  }
  return texture;
}