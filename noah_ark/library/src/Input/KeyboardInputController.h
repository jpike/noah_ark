#pragma once

#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    /// An input controller for the game that receives
    /// input from the keyboard.
    class KeyboardInputController
    {
    public:
        void DisableInput();
        void EnableInput();

        bool UpButtonPressed() const;
        bool DownButtonPressed() const;
        bool LeftButtonPressed() const;
        bool RightButtonPressed() const;
        bool PrimaryActionButtonPressed() const;

    private:
        bool Enabled = true; ///< Whether or not this controller can supply input.
        INPUT_CONTROL::Keyboard Keyboard = INPUT_CONTROL::Keyboard();    ///< The keyboard supplying user input.
    };
}