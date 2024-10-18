#pragma once

#include <SFML/Window/Event.hpp>

namespace ae {

using AeMouseMoveEvent = sf::Event::MouseMoveEvent;
using AeMouseButtonEvent = sf::Event::MouseButtonEvent;
using AeKeyEvent = sf::Event::KeyEvent;
using AeTextEvent = sf::Event::TextEvent;

}  // namespace ae