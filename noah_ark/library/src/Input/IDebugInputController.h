#pragma once

/// @brief  Holds code related to user input.
///         The namespace has _CONTROL appended to it only because
///         an existing INPUT symbol is defined by the Windows SDK.
namespace INPUT_CONTROL
{
    ///////////////////////////////////////////////////////////
    /// @brief  An interface to an input controller used for debug
    ///         and testing purposes.  This allows using user input
    ///         to quickly trigger certain actions in-game that 
    ///         would otherwise be more difficult to test.
    ///////////////////////////////////////////////////////////
    class IDebugInputController
    {
    public:
        /// @brief  Virtual destructor to allow proper destruction of implementers.
        virtual ~IDebugInputController() {};

        /// @brief  Disables input from the controller.
        virtual void DisableInput() = 0;
        /// @brief  Enables input from the controller.
        virtual void EnableInput() = 0;

        /// @brief  Checks if the button for scrolling the overworld map to the
        ///         above tile map is pressed.
        /// @return True if the scroll up button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool ScrollUpButtonPressed() const = 0;
        /// @brief  Checks if the button for scrolling the overworld map to the
        ///         below tile map is pressed.
        /// @return True if the scroll down button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool ScrollDownButtonPressed() const = 0;
        /// @brief  Checks if the button for scrolling the overworld map to the
        ///         left tile map is pressed.
        /// @return True if the scroll left button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool ScrollLeftButtonPressed() const = 0;
        /// @brief  Checks if the button for scrolling the overworld map to the
        ///         right tile map is pressed.
        /// @return True if the scroll right button is pressed (and input is enabled).
        ///         False otherwise.
        virtual bool ScrollRightButtonPressed() const = 0;
    };
}