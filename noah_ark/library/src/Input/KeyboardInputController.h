#pragma once

#include <SFML/Window.hpp>
#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    /// An input controller for the game that receives
    /// input from the keyboard.
    /// In order to update the state of input stored in this
    /// class, the ReadInput() method should be called once
    /// per frame.  This is needed to allow easier detection
    /// of more advanced input events, like a button being initially 
    /// pressed or released on a given frame (as opposed to just
    /// being up or down).
    class KeyboardInputController
    {
    public:
        // STATIC CONSTANTS.
        /// The key for the up button.
        static const sf::Keyboard::Key UP_KEY = sf::Keyboard::Up;
        /// The key for the down button.
        static const sf::Keyboard::Key DOWN_KEY = sf::Keyboard::Down;
        /// The key for the left button.
        static const sf::Keyboard::Key LEFT_KEY = sf::Keyboard::Left;
        /// The key for the right button.
        static const sf::Keyboard::Key RIGHT_KEY = sf::Keyboard::Right;
        /// The key for the primary action button.
        static const sf::Keyboard::Key PRIMARY_ACTION_KEY = sf::Keyboard::Z;
        /// A textual representation of the primary action key.
        static const char PRIMARY_ACTION_KEY_TEXT = 'Z';
        /// The key for the secondary button.
        static const sf::Keyboard::Key SECONDARY_ACTION_KEY = sf::Keyboard::X;
        /// The key for the start button.
        static const sf::Keyboard::Key START_KEY = sf::Keyboard::Return;
        /// The key for the back button.
        static const sf::Keyboard::Key BACK_KEY = sf::Keyboard::Escape;

        // METHODS.
        void DisableInput();
        void EnableInput();

        void ReadInput();

        bool ButtonDown(const sf::Keyboard::Key key) const;
        bool ButtonWasPressed(const sf::Keyboard::Key key) const;

    private:
        // HELPER METHODS.
        bool InputEnabledAndKeyDown(const sf::Keyboard::Key key) const;
        bool InputEnabledAndKeyWasPressed(const sf::Keyboard::Key key) const;

        // MEMBER VARIABLES.
        /// Whether or not this controller can supply input.
        bool Enabled = true;
        /// The keyboard supplying user input.
        INPUT_CONTROL::Keyboard Keyboard = INPUT_CONTROL::Keyboard();
    };
}