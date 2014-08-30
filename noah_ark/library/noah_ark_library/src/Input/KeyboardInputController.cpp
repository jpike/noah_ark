#include "Input/KeyboardInputController.h"

using namespace INPUT_CONTROL;

KeyboardInputController::KeyboardInputController(HGE* const pGameEngine) :
    m_enabled(true),
    m_keyboard(pGameEngine)
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
        const int UP_KEY_CODE = HGEK_UP;
        return m_keyboard.IsKeyDown(UP_KEY_CODE);
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
        const int DOWN_KEY_CODE = HGEK_DOWN;
        return m_keyboard.IsKeyDown(DOWN_KEY_CODE);
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
        const int LEFT_KEY_CODE = HGEK_LEFT;
        return m_keyboard.IsKeyDown(LEFT_KEY_CODE);
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
        const int RIGHT_KEY_CODE = HGEK_RIGHT;
        return m_keyboard.IsKeyDown(RIGHT_KEY_CODE);
    }
    else
    {
        return false;
    }
}
