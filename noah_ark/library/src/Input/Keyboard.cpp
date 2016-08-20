#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    /// Initializes the keyboard with no pressed keys.
    Keyboard::Keyboard() :
        PreviousFrameKeyStates(),
        CurrentFrameKeyStates()
    {
        PreviousFrameKeyStates.fill(false);
        CurrentFrameKeyStates.fill(false);
    }

    /// Reads the key states from the keyboard.
    /// Should be called once per frame to ensure
    /// accurate keyboard input is available.
    void Keyboard::ReadKeys()
    {
        // STORE THE FRAME STATES FOR THE PREVIOUS READ FRAME.
        // This allows for checking for changes across frames.
        PreviousFrameKeyStates = CurrentFrameKeyStates;

        // UPDATE KEY STATES FOR THE CURRENT FRAME.
        for (unsigned int key_code = sf::Keyboard::A;
            key_code < sf::Keyboard::KeyCount;
            ++key_code)
        {
            // STORE THE PRESSED STATE OF THE CURRENT KEY.
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(key_code);
            bool key_down = sf::Keyboard::isKeyPressed(key);
            CurrentFrameKeyStates[key_code] = key_down;
        }
    }

    /// Checks if a key is currently pressed down.
    /// @param[in]  key - The SFML key to check.
    /// @return     True if the key is down; false otherwise.
    bool Keyboard::IsKeyDown(const sf::Keyboard::Key key) const
    {
        bool key_down = CurrentFrameKeyStates[key];
        return key_down;
    }

    /// Checks if a key is currently not pressed down.
    /// @param[in]  key - The key to check.
    /// @return     True if the key is up; false otherwise.
    bool Keyboard::IsKeyUp(const sf::Keyboard::Key key) const
    {
        bool key_up = !IsKeyDown(key);
        return key_up;
    }

    /// Checks if a key was pressed down from an up state in the current frame.
    /// @return True if the key was pressed down this frame; false otherwise.
    bool Keyboard::WasKeyPressed(const sf::Keyboard::Key key) const
    {
        bool key_up_previous_frame = !PreviousFrameKeyStates[key];
        bool key_down_this_frame = CurrentFrameKeyStates[key];
        bool key_was_pressed_this_frame = (key_up_previous_frame && key_down_this_frame);
        return key_was_pressed_this_frame;
    }

    /// Checks if a key was released up from a down state in the current frame.
    /// @return True if the key was released this frame; false otherwise.
    bool Keyboard::WasKeyReleased(const sf::Keyboard::Key key) const
    {
        bool key_down_previous_frame = PreviousFrameKeyStates[key];
        bool key_up_this_frame = !CurrentFrameKeyStates[key];
        bool key_was_released_this_frame = (key_down_previous_frame && key_up_this_frame);
        return key_was_released_this_frame;
    }
}
