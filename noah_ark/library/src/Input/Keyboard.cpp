#pragma once

#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    Keyboard::Keyboard()
    {}

    Keyboard::~Keyboard()
    {}

    bool Keyboard::IsKeyDown(const sf::Keyboard::Key key_code) const
    {
        bool key_down = sf::Keyboard::isKeyPressed(key_code);
        return key_down;
    }

    bool Keyboard::IsKeyUp(const sf::Keyboard::Key key_code) const
    {
        bool key_up = !IsKeyDown(key_code);
        return key_up;
    }
}
