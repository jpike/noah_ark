#pragma once

#include <SFML/Window.hpp>

namespace INPUT_CONTROL
{
    /// Represents keyboard hardware that can supply user input.
    class Keyboard
    {
    public:
        bool IsKeyDown(const sf::Keyboard::Key key_code) const;
        bool IsKeyUp(const sf::Keyboard::Key key_code) const;
    };
}