#include "Graphics/Gui/PreFloodHeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "States/SavedGameData.h"

namespace GRAPHICS::GUI
{
    /// Constructor.
    /// @param[in]  font - The font to use in the HUD.
    /// @param[in]  main_text_box_width_in_pixels - The width of the main text box, in pixels.
    /// @param[in]  main_text_box_height_in_pixels - The height of the main text box, in pixels.
    PreFloodHeadsUpDisplay::PreFloodHeadsUpDisplay(
        const std::shared_ptr<GRAPHICS::GUI::Font>& font,
        const unsigned int main_text_box_width_in_pixels,
        const unsigned int main_text_box_height_in_pixels) :
    MainTextBox(main_text_box_width_in_pixels, main_text_box_height_in_pixels, font),
    AxeIconVisible(true),
    PauseMenuVisible(false),
    InventoryOpened(false),
    InventoryGui()
    {}

    /// Updates the HUD.
    /// @param[in]  current_game_data - The game data to display in the HUD.
    /// @param[in]  gaming_hardware - The gaming hardware supplying input.
    /// @return The state the game should be in after updating the HUD.
    STATES::GameState PreFloodHeadsUpDisplay::Update(
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
    /// @param[in]  main_text_color - The color for text in the main HUD.  This doesn't affect
    ///     text displayed in the main text box or the inventory GUI but only other text
    ///     displayed directly by this HUD.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void PreFloodHeadsUpDisplay::Render(
        const STATES::SavedGameData& current_game_data,
        const GRAPHICS::Color& main_text_color,
        GRAPHICS::Renderer& renderer)
    {
        // RENDER COMPONENTS INDICATING HOW TO SWING THE AXE.
        // An icon is rendered to help players know which key to press.
        const char SWING_AXE_KEY = INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY_TEXT;
        const MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
        // The axe icon should only be visible in some circumstances since the axe can only be used in some circumstances.
        // However, various computations above are still needed for positioning of other HUD components.
        if (AxeIconVisible)
        {
            renderer.RenderKeyIcon(SWING_AXE_KEY, TOP_LEFT_SCREEN_POSITION_IN_PIXELS);
        }

        // An axe icon is rendered to help players know what the previously rendered key icon is for.
        const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
        const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
        const float AXE_WIDTH_IN_PIXELS = 11.0f;
        const float AXE_HEIGHT_IN_PIXELS = 14.0f;
        const MATH::FloatRectangle AXE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(
            AXE_SPRITE_X_OFFSET_IN_PIXELS,
            AXE_SPRITE_Y_OFFSET_IN_PIXELS,
            AXE_WIDTH_IN_PIXELS,
            AXE_HEIGHT_IN_PIXELS);
        // The axe icon should be positioned just to the right of its key icon.
        // Since a single character (glyph) is rendered for the key icon,
        // the width of the icon is the width of a single glyph.
        const unsigned int KEY_ICON_WIDTH_IN_PIXELS = Glyph::DEFAULT_WIDTH_IN_PIXELS;
        MATH::Vector2ui axe_icon_screen_position = TOP_LEFT_SCREEN_POSITION_IN_PIXELS;
        axe_icon_screen_position.X += KEY_ICON_WIDTH_IN_PIXELS;

        // The axe icon should only be visible in some circumstances since the axe can only be used in some circumstances.
        // However, various computations above are still needed for positioning of other HUD components.
        if (AxeIconVisible)
        {
            renderer.RenderGuiIcon(RESOURCES::AssetId::AXE_TEXTURE, AXE_TEXTURE_SUB_RECTANGLE, axe_icon_screen_position);
        }

        // RENDER COMPONENTS INDICATING HOW MUCH WOOD HAS BEEN COLLECTED.
        // A wood icon is rendered to help players know what the text next to it corresponds to.
        const MATH::FloatRectangle WOOD_LOG_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(
            32.0f, 32.0f, 16.0f, 16.0f);
        // The wood icon should be next to the axe icon but with a little additional blank space
        // so that the two icons aren't too close together.
        const unsigned int PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON = 16;
        MATH::Vector2ui wood_icon_screen_position = axe_icon_screen_position;
        wood_icon_screen_position.X += static_cast<unsigned int>(AXE_TEXTURE_SUB_RECTANGLE.Width());
        wood_icon_screen_position.X += PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON;

        renderer.RenderGuiIcon(RESOURCES::AssetId::WOOD_LOG_TEXTURE, WOOD_LOG_TEXTURE_SUB_RECTANGLE, wood_icon_screen_position);

        // Text is rendered for "x#" to communicate how much wood has been collected.
        // For example, "x10" (no quotes) would be rendered if the player has collected
        // 10 wood logs.
        const std::string TIMES_COUNT_TEXT = "x";
        std::string wood_count_string = TIMES_COUNT_TEXT + std::to_string(current_game_data.Player->Inventory.WoodCount);
        // This text should be placed just to the right of the wood icon.
        MATH::Vector2f wood_text_top_left_screen_position_in_pixels(
            static_cast<float>(wood_icon_screen_position.X), 
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
        wood_text_top_left_screen_position_in_pixels.X += WOOD_LOG_TEXTURE_SUB_RECTANGLE.Width();
        renderer.RenderText(wood_count_string, RESOURCES::AssetId::FONT_TEXTURE, wood_text_top_left_screen_position_in_pixels, main_text_color);

        // RENDER SOME TEXT INDICATING COUNT OF FAMILY MEMBERS GATHERED.
        unsigned int family_member_gathered_count = 0;
        for (const bool family_member_gathered : current_game_data.FamilyMembersGathered)
        {
            if (family_member_gathered)
            {
                ++family_member_gathered_count;
            }
        }
        std::string family_count_string = "Family: " + std::to_string(family_member_gathered_count);

        // In order to keep this text from shifting around, it is placed a fixed amount to the right of the
        // wood count text above.  This has the potential to result in the wood count overlapping, but that
        // is an unlikely scenario (and the position would conflict with other items in the HUD anyway).
        constexpr float AXE_ICON_APPROXIMATE_WIDTH_IN_PIXELS = 2 * Glyph::DEFAULT_WIDTH_IN_PIXELS;
        constexpr float WOOD_COUNT_APPROXIMATE_WIDTH_IN_PIXELS = 6 * Glyph::DEFAULT_WIDTH_IN_PIXELS;
        constexpr float FAMILY_TEXT_TOP_LEFT_SCREEN_POSITION_IN_PIXELS = (
            AXE_ICON_APPROXIMATE_WIDTH_IN_PIXELS +
            WOOD_COUNT_APPROXIMATE_WIDTH_IN_PIXELS);
        
        MATH::Vector2f family_text_top_left_screen_position_in_pixels(
            FAMILY_TEXT_TOP_LEFT_SCREEN_POSITION_IN_PIXELS,
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));

        renderer.RenderText(family_count_string, RESOURCES::AssetId::FONT_TEXTURE, family_text_top_left_screen_position_in_pixels, main_text_color);

        // RENDER COMPONENTS INDICATING HOW TO OPEN THE INVENTORY.
        // This text is rendered to the far-right of the screen so that its position isn't changed
        // if the space for other GUI elements (like the count of collected wood) changes such
        // that they could distractingly shift the position of this text.
        MATH::Vector2f TOP_RIGHT_SCREEN_POSITION_IN_PIXELS(
            renderer.Screen->WidthInPixels<float>(),
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
        const std::string OPEN_INVENTORY_TEXT = "Inventory";
        // One glyph is rendered per character.
        const size_t OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS = (Glyph::DEFAULT_WIDTH_IN_PIXELS * OPEN_INVENTORY_TEXT.size());
        MATH::Vector2f open_inventory_text_top_left_screen_position_in_pixels = TOP_RIGHT_SCREEN_POSITION_IN_PIXELS;
        open_inventory_text_top_left_screen_position_in_pixels.X -= OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS;
        renderer.RenderText(
            OPEN_INVENTORY_TEXT, 
            RESOURCES::AssetId::FONT_TEXTURE, 
            open_inventory_text_top_left_screen_position_in_pixels, 
            main_text_color);

        // An icon is rendered to help players know which key to press.  It is rendered after
        // the above text for the inventory since it is easier to correctly position here
        // such that it appears just to the left of the text.
        const char OPEN_INVENTORY_KEY = 'X';
        MATH::Vector2ui open_inventory_key_text_top_left_screen_position_in_pixels(
            static_cast<unsigned int>(open_inventory_text_top_left_screen_position_in_pixels.X),
            static_cast<unsigned int>(open_inventory_text_top_left_screen_position_in_pixels.Y));
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
    bool PreFloodHeadsUpDisplay::ModalComponentDisplayed() const
    {
        bool modal_component_displayed = InventoryOpened || PauseMenuVisible;
        return modal_component_displayed;
    }
}
