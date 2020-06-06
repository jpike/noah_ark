#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace INPUT_CONTROL
{
    /// An input controller for the game that receives
    /// input from the keyboard and mouse.
    /// In order to update the state of input stored in this
    /// class, the ReadInput() method should be called once
    /// per frame.  This is needed to allow easier detection
    /// of more advanced input events, like a button being initially 
    /// pressed or released on a given frame (as opposed to just
    /// being up or down).  The mouse screen position must be
    /// set separately.
    class InputController
    {
    public:
        // STATIC CONSTANTS.
        /// The key for the primary action button.
        static constexpr sf::Keyboard::Key PRIMARY_ACTION_KEY = sf::Keyboard::Z;
        /// A textual representation of the primary action key.
        static constexpr char PRIMARY_ACTION_KEY_TEXT = 'Z';
        /// The key for the secondary button.
        static constexpr sf::Keyboard::Key SECONDARY_ACTION_KEY = sf::Keyboard::X;

        /// The key for toggling the map editor.
        static constexpr sf::Keyboard::Key MAP_EDITOR_KEY = sf::Keyboard::E;
        /// The key for toggling the map editor's tile palette.
        static constexpr sf::Keyboard::Key MAP_EDITOR_TILE_PALETTE_KEY = sf::Keyboard::T;
        /// The key for saving the updated map in a map editor.
        static constexpr sf::Keyboard::Key MAP_EDITOR_SAVE_MAP_KEY = sf::Keyboard::S;
        /// The key for closing the ark doors as a quick way to switch to that state.
        /// The key is completely arbitrary.
        static constexpr sf::Keyboard::Key DEBUG_CLOSE_ARK_DOORS_KEY = sf::Keyboard::J;

        /// The button for the main pointer.
        static constexpr sf::Mouse::Button MAIN_POINTER_BUTTON = sf::Mouse::Left;
        /// The button for the secondary pointer.
        static constexpr sf::Mouse::Button SECONDARY_POINTER_BUTTON = sf::Mouse::Right;

        // METHODS.
        void DisableInput();
        void EnableInput();

        void ReadInput();

        std::vector<sf::Keyboard::Key> GetTypedKeys() const;

        bool ButtonDown(const sf::Keyboard::Key key) const;
        bool ButtonDown(const sf::Mouse::Button button) const;
        bool ButtonWasPressed(const sf::Keyboard::Key key) const;
        bool ButtonWasPressed(const sf::Mouse::Button button) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The mouse supplying user input.
        INPUT_CONTROL::Mouse Mouse = INPUT_CONTROL::Mouse();

    private:
        // HELPER METHODS.
        bool InputEnabledAndKeyDown(const sf::Keyboard::Key key) const;
        bool InputEnabledAndKeyWasPressed(const sf::Keyboard::Key key) const;
        bool InputEnabledAndMouseButtonDown(const sf::Mouse::Button button) const;
        bool InputEnabledAndMouseButtonWasPressed(const sf::Mouse::Button button) const;

        // MEMBER VARIABLES.
        /// Whether or not this controller can supply input.
        bool Enabled = true;
        /// The keyboard supplying user input.
        INPUT_CONTROL::Keyboard Keyboard = INPUT_CONTROL::Keyboard();
    };
}