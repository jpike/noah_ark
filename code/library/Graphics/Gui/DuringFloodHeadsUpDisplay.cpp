#include <cmath>
#include "Gameplay/FloodElapsedTime.h"
#include "Graphics/Gui/DuringFloodHeadsUpDisplay.h"

namespace GRAPHICS::GUI
{
    /// Constructor.
    /// @param[in]  font - The font to use in the HUD.
    /// @param[in]  main_text_box_width_in_pixels - The width of the main text box, in pixels.
    /// @param[in]  main_text_box_height_in_pixels - The height of the main text box, in pixels.
    DuringFloodHeadsUpDisplay::DuringFloodHeadsUpDisplay(
        const std::shared_ptr<GRAPHICS::GUI::Font>& font,
        const unsigned int main_text_box_width_in_pixels,
        const unsigned int main_text_box_height_in_pixels) :
    MainTextBox(main_text_box_width_in_pixels, main_text_box_height_in_pixels, font),
    PauseMenuVisible(false),
    InventoryOpened(false),
    InventoryGui()
    {}

    /// Updates the HUD.
    /// @param[in]  current_game_data - The game data to display in the HUD.
    /// @param[in]  gaming_hardware - The gaming hardware supplying input.
    /// @return The state the game should be in after updating the HUD and any food type that should be dropped.
    std::tuple<STATES::GameState, OBJECTS::Food::TypeId> DuringFloodHeadsUpDisplay::Update(
        const STATES::SavedGameData& current_game_data,
        const HARDWARE::GamingHardware& gaming_hardware)
    {
        OBJECTS::Food::TypeId selected_food_type_to_drop = OBJECTS::Food::NONE;

        // CHECK IF THE PAUSE MENU IS OPEN.
        // If so, it should take precedence over other parts of the HUD.
        // After that, the main text box should take precedence over the inventory.
        if (PauseMenuVisible)
        {
            // CHECK IF AN APPLICABLE BUTTON WAS PRESSED.
            if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Return))
            {
                // SAVE THE GAME DATA.
                current_game_data.Write(current_game_data.Filepath);

                // CLOSE THE PAUSE MENU.
                PauseMenuVisible = false;
            }
            else if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::T))
            {
                // RETURN TO THE TITLE SCREEN.
                return std::make_tuple(STATES::GameState::TITLE_SCREEN, selected_food_type_to_drop);
            }
            else if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Escape))
            {
                // CLOSE THE PAUSE MENU.
                PauseMenuVisible = false;
            }
        }
        else if (MainTextBox.IsVisible)
        {
            // HAVE THE MAIN TEXT BOX RESPOND TO USER INPUT.
            if (gaming_hardware.InputController.ButtonDown(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
            {
                // CHECK IF THE TEXT BOX IS FINISHED DISPLAYING ITS CURRENT PAGE OF TEXT.
                // If the current page of text has not yet all been displayed, the next
                // page of text should not be moved to so that the user can finish
                // seeing the complete message.
                bool current_text_finished_being_displayed = MainTextBox.CurrentPageOfTextFinishedBeingDisplayed();
                if (current_text_finished_being_displayed)
                {
                    // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
                    MainTextBox.MoveToNextPage();
                }
            }

            // UPDATE THE TEXT BOX IF IT IS VISIBLE.
            // If the text box is currently being displayed, then it should capture any user input.
            MainTextBox.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
        }
        else
        {
            // CHECK IF THE SECONDARY ACTION BUTTON WAS PRESSED THIS FRAME.
            // To prevent rapid opening/closing of the inventory, the button
            // is checked to determine when it toggles to being pressed.
            bool inventory_button_pressed = gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::SECONDARY_ACTION_KEY);
            if (inventory_button_pressed)
            {
                // OPEN OR CLOSE THE INVENTORY.
                InventoryOpened = !InventoryOpened;
            }
            else if (InventoryOpened)
            {
                selected_food_type_to_drop = InventoryGui.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame, gaming_hardware.InputController);
            }
            else
            {
                // OPEN THE PAUSE MENU IF THE APPROPRIATE BUTTON WAS PRESSED.
                bool pause_menu_button_pressed = gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Escape);
                if (pause_menu_button_pressed)
                {
                    PauseMenuVisible = true;
                }
            }
        }

        // STAY ON THE CURRENT STATE.
        return std::make_tuple(current_game_data.CurrentGameState, selected_food_type_to_drop);
    }

    /// Renders the HUD to the provided target.
    /// @param[in]  current_game_data - The game data being visualized in the HUD.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void DuringFloodHeadsUpDisplay::Render(
        const STATES::SavedGameData& current_game_data,
        GRAPHICS::Renderer& renderer)
    {
        // RENDER TEXT INDICATING THE CURRENT DAY/TIME.
        unsigned int flood_day_count = 0;
        unsigned int current_day_hour = 0;
        GAMEPLAY::FloodElapsedTime::GetCurrentDayAndHour(current_game_data.FloodElapsedGameplayTime, flood_day_count, current_day_hour);
        std::string flood_day_and_time_text = 
            "Day: " + std::to_string(flood_day_count) +
            " Hour: " + std::to_string(current_day_hour);

        const MATH::Vector2f TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0.0f, 0.0f);
        // The main HUD text is rendered white to make it stand out against the black border inside the ark.
        const GRAPHICS::Color MAIN_TEXT_COLOR = GRAPHICS::Color::WHITE;
        renderer.RenderText(
            flood_day_and_time_text,
            RESOURCES::AssetId::FONT_TEXTURE,
            TOP_LEFT_SCREEN_POSITION_IN_PIXELS,
            MAIN_TEXT_COLOR);

        // RENDER ANY SPECIAL ACTION TEXT.
        bool special_action_available = (SpecialDayAction::NONE != CurrentSpecialDayAction) && (SpecialDayAction::CURRENT_DAY_ACTION_COMPLETED != CurrentSpecialDayAction);
        if (special_action_available)
        {
            // RENDER A BUTTON ICON FOR THE SPECIAL ACTION.
            constexpr char SPECIAL_ACTION_KEY_TEXT = INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY_TEXT;;
            // Screen position is somewhat hardcoded for simplicity.
            MATH::Vector2ui key_icon_top_left_screen_position(
                static_cast<unsigned int>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.X),
                static_cast<unsigned int>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
            key_icon_top_left_screen_position.X += 12 * Glyph::DEFAULT_HEIGHT_IN_PIXELS;
            renderer.RenderKeyIcon(SPECIAL_ACTION_KEY_TEXT, key_icon_top_left_screen_position);

            // GET THE TEXT FOR THE SPECIAL ACTION.
            std::string special_day_action_text;
            switch (CurrentSpecialDayAction)
            {
                case SpecialDayAction::SEND_OUT_RAVEN_FIRST_TIME:
                    special_day_action_text = "Send Raven";
                    break;
                case SpecialDayAction::SEND_OUT_DOVE_FIRST_TIME:
                case SpecialDayAction::SEND_OUT_DOVE_SECOND_TIME:
                case SpecialDayAction::SEND_OUT_DOVE_FINAL_TIME:
                    special_day_action_text = "Send Dove";
                    break;
            }

            // RENDER TEXT FOR THE SPECIAL ACTION.
            constexpr float KEY_ICON_WIDTH_IN_PIXELS = 16.0f;
            MATH::Vector2f special_action_text_left_top_position(
                static_cast<float>(key_icon_top_left_screen_position.X) + KEY_ICON_WIDTH_IN_PIXELS,
                static_cast<float>(key_icon_top_left_screen_position.Y));
            renderer.RenderText(
                special_day_action_text,
                RESOURCES::AssetId::FONT_TEXTURE,
                special_action_text_left_top_position,
                MAIN_TEXT_COLOR);
        }

        // RENDER COMPONENTS INDICATING HOW TO OPEN THE INVENTORY.
        // This text is rendered to the far-right of the screen so that its position isn't changed
        // if the space for other GUI elements (like the count of collected wood) changes such
        // that they could distractingly shift the position of this text.
        MATH::Vector2f TOP_RIGHT_SCREEN_POSITION_IN_PIXELS(
            renderer.Screen->WidthInPixels<float>(),
            TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        const std::string OPEN_INVENTORY_TEXT = "Inventory";
        // One glyph is rendered per character.
        const size_t OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS = (Glyph::DEFAULT_WIDTH_IN_PIXELS * OPEN_INVENTORY_TEXT.size());
        MATH::Vector2f open_inventory_text_top_left_screen_position_in_pixels = TOP_RIGHT_SCREEN_POSITION_IN_PIXELS;
        open_inventory_text_top_left_screen_position_in_pixels.X -= OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS;
        renderer.RenderText(
            OPEN_INVENTORY_TEXT, 
            RESOURCES::AssetId::FONT_TEXTURE, 
            open_inventory_text_top_left_screen_position_in_pixels, 
            MAIN_TEXT_COLOR);

        // An icon is rendered to help players know which key to press.  It is rendered after
        // the above text for the inventory since it is easier to correctly position here
        // such that it appears just to the left of the text.
        const char OPEN_INVENTORY_KEY = 'X';
        MATH::Vector2ui open_inventory_key_text_top_left_screen_position_in_pixels(
            static_cast<unsigned int>(open_inventory_text_top_left_screen_position_in_pixels.X),
            static_cast<unsigned int>(open_inventory_text_top_left_screen_position_in_pixels.Y));
        // Since a single character (glyph) is rendered for the key icon,
        // the width of the icon is the width of a single glyph.
        const unsigned int KEY_ICON_WIDTH_IN_PIXELS = Glyph::DEFAULT_WIDTH_IN_PIXELS;
        open_inventory_key_text_top_left_screen_position_in_pixels.X -= KEY_ICON_WIDTH_IN_PIXELS;
        renderer.RenderKeyIcon(OPEN_INVENTORY_KEY, open_inventory_key_text_top_left_screen_position_in_pixels);

        // RENDER THE INVENTORY GUI IF IT IS OPENED.
        if (InventoryOpened)
        {
            InventoryGui.Render(current_game_data, renderer);
        }

        // RENDER THE TEXT BOX IF IT IS VISIBLE.
        if (MainTextBox.IsVisible)
        {
            MainTextBox.Render(renderer);
        }

        // RENDER THE PAUSE MENUIF IT IS VISIBLE.
        if (PauseMenuVisible)
        {
            // The saved game filename is included in the text to clarify for users
            // what game is being saved.
            std::string saved_game_filename = current_game_data.Filepath.filename().string();
            std::string pause_menu_text = "[ENTER] - Save " + saved_game_filename + "\n[T] - Title\n\n[ESC] - Cancel";
            renderer.RenderCenteredText(
                pause_menu_text,
                RESOURCES::AssetId::FONT_TEXTURE,
                renderer.Screen->GetBoundingRectangle<float>());
        }
    }

    /// Checks if any modal GUI components of the HUD are currently displayed.
    /// @return True if a modal component of the HUD is displayed; false otherwise.
    bool DuringFloodHeadsUpDisplay::ModalComponentDisplayed() const
    {
        bool modal_component_displayed = InventoryOpened || PauseMenuVisible;
        return modal_component_displayed;
    }
}
