#include "Graphics/Color.h"
#include "States/TitleScreen.h"

namespace STATES
{
    /// Constructor.
    TitleScreen::TitleScreen() :
        SelectedMenuOptionIndex(0),
        MenuOptions({ GameState::PRE_FLOOD_GAMEPLAY, GameState::CREDITS_SCREEN })
    {}

    /// Handles any user input for the title screen.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return The state the game should be in based on the user's input.
    GameState TitleScreen::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
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
    void TitleScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // DRAW THE SUB-HEADING FOR THE GAME'S TITLE.
        // It is centered within the top third of the screen.
        MATH::FloatRectangle screen_rectangle = renderer.Screen->GetBoundingRectangle<float>();
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

        // DRAW EACH OF THE MENU OPTIONS.
        float two_thirds_screen_height_in_pixels = 2 * one_third_of_screen_height_in_pixels;
        float current_menu_option_top_screen_position = two_thirds_screen_height_in_pixels;
        size_t menu_option_count = MenuOptions.size();
        for (size_t menu_option_index = 0; menu_option_index < menu_option_count; ++menu_option_index)
        {
            // CALCULATE THE SCREEN RECTANGLE FOR THE CURRENT MENU OPTION.
            float menu_option_screen_height = static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
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
                // some additional "arrow" text is inserted before the
                // main text for the selected menu option.
                menu_option_text += "> ";
            }

            GameState current_menu_option = MenuOptions.at(menu_option_index);
            switch (current_menu_option)
            {
                case GameState::PRE_FLOOD_GAMEPLAY:
                    menu_option_text += "PLAY GAME";
                    break;
                case GameState::CREDITS_SCREEN:
                    menu_option_text += "CREDITS";
                    break;
            }

            // DRAW TEXT FOR THE CURRENT MENU OPTION.
            renderer.RenderCenteredText(
                menu_option_text,
                menu_option_screen_rectangle,
                menu_option_color);

            // UPDATE THE POSITION FOR THE NEXT MENU OPTION.
            current_menu_option_top_screen_position += menu_option_screen_height;
        }        
    }
}