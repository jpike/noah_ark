#include <filesystem>
#include "Debugging/DebugConsole.h"
#include "Math/Number.h"
#include "Math/Rectangle.h"
#include "Resources/AssetId.h"
#include "States/GameSelectionScreen.h"

namespace STATES
{
    /// Loads any available saved games into the current state.
    void GameSelectionScreen::LoadSavedGames()
    {
        // CHECK IF THE SAVED GAMES FOLDER EXISTS.
        const std::filesystem::path SAVED_GAMES_FOLDER_PATH = "SavedGames";
        const bool saved_games_folder_exists = std::filesystem::is_directory(SAVED_GAMES_FOLDER_PATH);
        if (!saved_games_folder_exists)
        {
            // There are no saved games to load.
            return;
        }

        // TRY LOADING SAVED GAMES FROM EACH FILE IN THE SAVED GAMES FOLDER.
        for (const auto& file : std::filesystem::directory_iterator(SAVED_GAMES_FOLDER_PATH))
        {
            DEBUGGING::DebugConsole::WriteLine(file.path().string());

            // SKIP OVER ANY NON-FILES.
            const bool is_file = file.is_regular_file();
            if (!is_file)
            {
                continue;
            }

            // TRY LOADING A SAVED GAME FROM THE FILE.
            std::unique_ptr<SavedGameData> saved_game = SavedGameData::Load(file.path().string());
            if (saved_game)
            {
                SavedGames.push_back(std::move(saved_game));
            }
        }
    }

