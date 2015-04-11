#pragma once

#include <SFML/Window.hpp>

namespace INPUT_CONTROL
{
    ///////////////////////////////////////////////////////////
    /// @brief  Represents keyboard hardware that can supply user input.
    ///////////////////////////////////////////////////////////
    class Keyboard
    {
    public:
        /// @brief      Constructor.
        explicit Keyboard();
        /// @brief  Destructor.
        ~Keyboard();

        /// @brief      Checks if a key is currently pressed down.
        /// @param[in]  keyCode - The SFML key code to check.
        /// @return     True if the key is down; false otherwise.
        bool IsKeyDown(const sf::Keyboard::Key keyCode) const;
        /// @brief      Checks if a key is currently not pressed down.
        /// @param[in]  keyCode - The SFML key code to check.
        /// @return     True if the key is up; false otherwise.
        bool IsKeyUp(const sf::Keyboard::Key keyCode) const;
    };
}