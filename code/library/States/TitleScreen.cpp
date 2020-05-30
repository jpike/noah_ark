#include "Graphics/Color.h"
#include "States/TitleScreen.h"

namespace STATES
{
    /// Constructor.
    TitleScreen::TitleScreen() :
        SelectedMenuOptionIndex(0),
        MenuOptions({ GameState::GAME_SELECTION_SCREEN, GameState::CREDITS_SCREEN })
    {}

    /// Updates the title screen based on elapsed time and user input.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return The state the game after updating the title screen.
    GameState TitleScreen::Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller)
    {
        // UPDATE THE ELAPSED TIME FOR THE TITLE SCREEN.
        ElapsedTime += elapsed_time;

        // CHECK IF THE MAIN 'START' BUTTON WAS PRESSED.
        bool start_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::START_KEY);
        if (start_button_pressed)
        {
            // GET THE STATE FOR THE CURRENTLY SELECTED MENU OPTION.
            GameState next_state = MenuOptions.at(SelectedMenuOptionIndex);
            return next_state;
        }
        else
        {
            // CHECK IF AN ARROW KEY WAS PRESSED TO MOVE TO ANOTHER MENU OPTION.
            bool up_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::UP_KEY);
            bool down_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DOWN_KEY);
            if (up_button_pressed)
            {
                // MOVE TO THE PREVIOUS MENU OPTION.
                // Protection against moving to an invalid menu option is needed.
                bool previous_menu_option_exists = (SelectedMenuOptionIndex > 0);
                if (previous_menu_option_exists)
                {
                    --SelectedMenuOptionIndex;
                }
                
            }
            else if (down_button_pressed)
            {
                // MOVE TO THE NEXT MENU OPTION.
                // Protection against moving to an invalid menu option is needed.
                size_t menu_option_count = MenuOptions.size();
                size_t last_menu_option_index = menu_option_count - 1;
                bool next_menu_option_exists = (SelectedMenuOptionIndex < last_menu_option_index);
                if (next_menu_option_exists)
                {
                    ++SelectedMenuOptionIndex;
                }
            }
            
            // INDICATE THE GAME SHOULD REMAIN ON THE TITLE SCREEN.
            return GameState::TITLE_SCREEN;
        }
    }

    /// Renders the title screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered title screen.
    sf::Sprite TitleScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // CALCULATE INITIAL PARAMETERS FOR THE GAME'S TITLE.
        // The sub-heading is centered within the top third of the screen,
        // and the main title will be below that.  However, these two components
        // end up scrolling in from the top initially.
        MATH::FloatRectangle screen_rectangle = renderer.Screen->GetBoundingRectangle<float>();
        float screen_left_x_position = screen_rectangle.LeftTop.X;
        float screen_top_y_position = screen_rectangle.LeftTop.Y;
        float screen_width_in_pixels = screen_rectangle.Width();
        float screen_height_in_pixels = screen_rectangle.Height();
        float one_third_of_screen_height_in_pixels = screen_height_in_pixels / 3.0f;

        // CALCULATE THE OFFSET OF THE TITLE BASED ON ELAPSED TIME.
        // It should scroll in from the top.
        const sf::Time TITLE_TOTAL_SCROLL_IN_TIME = sf::seconds(1);
        float title_y_scroll_offset_in_pixels = 0.0f;
        bool title_still_scrolling_in = (ElapsedTime < TITLE_TOTAL_SCROLL_IN_TIME);
        if (title_still_scrolling_in)
        {
            // How far the title components have scrolled in from the top should be based on
            // how much of the total elapsed time has passed.  For easier placement in the
            // final positions, the scroll offset will be negative to place the title components
            // higher up on the screen while they're still scrolling in.
            float ratio_of_elapsed_time = ElapsedTime / TITLE_TOTAL_SCROLL_IN_TIME;
            float ratio_of_remaining_elapsed_time = 1.0f - ratio_of_elapsed_time;
            title_y_scroll_offset_in_pixels = -1.0f * one_third_of_screen_height_in_pixels * ratio_of_remaining_elapsed_time;
        }

        // DRAW THE SUB-HEADING FOR THE GAME'S TITLE.
        // It is centered within the top third of the screen once fully scrolled in.
        float sub_heading_top_y_position = screen_top_y_position + title_y_scroll_offset_in_pixels;
        MATH::FloatRectangle sub_heading_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            sub_heading_top_y_position,
            screen_width_in_pixels,
            one_third_of_screen_height_in_pixels);
        const float BIBLE_GAMES_HEADING_TEXT_SCALE = 1.5f;
        renderer.RenderCenteredText(
            "BIBLE GAMES",
            RESOURCES::AssetId::SERIF_FONT_TEXTURE,
            sub_heading_screen_rectangle,
            GRAPHICS::Color::WHITE,
            BIBLE_GAMES_HEADING_TEXT_SCALE);

        // DRAW THE MAIN GAME'S TITLE.
        // It is centered within the middle third of the screen once fully scrolled in.
        float main_title_top_y_position = one_third_of_screen_height_in_pixels + title_y_scroll_offset_in_pixels;
        MATH::FloatRectangle main_title_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            screen_left_x_position,
            main_title_top_y_position,
            screen_width_in_pixels,
            one_third_of_screen_height_in_pixels);
        const float TITLE_TEXT_SCALE = 3.0f;
        renderer.RenderCenteredText(
            "NOAH's ARK",
            RESOURCES::AssetId::SERIF_FONT_TEXTURE,
            main_title_screen_rectangle,
            GRAPHICS::Color::YELLOW,
            TITLE_TEXT_SCALE);

        // DRAW EACH OF THE MENU OPTIONS.
        float two_thirds_screen_height_in_pixels = 2 * one_third_of_screen_height_in_pixels;
        float current_menu_option_top_screen_position = two_thirds_screen_height_in_pixels;
        size_t menu_option_count = MenuOptions.size();
        for (size_t menu_option_index = 0; menu_option_index < menu_option_count; ++menu_option_index)
        {
            // CALCULATE THE SCREEN RECTANGLE FOR THE CURRENT MENU OPTION.
            float menu_option_screen_height = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
            MATH::FloatRectangle menu_option_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                screen_left_x_position,
                current_menu_option_top_screen_position,
                screen_width_in_pixels,
                menu_option_screen_height);

            // GET THE TEXT COLOR FOR THE CURRENT MENU OPTION.
            // The selected one should have a different color in order to stand out.
            GRAPHICS::Color menu_option_color = GRAPHICS::Color::RED;
            bool current_menu_option_is_selected = (menu_option_index == SelectedMenuOptionIndex);
            if (current_menu_option_is_selected)
            {
                // A lighter color is chosen for selected menu options since that
                // seemed more intuitive.
                menu_option_color = GRAPHICS::Color::ORANGE;
            }

            // GET THE TEXT FOR THE CURRENT MENU OPTION.
            std::string menu_option_text = "";

            if (current_menu_option_is_selected)
            {
                // To better help players see which menu option is selected,
                // some additional "arrow" text is rendered before the
                // main text for the selected menu option.
                // It is rendered separately to avoid having the length of the
                // menu options change, which results in shifting if they're
                // rendered as centered text.
                // It conveniently happens that 1/3 across the width of the screen
                // places this arrow pointer just to the left of all of the
                // current menu options.
                float one_third_screen_width_in_pixels = screen_width_in_pixels / 3.0f;
                float selected_menu_option_arrow_left_screen_position_in_pixels = (screen_left_x_position + one_third_screen_width_in_pixels);
                MATH::Vector2f selected_menu_option_arrow_left_top_screen_position_in_pixels(
                    selected_menu_option_arrow_left_screen_position_in_pixels,
                    current_menu_option_top_screen_position);
                const std::string SELECTED_MENU_OPTION_POINTER_TEXT = ">";
                renderer.RenderText(
                    SELECTED_MENU_OPTION_POINTER_TEXT,
                    RESOURCES::AssetId::FONT_TEXTURE,
                    selected_menu_option_arrow_left_top_screen_position_in_pixels,
                    menu_option_color);
            }

            GameState current_menu_option = MenuOptions.at(menu_option_index);
            switch (current_menu_option)
            {
                case GameState::GAME_SELECTION_SCREEN:
                    menu_option_text += "PLAY GAME";
                    break;
                case GameState::CREDITS_SCREEN:
                    menu_option_text += "CREDITS";
                    break;
            }

            // DRAW TEXT FOR THE CURRENT MENU OPTION.
            renderer.RenderCenteredText(
                menu_option_text,
                RESOURCES::AssetId::FONT_TEXTURE,
                menu_option_screen_rectangle,
                menu_option_color);

            // UPDATE THE POSITION FOR THE NEXT MENU OPTION.
            current_menu_option_top_screen_position += menu_option_screen_height;
        }        

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen();
        return screen;
    }
}