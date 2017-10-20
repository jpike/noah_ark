#include "Input/InputController.h"

namespace INPUT_CONTROL
{
    /// Disables input from the controller.
    void InputController::DisableInput()
    {
        Enabled = false;
    }

    /// Enables input from the controller.
    void InputController::EnableInput()
    {
        Enabled = true;
    }

    /// Reads the input states from the controller, if input is enabled.
    /// Should be called once per frame to ensure
    /// accurate input is available.
    void InputController::ReadInput()
    {
        if (Enabled)
        {
            Keyboard.ReadKeys();
            Mouse.ReadButtons();
        }
    }

    /// Checks if the specified button is currently pressed down.
    /// @param[in]  key - The key for the button to check.
    /// @return True if the specified button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool InputController::ButtonDown(const sf::Keyboard::Key key) const
    {
        return InputEnabledAndKeyDown(key);
    }

    /// Checks if the specified button was pressed down this frame.
    /// @param[in]  key - The key for the button to check.
    /// @return True if the specified button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool InputController::ButtonWasPressed(const sf::Keyboard::Key key) const
    {
        return InputEnabledAndKeyWasPressed(key);
    }

    /// Checks if the specified button was pressed down this frame.
    /// @param[in]  button - The mouse button to check.
    /// @return True if the specified button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool InputController::ButtonWasPressed(const sf::Mouse::Button button) const
    {
        return InputEnabledAndMouseButtonWasPressed(button);
    }

    /// A helper method to return if a key is currently down and input is enabled.
    /// @param[in]  key - The key to check.
    /// @return True if the key is currently down (and input is enabled).
    ///     False otherwise.
    bool InputController::InputEnabledAndKeyDown(const sf::Keyboard::Key key) const
    {
        if (Enabled)
        {
            return Keyboard.IsKeyDown(key);
        }
        else
        {
            return false;
        }
    }

    /// A helper method to return if a key was pressed this frame and input is enabled.
    /// @param[in]  key - The key to check.
    /// @return True if the key was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool InputController::InputEnabledAndKeyWasPressed(const sf::Keyboard::Key key) const
    {
        if (Enabled)
        {
            return Keyboard.WasKeyPressed(key);
        }
        else
        {
            return false;
        }
    }

    /// A helper method to return if a mouse button was pressed this frame and input is enabled.
    /// @param[in]  button - The mouse button to check.
    /// @return True if the button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool InputController::InputEnabledAndMouseButtonWasPressed(const sf::Mouse::Button button) const
    {
        if (Enabled)
        {
            return Mouse.WasButtonPressed(button);
        }
        else
        {
            return false;
        }
    }
}
