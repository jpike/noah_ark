#pragma once

#include "Input/Keyboard.h"

using namespace INPUT_CONTROL;

Keyboard::Keyboard()
{}

Keyboard::~Keyboard()
{}

bool Keyboard::IsKeyDown(const sf::Keyboard::Key keyCode) const
{
    bool keyDown = sf::Keyboard::isKeyPressed(keyCode);
    return keyDown;
}

bool Keyboard::IsKeyUp(const sf::Keyboard::Key keyCode) const
{
    bool keyUp = !IsKeyDown(keyCode);
    return keyUp;
}