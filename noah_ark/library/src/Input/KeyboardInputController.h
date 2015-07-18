#pragma once

#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    /// An input controller for the game that receives
    /// input from the keyboard.
    class KeyboardInputController
    {
    public:
        /// Constructor.  Input is enabled by default.
        explicit KeyboardInputController();
        /// Destructor.
        virtual ~KeyboardInputController();

        /// Disables input from the controller.
        virtual void DisableInput();
        /// Enables input from the controller.
        virtual void EnableInput();

        /// Checks if the up button is pressed.
        /// @return True if the up button is pressed (and input is enabled).
        ///     False otherwise.
        virtual bool UpButtonPressed() const;
        /// Checks if the down button is pressed.
        /// @return True if the down button is pressed (and input is enabled).
        ///     False otherwise.
        virtual bool DownButtonPressed() const;
        /// Checks if the left button is pressed.
        /// @return True if the left button is pressed (and input is enabled).
        ///     False otherwise.
        virtual bool LeftButtonPressed() const;
        /// Checks if the right button is pressed.
        /// @return True if the right button is pressed (and input is enabled).
        ///     False otherwise.
        virtual bool RightButtonPressed() const;

        /// Checks if the primary action button is pressed.
        /// @return True if the primary action button is pressed (and input is enabled).
        ///     False otherwise.
        virtual bool PrimaryActionButtonPressed() const;

    private:

        bool Enabled; ///< Whether or not this controller can supply input.
        Keyboard Keyboard;    ///< The keyboard supplying user input.
    };
}