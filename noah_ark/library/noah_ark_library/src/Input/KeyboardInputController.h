#pragma once

#include <hge.h>
#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    ///////////////////////////////////////////////////////////
    /// @brief  An input controller for the game that receives
    ///         input from the keyboard.
    ///////////////////////////////////////////////////////////
    class KeyboardInputController
    {
    public:
        /// @brief      Constructor.  Input is enabled by default.
        /// @param[in]  pGameEngine - The HGE game engine.  Must not be nullptr.
        explicit KeyboardInputController(HGE* const pGameEngine);
        /// @brief  Destructor.
        virtual ~KeyboardInputController();

        /// @brief  Disables input from the controller.
        virtual void DisableInput();
        /// @brief  Enables input from the controller.
        virtual void EnableInput();

        /// @brief  Checks if the up button is pressed.
        /// @return True if the up button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool UpButtonPressed() const;
        /// @brief  Checks if the down button is pressed.
        /// @return True if the down button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool DownButtonPressed() const;
        /// @brief  Checks if the left button is pressed.
        /// @return True if the left button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool LeftButtonPressed() const;
        /// @brief  Checks if the right button is pressed.
        /// @return True if the right button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool RightButtonPressed() const;

    private:

        bool m_enabled; ///< Whether or not this controller can supply input.
        Keyboard m_keyboard;    ///< The keyboard supplying user input.
    };
}