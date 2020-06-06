#include <cmath>
#include "States/CreditsScreen.h"

namespace STATES
{
    /// Updates the credits screen based on elapsed time and user input.
    /// @param[in]  elapsed_time - The elapsed time for which to update the credits screen.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return The state the game should be in based on the user's input.
    GameState CreditsScreen::Update(
        const sf::Time& elapsed_time, 
        const INPUT_CONTROL::InputController& input_controller)
    {
        // CHECK IF THE BACK BUTTON WAS PRESSED.
        bool back_button_pressed = input_controller.ButtonWasPressed(sf::Keyboard::Escape);
        if (back_button_pressed)
        {
            // RETURN TO THE TITLE SCREEN.
            return GameState::TITLE_SCREEN;
        }
        else
        {
            // TRACK THE ELAPSED TIME.
            ElapsedTime += elapsed_time;

            // INDICATE THE GAME SHOULD REMAIN ON THE CREDITS SCREEN.
            return GameState::CREDITS_SCREEN;
        }
    }

    /// Renders the credits screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered credits screen.
    sf::Sprite CreditsScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // CALCULATE THE OFFSET OF THE TOP OF THE CREDITS BASED ON THE ELAPSED TIME.
        // This helps implement scrolling of the credits text upward.
        constexpr float SCROLL_RATE_IN_PIXELS_PER_SECOND = -4.0f;
        float elapsed_time_in_seconds = ElapsedTime.asSeconds();
        // The result needs to be rounded to an integer to avoid flickering due to sub-pixel values.
        float credits_text_top_offset_in_pixels = std::round(SCROLL_RATE_IN_PIXELS_PER_SECOND * elapsed_time_in_seconds);

        // RENDER TEXT INDICATING THE PURPOSE OF THIS SCREEN.
        const GRAPHICS::Color CREDITS_TEXT_COLOR = GRAPHICS::Color::WHITE;
        
        MATH::FloatRectangle screen_rectangle = renderer.Screen->GetBoundingRectangle<float>();
        float screen_left_x_position = screen_rectangle.LeftTop.X;
        float screen_top_y_position = screen_rectangle.LeftTop.Y;
        float screen_width_in_pixels = screen_rectangle.Width();
        float screen_height_in_pixels = screen_rectangle.Height();
        constexpr float SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        float credits_text_start_top_y_position = screen_top_y_position + credits_text_top_offset_in_pixels;
        MATH::FloatRectangle credits_title_text_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            credits_text_start_top_y_position,
            screen_width_in_pixels,
            SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);

        renderer.RenderCenteredText(
            "CREDITS",
            RESOURCES::AssetId::FONT_TEXTURE,
            credits_title_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER TEXT CREDITING GOD.
        const std::string GOD_CREDIT_TEXT = R"(Him by whom "all things were created" and "all things consist" - from Colossians 1:16-17 (KJV))";

        // Some additional spacing is added to help offset this credit text from previous text.
        float start_of_credits_top_y_position_in_pixels = 2 * SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS + credits_text_start_top_y_position;

        // This text takes multiple lines to render using the current font.
        constexpr unsigned int GOD_CREDIT_TEXT_LINE_COUNT = 4;
        constexpr float GOD_CREDIT_TEXT_HEIGHT_IN_PIXELS = static_cast<float>(GOD_CREDIT_TEXT_LINE_COUNT * SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);
        MATH::FloatRectangle current_credit_text_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            start_of_credits_top_y_position_in_pixels,
            screen_width_in_pixels,
            GOD_CREDIT_TEXT_HEIGHT_IN_PIXELS);

        renderer.RenderCenteredText(
            GOD_CREDIT_TEXT,
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER ADDITIONAL CREDITS FOR INDIVIDUAL PEOPLE.
        // Some additional spacing is added to help offset this credit text from other text.
        constexpr float SPACING_BEFORE_REMAINING_CREDITS = (2 * SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);
        float current_credits_text_top_y_position_in_pixels = 
            start_of_credits_top_y_position_in_pixels +
            GOD_CREDIT_TEXT_HEIGHT_IN_PIXELS +
            SPACING_BEFORE_REMAINING_CREDITS;
        current_credit_text_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            current_credits_text_top_y_position_in_pixels,
            screen_width_in_pixels,
            SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);
        renderer.RenderCenteredText(
            "Programming - Jacob Pike",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        const MATH::Vector2f MOVE_DOWN_SINGLE_LINE(0, SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);        
        renderer.RenderCenteredText(
            "Graphics - Jacob Pike",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Music - Chad Pike",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Music - Jacob Pike",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER ADDITIONAL CREDITS FOR SOUND EFFECTS.
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "ANIMAL SOUNDS",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "soundbible.com",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "BuffBill84",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Mike Koenig",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "snottyboy",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "NPS",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "fws.gov",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "BlastwaveFx.com",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Daniel Simion",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Caroline Ford",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "JimBob",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "J Dawg",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "wrzesien",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Lisa Redfern",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Tony Phillips",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER ADDITIONAL CREDITS FOR DEVELOPMENT TOOLS.
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "DEVELOPMENT TOOLS",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Visual Studio 2019",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Git/GitHub",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Microsoft Paint",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Bfxr",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Bosca Ceoil",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Doxygen",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "SFML",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "Catch",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER ADDITIONAL CREDITS FOR OTHER RESOURCES.
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "OTHER RESOURCES",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        current_credit_text_screen_rectangle.Move(MOVE_DOWN_SINGLE_LINE);
        renderer.RenderCenteredText(
            "KJV Bible",
            RESOURCES::AssetId::FONT_TEXTURE,
            current_credit_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RENDER SOME HELP TEXT TO INFORM PLAYERS HOW TO RETURN TO THE TITLE SCREEN.
        const float SCREEN_Y_POSITION_ONE_LINE_BEFORE_BOTTOM = screen_height_in_pixels - SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS;
        MATH::FloatRectangle help_text_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            SCREEN_Y_POSITION_ONE_LINE_BEFORE_BOTTOM,
            screen_width_in_pixels,
            SINGLE_TEXT_LINE_HEIGHT_IN_PIXELS);
        // A black rectangle is rendered behind the text to avoid having the text overlap
        // with other text from the credits and make things hard to read.
        renderer.RenderScreenRectangle(help_text_screen_rectangle, GRAPHICS::Color::BLACK);
        renderer.RenderText(
            "[ESC] to title",
            RESOURCES::AssetId::FONT_TEXTURE,
            help_text_screen_rectangle,
            CREDITS_TEXT_COLOR);

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen();
        return screen;
    }
}