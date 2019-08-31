#include <algorithm>
#include <filesystem>
#include "Debugging/DebugConsole.h"
#include "Math/Number.h"
#include "Math/Rectangle.h"
#include "Resources/AssetId.h"
#include "States/GameSelectionScreen.h"

namespace STATES
{
    const std::filesystem::path GameSelectionScreen::SAVED_GAMES_FOLDER_PATH = "SavedGames";

    /// Loads any available saved games into the current state.
    void GameSelectionScreen::LoadSavedGames()
    {
        // CHECK IF THE SAVED GAMES FOLDER EXISTS.
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
            std::shared_ptr<SavedGameData> saved_game = SavedGameData::Load(file.path().string());
            if (saved_game)
            {
                SavedGames.push_back(saved_game);
            }
        }
    }

    /// Handles any user input for the game selection screen.
    /// @param[in]  input_controller - The controller supplying user input to respond to.
    /// @return The state the game should be in based on the user's input.
    GameState GameSelectionScreen::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // RESPOND TO INPUT BASED ON THE CURRENT STATE.
        switch (CurrentSubState)
        {
            case SubState::LISTING_GAMES:
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
                    // CHECK IF A NEW GAME IS BEING CHOSEN.
                    std::size_t saved_game_count = SavedGames.size();
                    bool new_game_box_selected = (saved_game_count == SelectedGameIndex);
                    if (new_game_box_selected)
                    {
                        // SWITCH TO ENTERING A NEW GAME NAME.
                        CurrentNewGameFilenameText.clear();
                        CurrentSubState = SubState::ENTERING_NEW_GAME;
                        return GameState::GAME_SELECTION_SCREEN;
                    }
                    else
                    {
                        return GameState::GAMEPLAY;
                    }
                }
                else
                {
                    // INDICATE THE GAME SHOULD REMAIN ON THE GAME SELECTION SCREEN.
                    return GameState::GAME_SELECTION_SCREEN;
                }
                break;
            }
            case SubState::ENTERING_NEW_GAME:
            {
                // CHECK IF THE MAIN 'START' BUTTON WAS PRESSED.
                bool start_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::START_KEY);
                if (start_button_pressed)
                {
                    // START THE GAME IF AT LEAST ONE CHARACTER EXISTS IN THE FILENAME.
                    if (CurrentNewGameFilenameText.empty())
                    {
                        return GameState::GAME_SELECTION_SCREEN;
                    }
                    else
                    {
                        // CREATE THE NEW SAVED GAME DATA.
                        std::shared_ptr<SavedGameData> new_saved_game = std::make_shared<SavedGameData>(SavedGameData::DefaultSavedGameData());
                        new_saved_game->Filepath = SAVED_GAMES_FOLDER_PATH / CurrentNewGameFilenameText;
                        SavedGames.push_back(new_saved_game);
                        return GameState::GAMEPLAY;
                    }
                }

                // CHECK IF THE BACK BUTTON WAS PRESSED.
                bool back_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::BACK_KEY);
                if (back_button_pressed)
                {
                    // SWITCH BACK TO LISTING OTHER SAVED GAMES.
                    CurrentSubState = SubState::LISTING_GAMES;
                    return GameState::GAME_SELECTION_SCREEN;
                }

                // CHECK IF THE BACKSPACE KEY WAS PRESSED.
                bool backspace_key_pressed = input_controller.ButtonWasPressed(sf::Keyboard::Backspace);
                if (backspace_key_pressed)
                {
                    // DELETE A CHARACTER FROM THE FILENAME IF ONE EXISTS.
                    bool new_game_filename_populated = !CurrentNewGameFilenameText.empty();
                    if (new_game_filename_populated)
                    {
                        CurrentNewGameFilenameText.pop_back();
                    }
                }
                // GET ANY VALID FILENAME CHARACTERS THAT HAVE BEEN PRESSED.
                std::vector<sf::Keyboard::Key> pressed_keys = input_controller.GetTypedKeys();
                static const std::vector<sf::Keyboard::Key> valid_filename_keys = 
                {
                    sf::Keyboard::A,        
                    sf::Keyboard::B,            
                    sf::Keyboard::C,            
                    sf::Keyboard::D,            
                    sf::Keyboard::E,            
                    sf::Keyboard::F,            
                    sf::Keyboard::G,            
                    sf::Keyboard::H,            
                    sf::Keyboard::I,            
                    sf::Keyboard::J,            
                    sf::Keyboard::K,            
                    sf::Keyboard::L,            
                    sf::Keyboard::M,            
                    sf::Keyboard::N,            
                    sf::Keyboard::O,            
                    sf::Keyboard::P,            
                    sf::Keyboard::Q,            
                    sf::Keyboard::R,            
                    sf::Keyboard::S,            
                    sf::Keyboard::T,            
                    sf::Keyboard::U,            
                    sf::Keyboard::V,            
                    sf::Keyboard::W,            
                    sf::Keyboard::X,            
                    sf::Keyboard::Y,            
                    sf::Keyboard::Z,            
                    sf::Keyboard::Num0,         
                    sf::Keyboard::Num1,         
                    sf::Keyboard::Num2,         
                    sf::Keyboard::Num3,         
                    sf::Keyboard::Num4,         
                    sf::Keyboard::Num5,         
                    sf::Keyboard::Num6,         
                    sf::Keyboard::Num7,         
                    sf::Keyboard::Num8,         
                    sf::Keyboard::Num9,         
                    sf::Keyboard::Period,       
                    sf::Keyboard::Hyphen,       
                    sf::Keyboard::Numpad0,      
                    sf::Keyboard::Numpad1,      
                    sf::Keyboard::Numpad2,      
                    sf::Keyboard::Numpad3,      
                    sf::Keyboard::Numpad4,      
                    sf::Keyboard::Numpad5,      
                    sf::Keyboard::Numpad6,      
                    sf::Keyboard::Numpad7,      
                    sf::Keyboard::Numpad8,      
                    sf::Keyboard::Numpad9,      
                };
                std::vector<sf::Keyboard::Key> valid_pressed_filename_keys;
                std::set_intersection(
                    pressed_keys.cbegin(),
                    pressed_keys.cend(),
                    valid_filename_keys.cbegin(),
                    valid_filename_keys.cend(),
                    std::back_inserter(valid_pressed_filename_keys));

                // CONVERT PRESSED FILENAME CHARACTERS TO ACTUAL TEXT.
                static const std::map<sf::Keyboard::Key, char> FILENAME_KEY_TO_LOWERCASE_CHARACTER_LOOKUP =
                {
                    { sf::Keyboard::A, 'a' },
                    { sf::Keyboard::B, 'b' },
                    { sf::Keyboard::C, 'c' },
                    { sf::Keyboard::D, 'd' },
                    { sf::Keyboard::E, 'e' },
                    { sf::Keyboard::F, 'f' },
                    { sf::Keyboard::G, 'g' },
                    { sf::Keyboard::H, 'h' },
                    { sf::Keyboard::I, 'i' },
                    { sf::Keyboard::J, 'j' },
                    { sf::Keyboard::K, 'k' },
                    { sf::Keyboard::L, 'l' },
                    { sf::Keyboard::M, 'm' },
                    { sf::Keyboard::N, 'n' },
                    { sf::Keyboard::O, 'o' },
                    { sf::Keyboard::P, 'p' },
                    { sf::Keyboard::Q, 'q' },
                    { sf::Keyboard::R, 'r' },
                    { sf::Keyboard::S, 's' },
                    { sf::Keyboard::T, 't' },
                    { sf::Keyboard::U, 'u' },
                    { sf::Keyboard::V, 'v' },
                    { sf::Keyboard::W, 'w' },
                    { sf::Keyboard::X, 'x' },
                    { sf::Keyboard::Y, 'y' },
                    { sf::Keyboard::Z, 'z' },
                    { sf::Keyboard::Num0, '0' },
                    { sf::Keyboard::Num1, '1' },
                    { sf::Keyboard::Num2, '2' },
                    { sf::Keyboard::Num3, '3' },
                    { sf::Keyboard::Num4, '4' },
                    { sf::Keyboard::Num5, '5' },
                    { sf::Keyboard::Num6, '6' },
                    { sf::Keyboard::Num7, '7' },
                    { sf::Keyboard::Num8, '8' },
                    { sf::Keyboard::Num9, '9' },
                    { sf::Keyboard::Period, '.' },
                    { sf::Keyboard::Hyphen, '-' },
                    { sf::Keyboard::Numpad0, '0' },
                    { sf::Keyboard::Numpad1, '1' },
                    { sf::Keyboard::Numpad2, '2' },
                    { sf::Keyboard::Numpad3, '3' },
                    { sf::Keyboard::Numpad4, '4' },
                    { sf::Keyboard::Numpad5, '5' },
                    { sf::Keyboard::Numpad6, '6' },
                    { sf::Keyboard::Numpad7, '7' },
                    { sf::Keyboard::Numpad8, '8' },
                    { sf::Keyboard::Numpad9, '9' },
                };
                static const std::map<sf::Keyboard::Key, char> FILENAME_KEY_TO_UPPERCASE_CHARACTER_LOOKUP =
                {
                    { sf::Keyboard::A, 'A' },
                    { sf::Keyboard::B, 'B' },
                    { sf::Keyboard::C, 'C' },
                    { sf::Keyboard::D, 'D' },
                    { sf::Keyboard::E, 'E' },
                    { sf::Keyboard::F, 'F' },
                    { sf::Keyboard::G, 'G' },
                    { sf::Keyboard::H, 'H' },
                    { sf::Keyboard::I, 'I' },
                    { sf::Keyboard::J, 'J' },
                    { sf::Keyboard::K, 'K' },
                    { sf::Keyboard::L, 'L' },
                    { sf::Keyboard::M, 'M' },
                    { sf::Keyboard::N, 'N' },
                    { sf::Keyboard::O, 'O' },
                    { sf::Keyboard::P, 'P' },
                    { sf::Keyboard::Q, 'Q' },
                    { sf::Keyboard::R, 'R' },
                    { sf::Keyboard::S, 'S' },
                    { sf::Keyboard::T, 'T' },
                    { sf::Keyboard::U, 'U' },
                    { sf::Keyboard::V, 'V' },
                    { sf::Keyboard::W, 'W' },
                    { sf::Keyboard::X, 'X' },
                    { sf::Keyboard::Y, 'Y' },
                    { sf::Keyboard::Z, 'Z' },
                    // Uppercase versions of numbers aren't supported.
                    { sf::Keyboard::Num0, '0' },
                    { sf::Keyboard::Num1, '1' },
                    { sf::Keyboard::Num2, '2' },
                    { sf::Keyboard::Num3, '3' },
                    { sf::Keyboard::Num4, '4' },
                    { sf::Keyboard::Num5, '5' },
                    { sf::Keyboard::Num6, '6' },
                    { sf::Keyboard::Num7, '7' },
                    { sf::Keyboard::Num8, '8' },
                    { sf::Keyboard::Num9, '9' },
                    // Uppercase versions of the period isn't supported.
                    { sf::Keyboard::Period, '.' },
                    { sf::Keyboard::Hyphen, '_' },
                    // Uppercase versions of numbers aren't supported.
                    { sf::Keyboard::Numpad0, '0' },
                    { sf::Keyboard::Numpad1, '1' },
                    { sf::Keyboard::Numpad2, '2' },
                    { sf::Keyboard::Numpad3, '3' },
                    { sf::Keyboard::Numpad4, '4' },
                    { sf::Keyboard::Numpad5, '5' },
                    { sf::Keyboard::Numpad6, '6' },
                    { sf::Keyboard::Numpad7, '7' },
                    { sf::Keyboard::Numpad8, '8' },
                    { sf::Keyboard::Numpad9, '9' },
                };
                for (sf::Keyboard::Key key : valid_pressed_filename_keys)
                {
                    // GET THE APPROPRIATE VERSION OF THE CURRENT KEY'S CHARACTER.
                    char current_key_character = ' ';
                    bool left_shift_key_down = input_controller.ButtonDown(sf::Keyboard::LShift);
                    bool right_shift_key_down = input_controller.ButtonDown(sf::Keyboard::RShift);
                    bool shift_down = (left_shift_key_down || right_shift_key_down);
                    if (shift_down)
                    {
                        current_key_character = FILENAME_KEY_TO_UPPERCASE_CHARACTER_LOOKUP.at(key);
                    }
                    else
                    {
                        current_key_character = FILENAME_KEY_TO_LOWERCASE_CHARACTER_LOOKUP.at(key);
                    }

                    // ADD THE CURRENT KEY'S CHARACTER IF THERE'S ENOUGH ROOM.
                    // The length is largely arbitrary but currently set to not result in
                    // the filename wrapping onto multiple lines.  One thought was to enforce
                    // an old "8.3" file naming convention, but that was discarded for now
                    // since it could be confusing due to all of the extra width on screen.
                    constexpr std::size_t MAX_FILENAME_LENGTH_IN_CHARACTERS = 30;
                    bool filename_has_more_room = (CurrentNewGameFilenameText.length() < MAX_FILENAME_LENGTH_IN_CHARACTERS);
                    if (filename_has_more_room)
                    {
                        CurrentNewGameFilenameText += current_key_character;
                    }
                }

                return GameState::GAME_SELECTION_SCREEN;
            }
            default:
                // STAY ON THE CURRENT SCREEN.
                return GameState::GAME_SELECTION_SCREEN;
        }
    }

    /// Renders the game selection screen.
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
            const std::shared_ptr<SavedGameData>& saved_game = SavedGames.at(saved_game_index);
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

        // The text for the new game needs to differ based on the current substate.
        std::string new_game_box_text;
        switch (CurrentSubState)
        {
            case SubState::LISTING_GAMES:
            {
                // The player should be shown that they can create a new game.
                new_game_box_text = "NEW GAME";
                break;
            }
            case SubState::ENTERING_NEW_GAME:
            {
                // The player should be shown the text they have currently entered.
                new_game_box_text = CurrentNewGameFilenameText;
                break;
            }
        }
        renderer.RenderTextInBorderedBox(
            new_game_box_text,
            RESOURCES::AssetId::FONT_TEXTURE,
            GAME_OPTION_TEXT_COLOR,
            TEXT_OFFSET_FROM_BORDER_IN_PIXELS,
            game_selection_option_bounding_screen_rectangle,
            *new_game_box_color,
            new_game_box_border_thickness_in_pixels);
    }
}