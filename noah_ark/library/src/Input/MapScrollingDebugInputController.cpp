#include "Input/MapScrollingDebugInputController.h"

using namespace INPUT_CONTROL;

MapScrollingDebugInputController::MapScrollingDebugInputController() :
    m_enabled(true),
    m_keyboard()
{}

MapScrollingDebugInputController::~MapScrollingDebugInputController()
{}

void MapScrollingDebugInputController::DisableInput()
{
    m_enabled = false;
}

void MapScrollingDebugInputController::EnableInput()
{
    m_enabled = true;
}

bool MapScrollingDebugInputController::ScrollUpButtonPressed() const
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
        
bool MapScrollingDebugInputController::ScrollDownButtonPressed() const
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
        
bool MapScrollingDebugInputController::ScrollLeftButtonPressed() const
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
        
bool MapScrollingDebugInputController::ScrollRightButtonPressed() const
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
