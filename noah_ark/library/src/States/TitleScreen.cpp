#include "Graphics/Color.h"
#include "States/TitleScreen.h"

namespace STATES
{
    /// Handles any user input for the title screen.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return True if the user selected an option for starting the main gameplay; false otherwise.
    bool TitleScreen::RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller) const
    {
        // CHECK IF THE MAIN 'START' BUTTON WAS PRESSED.
        bool start_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::START_KEY);
        return start_button_pressed;
    }

    /// Renders the title screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void TitleScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // DRAW THE SUB-HEADING FOR THE GAME'S TITLE.
        // It is centered within the top third of the screen.
        MATH::FloatRectangle screen_rectangle = renderer.Screen.GetBoundingRectangle<float>();
        float screen_left_x_position = screen_rectangle.GetLeftXPosition();
        float screen_top_y_position = screen_rectangle.GetTopYPosition();
        float screen_width_in_pixels = screen_rectangle.GetWidth();
        float screen_height_in_pixels = screen_rectangle.GetHeight();
        float one_third_of_screen_height_in_pixels = screen_height_in_pixels / 3.0f;
        MATH::FloatRectangle sub_heading_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            screen_top_y_position,
            screen_width_in_pixels,
            one_third_of_screen_height_in_pixels);
        const float BIBLE_GAMES_HEADING_TEXT_SCALE = 1.5f;
        renderer.RenderCenteredText(
            "BIBLE GAMES",
            sub_heading_screen_rectangle,
            GRAPHICS::Color::WHITE,
            BIBLE_GAMES_HEADING_TEXT_SCALE);

        // DRAW THE MAIN GAME'S TITLE.
        // It is centered within the middle third of the screen.
        MATH::FloatRectangle main_title_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            one_third_of_screen_height_in_pixels,
            screen_width_in_pixels,
            one_third_of_screen_height_in_pixels);
        const float TITLE_TEXT_SCALE = 3.0f;
        renderer.RenderCenteredText(
            "NOAH's ARK",
            main_title_screen_rectangle,
            GRAPHICS::Color::YELLOW,
            TITLE_TEXT_SCALE);

        // DRAW THE 'PRESS START' TEXT.
        // It is centered within the bottom third of the screen.
        float two_thirds_screen_height_in_pixels = 2 * one_third_of_screen_height_in_pixels;
        MATH::FloatRectangle press_enter_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            two_thirds_screen_height_in_pixels,
            screen_width_in_pixels,
            one_third_of_screen_height_in_pixels);
        renderer.RenderCenteredText(
            "PRESS ENTER",
            press_enter_screen_rectangle,
            GRAPHICS::Color::RED);
    }
}