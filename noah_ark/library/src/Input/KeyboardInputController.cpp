#include "Input/KeyboardInputController.h"

namespace INPUT_CONTROL
{
    /// Disables input from the controller.
    void KeyboardInputController::DisableInput()
    {
        Enabled = false;
    }

    /// Enables input from the controller.
    void KeyboardInputController::EnableInput()
    {
        Enabled = true;
    }

    /// Checks if the up button is pressed.
    /// @return True if the up button is pressed (and input is enabled).
    ///     False otherwise.
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

    /// Checks if the down button is pressed.
    /// @return True if the down button is pressed (and input is enabled).
    ///     False otherwise.
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

    /// Checks if the left button is pressed.
    /// @return True if the left button is pressed (and input is enabled).
    ///     False otherwise.
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

    /// Checks if the right button is pressed.
    /// @return True if the right button is pressed (and input is enabled).
    ///     False otherwise.
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

    /// Checks if the primary action button is pressed.
    /// @return True if the primary action button is pressed (and input is enabled).
    ///     False otherwise.
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
