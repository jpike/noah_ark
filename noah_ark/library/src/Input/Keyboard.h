#pragma once

#include <array>
#include <SFML/Window.hpp>

namespace INPUT_CONTROL
{
    /// Keyboard hardware that can supply user input.
    /// In order to update the state of keys stored in this
    /// class, the ReadKeys() method should be called once
    /// per frame.  This is needed to allow easier detection
    /// of more advanced key events, like a key being initially 
    /// pressed or released on a given frame (as opposed to just
    /// being up or down).
    class Keyboard
    {
    public:
        explicit Keyboard();

        void ReadKeys();

        bool IsKeyDown(const sf::Keyboard::Key key) const;
        bool IsKeyUp(const sf::Keyboard::Key key) const;

        bool WasKeyPressed(const sf::Keyboard::Key key) const;
        bool WasKeyReleased(const sf::Keyboard::Key key) const;

    private:
        /// The states of the keys for the previous frame.
        /// True if pressed; false if not pressed.
        std::array<bool, sf::Keyboard::KeyCount> PreviousFrameKeyStates;
        /// The states of the keys for the current frame.
        /// True if pressed; false if not pressed.
        std::array<bool, sf::Keyboard::KeyCount> CurrentFrameKeyStates;
    };
}