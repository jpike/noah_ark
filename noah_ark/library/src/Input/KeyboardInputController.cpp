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

    /// Reads the input states from the controller, if input is enabled.
    /// Should be called once per frame to ensure
    /// accurate input is available.
    void KeyboardInputController::ReadInput()
    {
        if (Enabled)
        {
            Keyboard.ReadKeys();
        }
    }

    /// Checks if the up button is currently pressed down.
    /// @return True if the up button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::UpButtonDown() const
    {
        return InputEnabledAndKeyDown(UP_KEY);
    }

    /// Checks if the down button is currently pressed down.
    /// @return True if the down button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::DownButtonDown() const
    {
        return InputEnabledAndKeyDown(DOWN_KEY);
    }

    /// Checks if the left button is currently pressed down.
    /// @return True if the left button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::LeftButtonDown() const
    {
        return InputEnabledAndKeyDown(LEFT_KEY);
    }

    /// Checks if the right button is currently pressed down.
    /// @return True if the right button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::RightButtonDown() const
    {
        return InputEnabledAndKeyDown(RIGHT_KEY);
    }

    /// Checks if the primary action button is currently pressed down.
    /// @return True if the primary action button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::PrimaryActionButtonDown() const
    {
        return InputEnabledAndKeyDown(PRIMARY_ACTION_KEY);
    }

    /// Checks if the secondary action button is currently pressed down.
    /// @return True if the secondary action button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::SecondaryActionButtonDown() const
    {
        return InputEnabledAndKeyDown(SECONDARY_ACTION_KEY);
    }

    /// Checks if the start button is currently pressed down.
    /// @return True if the start button is currently pressed (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::StartButtonDown() const
    {
        return InputEnabledAndKeyDown(START_KEY);
    }

    /// Checks if the up button was pressed down this frame.
    /// @return True if the up button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::UpButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(UP_KEY);
    }
    
    /// Checks if the down button was pressed down this frame.
    /// @return True if the down button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::DownButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(DOWN_KEY);
    }

    /// Checks if the left button was pressed down this frame.
    /// @return True if the left button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::LeftButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(LEFT_KEY);
    }

    /// Checks if the right button was pressed down this frame.
    /// @return True if the right button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::RightButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(RIGHT_KEY);
    }

    /// Checks if the primary action button was pressed down this frame.
    /// @return True if the primary action button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::PrimaryActionButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(PRIMARY_ACTION_KEY);
    }

    /// Checks if the secondary action button is was pressed down this frame.
    /// @return True if the secondary action button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::SecondaryActionButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(SECONDARY_ACTION_KEY);
    }

    /// Checks if the start button was pressed down this frame.
    /// @return True if the start button was pressed this frame (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::StartButtonWasPressed() const
    {
        return InputEnabledAndKeyWasPressed(START_KEY);
    }

    /// A helper method to return if a key is currently down and input is enabled.
    /// @param[in]  key - The key to check.
    /// @return True if the key is currently down (and input is enabled).
    ///     False otherwise.
    bool KeyboardInputController::InputEnabledAndKeyDown(const sf::Keyboard::Key key) const
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
    bool KeyboardInputController::InputEnabledAndKeyWasPressed(const sf::Keyboard::Key key) const
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
}
