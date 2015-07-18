#include "Input/KeyboardInputController.h"

namespace INPUT_CONTROL
{
    KeyboardInputController::KeyboardInputController() :
    Enabled(true),
    Keyboard()
    {}

    KeyboardInputController::~KeyboardInputController()
    {}

    void KeyboardInputController::DisableInput()
    {
        Enabled = false;
    }

    void KeyboardInputController::EnableInput()
    {
        Enabled = true;
    }

    bool KeyboardInputController::UpButtonPressed() const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(sf::Keyboard::Up);
        }
        else
        {
            return false;
        }
    }

    bool KeyboardInputController::DownButtonPressed() const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(sf::Keyboard::Down);
        }
        else
        {
            return false;
        }
    }

    bool KeyboardInputController::LeftButtonPressed() const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(sf::Keyboard::Left);
        }
        else
        {
            return false;
        }
    }

    bool KeyboardInputController::RightButtonPressed() const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(sf::Keyboard::Right);
        }
        else
        {
            return false;
        }
    }

    bool KeyboardInputController::PrimaryActionButtonPressed() const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(sf::Keyboard::Z);
        }
        else
        {
            return false;
        }
    }
}
