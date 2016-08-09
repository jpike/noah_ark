#pragma once

#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    /// Checks if a key is currently pressed down.
    /// @param[in]  key_code - The SFML key code to check.
    /// @return     True if the key is down; false otherwise.
    bool Keyboard::IsKeyDown(const sf::Keyboard::Key key_code) const
    {
        bool key_down = sf::Keyboard::isKeyPressed(key_code);
        return key_down;
    }

    /// Checks if a key is currently not pressed down.
    /// @param[in]  key_code - The SFML key code to check.
    /// @return     True if the key is up; false otherwise.
    bool Keyboard::IsKeyUp(const sf::Keyboard::Key key_code) const
    {
        bool key_up = !IsKeyDown(key_code);
        return key_up;
    }
}
