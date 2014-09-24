#include "Input/KeyboardInputController.h"

using namespace INPUT_CONTROL;

KeyboardInputController::KeyboardInputController() :
    m_enabled(true),
    m_keyboard()
{}

KeyboardInputController::~KeyboardInputController()
{}

void KeyboardInputController::DisableInput()
{
    m_enabled = false;
}

void KeyboardInputController::EnableInput()
{
    m_enabled = true;
}

bool KeyboardInputController::UpButtonPressed() const
{
    if (m_enabled)
    {
        return m_keyboard.IsKeyDown(sf::Keyboard::Up);
    }
    else
    {
        return false;
    }
}
        
bool KeyboardInputController::DownButtonPressed() const
{
    if (m_enabled)
    {
        return m_keyboard.IsKeyDown(sf::Keyboard::Down);
    }
    else
    {
        return false;
    }
}
        
bool KeyboardInputController::LeftButtonPressed() const
{
    if (m_enabled)
    {
        return m_keyboard.IsKeyDown(sf::Keyboard::Left);
    }
    else
    {
        return false;
    }
}
        
bool KeyboardInputController::RightButtonPressed() const
{
    if (m_enabled)
    {
        return m_keyboard.IsKeyDown(sf::Keyboard::Right);
    }
    else
    {
        return false;
    }
}
