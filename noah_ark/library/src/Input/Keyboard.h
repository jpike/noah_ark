#pragma once

#include <SFML/Window.hpp>

namespace INPUT_CONTROL
{
    /// Represents keyboard hardware that can supply user input.
    class Keyboard
    {
    public:
        /// Constructor.
        explicit Keyboard();
        /// Destructor.
        ~Keyboard();

        /// Checks if a key is currently pressed down.
        /// @param[in]  key_code - The SFML key code to check.
        /// @return     True if the key is down; false otherwise.
        bool IsKeyDown(const sf::Keyboard::Key key_code) const;
        /// Checks if a key is currently not pressed down.
        /// @param[in]  key_code - The SFML key code to check.
        /// @return     True if the key is up; false otherwise.
        bool IsKeyUp(const sf::Keyboard::Key key_code) const;
    };
}