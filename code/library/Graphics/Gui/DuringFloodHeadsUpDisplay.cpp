#include <cmath>
#include "Gameplay/FloodElapsedTime.h"
#include "Graphics/Gui/DuringFloodHeadsUpDisplay.h"

namespace GRAPHICS::GUI
{
    /// Updates the HUD.
    /// @param[in]  current_game_data - The game data to display in the HUD.
    /// @param[in]  gaming_hardware - The gaming hardware supplying input.
    /// @return The state the game should be in after updating the HUD.
    STATES::GameState DuringFloodHeadsUpDisplay::Update(
        const STATES::SavedGameData& current_game_data,
        const HARDWARE::GamingHardware& gaming_hardware)
    {
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
                return STATES::GameState::TITLE_SCREEN;
            }
            else if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Escape))
            {
                // CLOSE THE PAUSE MENU.
                PauseMenuVisible = false;
            }
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
                InventoryGui.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame, gaming_hardware.InputController);
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
        return current_game_data.CurrentGameState;
    }

    /// Renders the HUD to the provided target.
    /// @param[in]  current_game_data - The game data being visualized in the HUD.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void DuringFloodHeadsUpDisplay::Render(
        const STATES::SavedGameData& current_game_data,
        GRAPHICS::Renderer& renderer) const
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
