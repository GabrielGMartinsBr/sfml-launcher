#pragma once

#include <SFML/System/Vector2.hpp>
namespace Core {

struct Vec2f {
  float x;
  float y;

  Vec2f() :
      Vec2f(0, 0) { }

  Vec2f(const sf::Vector2f& other) :
      Vec2f(other.x, other.y) { }

  Vec2f(const sf::Vector2i& other) :
      Vec2f(other.x, other.y) { }

  Vec2f(const sf::Vector2u& other) :
      Vec2f(other.x, other.y) { }

  Vec2f(float x, float y) :
      x(x),
      y(y) { }

  /*
    ⇩⇩⇩ Operators ⇩⇩⇩
  */

  //
  inline bool operator==(const Vec2f& other) const
  {
    return x == other.x && y == other.y;
  }
  inline Vec2f& operator=(const Vec2f& other)
  {
    x = other.x;
    y = other.y;
    return *this;
  }

  //
  inline Vec2f& operator+=(const Vec2f& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vec2f& operator-=(const Vec2f& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  inline Vec2f& operator*=(const Vec2f& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }
  inline Vec2f& operator/=(const Vec2f& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  //
  inline Vec2f& operator+=(float value)
  {
    x += value;
    y += value;
    return *this;
  }
  inline Vec2f& operator-=(float value)
  {
    x -= value;
    y -= value;
    return *this;
  }
  inline Vec2f& operator*=(float value)
  {
    x *= value;
    y *= value;
    return *this;
  }
  inline Vec2f& operator/=(float value)
  {
    x /= value;
    y /= value;
    return *this;
  }

  //
  inline Vec2f operator+(const Vec2f& other) const
  {
    return Vec2f(x + other.x, y + other.y);
  }
  inline Vec2f operator-(const Vec2f& other) const
  {
    return Vec2f(x - other.x, y - other.y);
  }
  inline Vec2f operator*(const Vec2f& other) const
  {
    return Vec2f(x * other.x, y * other.y);
  }
  inline Vec2f operator/(const Vec2f& other) const
  {
    return Vec2f(x / other.x, y / other.y);
  }

  //
  inline Vec2f operator+(float value) const
  {
    return Vec2f(x + value, y + value);
  }
  inline Vec2f operator-(float value) const
  {
    return Vec2f(x - value, y - value);
  }
  inline Vec2f operator*(float value) const
  {
    return Vec2f(x * value, y * value);
  }
  inline Vec2f operator/(float value) const
  {
    return Vec2f(x / value, y / value);
  }

  /*
    ⇩⇩⇩ Sfml Compatible ⇩⇩⇩
  */

  //
  inline bool operator==(const sf::Vector2f& other) const
  {
    return x == other.x && y == other.y;
  }
  inline Vec2f& operator=(const sf::Vector2f& other)
  {
    x = other.x;
    y = other.y;
    return *this;
  }

  //
  inline Vec2f& operator+=(const sf::Vector2f& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vec2f& operator-=(const sf::Vector2f& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  inline Vec2f& operator*=(const sf::Vector2f& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }
  inline Vec2f& operator/=(const sf::Vector2f& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  //
  inline Vec2f operator+(const sf::Vector2f& other) const
  {
    return Vec2f(x + other.x, y + other.y);
  }
  inline Vec2f operator-(const sf::Vector2f& other) const
  {
    return Vec2f(x - other.x, y - other.y);
  }
  inline Vec2f operator*(const sf::Vector2f& other) const
  {
    return Vec2f(x * other.x, y * other.y);
  }
  inline Vec2f operator/(const sf::Vector2f& other) const
  {
    return Vec2f(x / other.x, y / other.y);
  }

  sf::Vector2f sfVector2f()
  {
    return sf::Vector2f(x, y);
  }
};

}  // namespace Core