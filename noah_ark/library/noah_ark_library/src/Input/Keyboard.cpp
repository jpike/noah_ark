#pragma once

#include "Input/Keyboard.h"

using namespace INPUT_CONTROL;

Keyboard::Keyboard(HGE* const pGameEngine) :
    m_pGameEngine(pGameEngine)
{}

Keyboard::~Keyboard()
{}

bool Keyboard::IsKeyDown(const int keyCode) const
{
    bool keyDown = m_pGameEngine->Input_GetKeyState(keyCode);
    return keyDown;
}

bool Keyboard::IsKeyUp(const int keyCode) const
{
    bool keyUp = !IsKeyDown(keyCode);
    return keyUp;
}