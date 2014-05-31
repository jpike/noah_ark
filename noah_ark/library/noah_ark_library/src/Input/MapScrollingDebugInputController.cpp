#include "Input/MapScrollingDebugInputController.h"

using namespace INPUT_CONTROL;

MapScrollingDebugInputController::MapScrollingDebugInputController(HGE* const pGameEngine) :
    m_enabled(true),
    m_keyboard(pGameEngine)
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
        const int SCROLL_UP_KEY_CODE = HGEK_UP;
        return m_keyboard.IsKeyDown(SCROLL_UP_KEY_CODE);
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
        const int SCROLL_DOWN_KEY_CODE = HGEK_DOWN;
        return m_keyboard.IsKeyDown(SCROLL_DOWN_KEY_CODE);
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
        const int SCROLL_LEFT_KEY_CODE = HGEK_LEFT;
        return m_keyboard.IsKeyDown(SCROLL_LEFT_KEY_CODE);
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
        const int SCROLL_RIGHT_KEY_CODE = HGEK_RIGHT;
        return m_keyboard.IsKeyDown(SCROLL_RIGHT_KEY_CODE);
    }
    else
    {
        return false;
    }
}