    /// Handles any user input for the game selection screen.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return The state the game should be in based on the user's input.
    GameState GameSelectionScreen::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // CHECK IF THE BACK BUTTON WAS PRESSED.
        bool back_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::BACK_KEY);
        if (back_button_pressed)
        {
            // RETURN TO THE TITLE SCREEN.
            return GameState::TITLE_SCREEN;
        }

        // CHECK IF THE USER PRESSED A BUTTON TO MOVE UP/DOWN IN THE LIST OF SAVED GAMES.
        constexpr std::size_t MIN_SELECTED_GAME_INDEX = 0;
        // The max valid selected game index is equal to the number of saved games
        // as the index equal to this size is reserved for creating a new game.
        std::size_t max_selected_game_index = SavedGames.size();
        bool up_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::UP_KEY);
        if (up_button_pressed)
        {
            // MOVE UP TO THE NEXT SAVED GAME.
            SelectedGameIndex = MATH::Number::DecrementAndWrapWithinRange(
                SelectedGameIndex,
                MIN_SELECTED_GAME_INDEX,
                max_selected_game_index);
        }

        bool down_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DOWN_KEY);
        if (down_button_pressed)
        {
            // MOVE DOWN TO THE NEXT SAVED GAME.
            SelectedGameIndex = MATH::Number::IncrementAndWrapWithinRange(
                SelectedGameIndex,
                MIN_SELECTED_GAME_INDEX,
                max_selected_game_index);
        }

        // CHECK IF THE MAIN 'START' BUTTON WAS PRESSED.
        bool start_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::START_KEY);
        if (start_button_pressed)
        {
            /// @todo   Check for if this is for a new game!
            return GameState::GAMEPLAY;
        }
        else
        {
            // INDICATE THE GAME SHOULD REMAIN ON THE GAME SELECTION SCREEN.
            return GameState::GAME_SELECTION_SCREEN;
        }
    }

    /// Renders the title screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void GameSelectionScreen::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER OPTIONS FOR EACH OF THE SAVED GAMES.
        const GRAPHICS::Color GAME_OPTION_TEXT_COLOR = GRAPHICS::Color::WHITE;
        const GRAPHICS::Color UNSELECTED_GAME_OPTION_BOX_BORDER_COLOR = GRAPHICS::Color::BLUE;
        constexpr float UNSELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS = 1.0f;
        // A lighter blue with thicker outline is used to indicate the currently selected saved game.
        const GRAPHICS::Color SELECTED_GAME_OPTION_BOX_BORDER_COLOR = GRAPHICS::Color(128, 128, 255);
        constexpr float SELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS = 4.0f;
        constexpr float GAME_SELECTION_BOX_SCREEN_SIDE_OFFSET_IN_PIXELS = 16.0f;
        float screen_width_in_pixels = renderer.Screen->WidthInPixels<float>();
        constexpr float APPLY_OFFSET_TO_BOTH_SIDES = 2.0f;
        float game_selection_box_width_in_pixels = 
            screen_width_in_pixels - 
            (APPLY_OFFSET_TO_BOTH_SIDES * GAME_SELECTION_BOX_SCREEN_SIDE_OFFSET_IN_PIXELS);
        constexpr float TEXT_ROW_COUNT_PER_GAME_SELECTION_BOX = 2.0f;
        float game_selection_box_height_in_pixels = TEXT_ROW_COUNT_PER_GAME_SELECTION_BOX * static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        // Because the number of saved games might exceed the height of the screen,
        // the starting y position for a given box for a saved game needs to be shifted
        // down based on the currently selected game index.
        constexpr float VERTICAL_SPACING_BETWEEN_GAME_OPTIONS_IN_PIXELS = 8.0f;
        float game_selection_box_height_with_padding_in_pixels = (
            game_selection_box_height_in_pixels + VERTICAL_SPACING_BETWEEN_GAME_OPTIONS_IN_PIXELS);
        // If the player went beyond the halfway point of saved games, the boxes should start
        // scrolling up to let the player more easily see additional saved games.
        float screen_height_in_pixels = renderer.Screen->HeightInPixels<float>();
        std::size_t max_selected_saved_games_per_screen = static_cast<std::size_t>(screen_height_in_pixels / game_selection_box_height_with_padding_in_pixels);
        std::size_t max_selected_saved_games_per_half_screen = max_selected_saved_games_per_screen / 2;
        float first_game_option_vertical_offset_in_pixels_based_on_scrolling_for_selected_game = GAME_SELECTION_BOX_SCREEN_SIDE_OFFSET_IN_PIXELS;
        bool player_scrolled_beyond_saved_games_in_top_half_of_screen = (SelectedGameIndex >= max_selected_saved_games_per_half_screen);
        if (player_scrolled_beyond_saved_games_in_top_half_of_screen)
        {
            constexpr float SCROLL_UP = -1.0f;
            std::size_t selected_game_index_offset_from_saved_game_count_per_half_screen = (
                SelectedGameIndex - max_selected_saved_games_per_half_screen);
            first_game_option_vertical_offset_in_pixels_based_on_scrolling_for_selected_game = (
                SCROLL_UP * 
                game_selection_box_height_with_padding_in_pixels * 
                selected_game_index_offset_from_saved_game_count_per_half_screen) 
                + 
                GAME_SELECTION_BOX_SCREEN_SIDE_OFFSET_IN_PIXELS;
        }
        MATH::FloatRectangle game_selection_option_bounding_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            GAME_SELECTION_BOX_SCREEN_SIDE_OFFSET_IN_PIXELS,
            first_game_option_vertical_offset_in_pixels_based_on_scrolling_for_selected_game,
            game_selection_box_width_in_pixels,
            game_selection_box_height_in_pixels);
        const MATH::Vector2f TEXT_OFFSET_FROM_BORDER_IN_PIXELS(0.0f, 0.0f);
        std::size_t saved_game_count = SavedGames.size();
        for (std::size_t saved_game_index = 0; saved_game_index < saved_game_count; ++saved_game_index)
        {
            // DETERMINE THE BORDER PROPERTIES FOR THE CURRENT SAVED GAME BOX.
            // The currently selected saved game box should stand out more.
            const GRAPHICS::Color* current_box_color = &UNSELECTED_GAME_OPTION_BOX_BORDER_COLOR;
            float current_box_border_thickness_in_pixels = UNSELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS;
            bool current_saved_game_is_selected = (saved_game_index == SelectedGameIndex);
            if (current_saved_game_is_selected)
            {
                current_box_color = &SELECTED_GAME_OPTION_BOX_BORDER_COLOR;
                current_box_border_thickness_in_pixels = SELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS;
            }

            // RENDER AN OPTION FOR THE CURRENT SAVED GAME
            const std::unique_ptr<SavedGameData>& saved_game = SavedGames.at(saved_game_index);
            std::string saved_game_filename = saved_game->Filepath.filename().string();
            renderer.RenderTextInBorderedBox(
                saved_game_filename,
                RESOURCES::AssetId::FONT_TEXTURE,
                GAME_OPTION_TEXT_COLOR,
                TEXT_OFFSET_FROM_BORDER_IN_PIXELS,
                game_selection_option_bounding_screen_rectangle,
                *current_box_color,
                current_box_border_thickness_in_pixels);

            // MOVE DOWN THE RECTANGLE FOR THE NEXT GAME OPTION.
            MATH::Vector2f next_game_option_offset_in_pixels(0.0f, game_selection_box_height_in_pixels);
            next_game_option_offset_in_pixels.Y += VERTICAL_SPACING_BETWEEN_GAME_OPTIONS_IN_PIXELS;
            game_selection_option_bounding_screen_rectangle.Move(next_game_option_offset_in_pixels);
        }

        // RENDER AN OPTION FOR A NEW GAME.
        // If no other saved game is selected, then this box should stand out.
        const GRAPHICS::Color* new_game_box_color = &UNSELECTED_GAME_OPTION_BOX_BORDER_COLOR;
        float new_game_box_border_thickness_in_pixels = UNSELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS;
        bool new_game_box_selected = (saved_game_count == SelectedGameIndex);
        if (new_game_box_selected)
        {
            new_game_box_color = &SELECTED_GAME_OPTION_BOX_BORDER_COLOR;
            new_game_box_border_thickness_in_pixels = SELECTED_GAME_OPTION_BOX_BORDER_THICKNESS_IN_PIXELS;
        }
        renderer.RenderTextInBorderedBox(
            "NEW GAME",
            RESOURCES::AssetId::FONT_TEXTURE,
            GAME_OPTION_TEXT_COLOR,
            TEXT_OFFSET_FROM_BORDER_IN_PIXELS,
            game_selection_option_bounding_screen_rectangle,
            *new_game_box_color,
            new_game_box_border_thickness_in_pixels);
    }
}
