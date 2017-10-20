#include "Input/Mouse.h"

namespace INPUT_CONTROL
{
    /// Initialize the mouse with no buttons considered pressed.
    Mouse::Mouse() :
        PreviousFrameButtonStates(),
        CurrentFrameButtonStates()
    {
        PreviousFrameButtonStates.fill(false);
        CurrentFrameButtonStates.fill(false);
    }

    /// Reads button pressed states for the mouse.
    /// Should be called once per frame to ensure
    /// accurate mouse input is available.
    void Mouse::ReadButtons()
    {
        // STORE THE FRAME STATES FOR THE PREVIOUS READ FRAME.
        // This allows for checking for changes across frames.
        PreviousFrameButtonStates = CurrentFrameButtonStates;

        // UPDATE BUTTON STATES FOR THE CURRENT FRAME.
        for (unsigned int button_code = sf::Mouse::Left;
            button_code < sf::Mouse::ButtonCount;
            ++button_code)
        {
            // STORE THE PRESSED STATE OF THE CURRENT BUTTON.
            sf::Mouse::Button button = static_cast<sf::Mouse::Button>(button_code);
            bool button_down = sf::Mouse::isButtonPressed(button);
            CurrentFrameButtonStates[button_code] = button_down;
        }
    }

    /// Checks if a button was pressed down from an up state in the current frame.
    /// @return True if the button was pressed down this frame; false otherwise.
    bool Mouse::WasButtonPressed(const sf::Mouse::Button button) const
    {
        bool button_up_previous_frame = !PreviousFrameButtonStates[button];
        bool button_down_this_frame = CurrentFrameButtonStates[button];
        bool button_was_pressed_this_frame = (button_up_previous_frame && button_down_this_frame);
        return button_was_pressed_this_frame;
    }
}
