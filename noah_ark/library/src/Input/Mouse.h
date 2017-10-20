#pragma once

#include <array>
#include <SFML/Window.hpp>
#include "Math/Vector2.h"

namespace INPUT_CONTROL
{
    /// Mouse hardware that can provide user input.
    ///
    /// The mouse screen position must be updated explicitly
    /// by external code.
    ///
    /// In order to update the state of buttons stored in this
    /// class, the ReadButtons() method should be called once
    /// per frame.  This is needed to allow easier detection
    /// of more advanced button events, like a button being initially 
    /// pressed or released on a given frame (as opposed to just
    /// being up or down).
    class Mouse
    {
    public:
        // CONSTRUCTION.
        explicit Mouse();

        // OTHER METHODS.
        void ReadButtons();

        bool WasButtonPressed(const sf::Mouse::Button button) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The screen position (in pixels) of the mouse cursor relative
        /// to the top-left of the game window.
        MATH::Vector2f ScreenPosition;

    private:
        // MEMBER VARIABLES.
        /// The states of the buttons for the previous frame.
        /// True if pressed; false if not pressed.
        std::array<bool, sf::Mouse::ButtonCount> PreviousFrameButtonStates;
        /// The states of the buttons for the current frame.
        /// True if pressed; false if not pressed.
        std::array<bool, sf::Mouse::ButtonCount> CurrentFrameButtonStates;
    };
}
