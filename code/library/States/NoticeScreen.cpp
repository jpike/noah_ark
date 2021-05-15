#include "Graphics/Gui/Text.h"
#include "States/NoticeScreen.h"

namespace STATES
{
    /// Updates the notice screen based on elapsed time and user input.
    /// @param[in]  gaming_hardware - The hardware supplying input for the update.
    /// @return The state the game should be in based on the user's input.
    GameState NoticeScreen::Update(const HARDWARE::GamingHardware& gaming_hardware)
    {
        // CHECK IF THE BACK BUTTON WAS PRESSED.
        bool back_button_pressed = gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Escape);
        if (back_button_pressed)
        {
            // RETURN TO THE TITLE SCREEN.
            return GameState::TITLE_SCREEN;
        }
        else
        {
            // INDICATE THE GAME SHOULD REMAIN ON THE CREDITS SCREEN.
            return GameState::NOTICE_SCREEN;
        }
    }

    /// Renders the notice screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered notice screen.
    sf::Sprite NoticeScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER THE NOTICE.
        const GRAPHICS::Color TEXT_COLOR = GRAPHICS::Color::WHITE;
        MATH::FloatRectangle screen_rectangle = renderer.Screen->GetBoundingRectangle<float>();
        std::string notice_text =
            "This game is not rated by the ESRB or any other ratings board."
            "This game uses text directly from the public domain King James Version (KJV) English translation of the Bible.  "
            "While the entire Bible is not included in this game, the verses that are included aim to be authentic to the "
            "truth of Scripture, not shying away from what the text says.";
        renderer.RenderCenteredText(
            notice_text,
            RESOURCES::AssetId::FONT_TEXTURE,
            screen_rectangle,
            TEXT_COLOR);

        // RENDER INSTRUCTIONS FOR RETURNING TO THE TITLE SCREEN.
        // It's positioned near the bottom of the screen to have it out of the way.
        float screen_left_x_position = screen_rectangle.LeftTop.X;
        float screen_width_in_pixels = screen_rectangle.Width();

        float screen_height_in_pixels = screen_rectangle.Height();
        constexpr float SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        float screen_y_position_one_line_before_bottom = screen_height_in_pixels - SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS;

        MATH::FloatRectangle help_text_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            screen_y_position_one_line_before_bottom,
            screen_width_in_pixels,
            SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);

        renderer.RenderText(
            "[ESC] to title",
            RESOURCES::AssetId::FONT_TEXTURE,
            help_text_screen_rectangle,
            TEXT_COLOR);

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen();
        return screen;
    }
}
