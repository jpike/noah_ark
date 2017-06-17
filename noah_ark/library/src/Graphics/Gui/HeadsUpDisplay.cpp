#include <cassert>
#include "Core/NullChecking.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "States/SavedGameData.h"

namespace GRAPHICS
{
namespace GUI
{
    /// Constructor.
    /// @param[in]  overworld - The overworld whose information is being diplayed in the HUD.
    /// @param[in]  main_text_box_width_in_pixels - The width of the main text box, in pixels.
    /// @param[in]  main_text_box_height_in_pixels - The height of the main text box, in pixels.
    /// @param[in]  assets - The assets to use for the page.
    /// @throws std::exception - Thrown if a parameter is null.
    HeadsUpDisplay::HeadsUpDisplay(
        const std::shared_ptr<MAPS::Overworld>& overworld,
        const unsigned int main_text_box_width_in_pixels,
        const unsigned int main_text_box_height_in_pixels,
        const std::shared_ptr<RESOURCES::Assets>& assets) :
    MainTextBox(main_text_box_width_in_pixels, main_text_box_height_in_pixels),
    InventoryOpened(false),
    InventoryGui(overworld->NoahPlayer->Inventory, assets),
    SaveDialogBoxVisible(false),
    Assets(assets),
    Overworld(overworld)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Assets,
            "Null assets provided to HUD.");
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Overworld,
            "Null overworld provided to HUD.");
    }

    /// Has the HUD respond to the provided user input.
    /// @param[in]  input_controller - The controller on which to check user input.
    void HeadsUpDisplay::RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller)
    {
        // CHECK IF THE SAVE DIALOG BOX IS OPEN.
        // If so, it should take precedence over other parts of the HUD.
        // After that, the main text box should take precedence over the inventory.
        if (SaveDialogBoxVisible)
        {
            // CHECK IF AN APPLICABLE BUTTON WAS PRESSED.
            if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::START_KEY))
            {
                // SAVE THE GAME DATA.
                STATES::SavedGameData saved_game_data;
                saved_game_data.PlayerWorldPosition = Overworld->NoahPlayer->GetWorldPosition();
                saved_game_data.WoodCount = Overworld->NoahPlayer->Inventory->WoodCount;
                saved_game_data.FoundBibleVerses = std::vector<BIBLE::BibleVerse>(
                    Overworld->NoahPlayer->Inventory->BibleVerses.cbegin(),
                    Overworld->NoahPlayer->Inventory->BibleVerses.cend());
                
                // Built ark piece data from all tile maps needs to be included.
                unsigned int tile_map_row_count = Overworld->TileMaps.GetHeight();
                unsigned int tile_map_column_count = Overworld->TileMaps.GetWidth();
                for (unsigned int tile_map_y_index = 0; tile_map_y_index < tile_map_row_count; ++tile_map_y_index)
                {
                    for (unsigned int tile_map_x_index = 0; tile_map_x_index < tile_map_column_count; ++tile_map_x_index)
                    {
                        // GATHER ANY ARK PIECES BUILT IN THE CURRENT TILE MAP.
                        const MAPS::TileMap* current_tile_map = Overworld->GetTileMap(tile_map_y_index, tile_map_x_index);
                        assert(current_tile_map);
                        std::vector<size_t> built_ark_piece_indices;
                        size_t ark_piece_count = current_tile_map->ArkPieces.size();
                        for (size_t ark_piece_index = 0; ark_piece_index < ark_piece_count; ++ark_piece_index)
                        {
                            // CHECK IF THE CURRENT ARK PIECE IS BUILT.
                            const auto& ark_piece = current_tile_map->ArkPieces.at(ark_piece_index);
                            if (ark_piece.Built)
                            {
                                // STORE THE BUILT ARK PIECE INDEX.
                                built_ark_piece_indices.push_back(ark_piece_index);
                            }
                        }

                        // CHECK IF THE CURRENT TILE MAP HAS ANY BUILT ARK PIECES.
                        // This prevents unnecessary data from being stored in the file.
                        bool ark_pieces_built_in_map = !built_ark_piece_indices.empty();
                        if (ark_pieces_built_in_map)
                        {
                            // ADD THE CURRENT TILE MAP'S BUILT ARK PIECES.
                            STATES::BuiltArkPieceTileMapData tile_map_ark_data;
                            tile_map_ark_data.TileMapGridXPosition = tile_map_x_index;
                            tile_map_ark_data.TileMapGridYPosition = tile_map_y_index;
                            tile_map_ark_data.BuiltArkPieceIndices = built_ark_piece_indices;
                            saved_game_data.BuildArkPieces.push_back(tile_map_ark_data);
                        }
                    }
                }

                saved_game_data.CollectedAnimals = Overworld->NoahPlayer->Inventory->CollectedAnimalCounts;

                saved_game_data.Write(STATES::SavedGameData::DEFAULT_FILENAME);

                // CLOSE THE SAVE DIALOG BOX.
                SaveDialogBoxVisible = false;
            }
            else if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::BACK_KEY))
            {
                // CLOSE THE SAVE DIALOG BOX.
                SaveDialogBoxVisible = false;
            }
        }
        else if (MainTextBox.IsVisible)
        {
            // HAVE THE MAIN TEXT BOX RESPOND TO USER INPUT.
            if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::PRIMARY_ACTION_KEY))
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
        }
        else
        {
            // CHECK IF THE SECONDARY ACTION BUTTON WAS PRESSED THIS FRAME.
            // To prevent rapid opening/closing of the inventory, the button
            // is checked to determine when it toggles to being pressed.
            bool inventory_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::SECONDARY_ACTION_KEY);
            if (inventory_button_pressed)
            {
                // OPEN OR CLOSE THE INVENTORY.
                InventoryOpened = !InventoryOpened;
            }
            else if (InventoryOpened)
            {
                InventoryGui.RespondToInput(input_controller);
            }
            else
            {
                // OPEN THE SAVE DIALOG BOX IF THE APPROPRIATE BUTTON WAS PRESSED.
                bool save_dialog_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::BACK_KEY);
                if (save_dialog_button_pressed)
                {
                    SaveDialogBoxVisible = true;
                }
            }
        }
    }

    /// Renders the HUD to the provided target.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void HeadsUpDisplay::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER COMPONENTS INDICATING HOW TO SWING THE AXE.
        // An icon is rendered to help players know which key to press.
        const char SWING_AXE_KEY = INPUT_CONTROL::KeyboardInputController::PRIMARY_ACTION_KEY_TEXT;
        MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
        renderer.RenderKeyIcon(SWING_AXE_KEY, TOP_LEFT_SCREEN_POSITION_IN_PIXELS);

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
        const unsigned int KEY_ICON_WIDTH_IN_PIXELS = Glyph::WIDTH_IN_PIXELS;
        MATH::Vector2ui axe_icon_screen_position = TOP_LEFT_SCREEN_POSITION_IN_PIXELS;
        axe_icon_screen_position.X += KEY_ICON_WIDTH_IN_PIXELS;

        std::shared_ptr<GRAPHICS::Texture> axe_texture = Assets->GetTexture(RESOURCES::AXE_TEXTURE_ID);
        if (axe_texture)
        {
            renderer.RenderGuiIcon(*axe_texture, AXE_TEXTURE_SUB_RECTANGLE, axe_icon_screen_position);
        }

        // RENDER COMPONENTS INDICATING HOW MUCH WOOD HAS BEEN COLLECTED.
        // A wood icon is rendered to help players know what the text next to it corresponds to.
        const MATH::FloatRectangle WOOD_LOG_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(
            32.0f, 32.0f, 16.0f, 16.0f);
        // The wood icon should be next to the axe icon but with a little additional blank space
        // so that the two icons aren't too close together.
        const unsigned int PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON = 16;
        MATH::Vector2ui wood_icon_screen_position = axe_icon_screen_position;
        wood_icon_screen_position.X += static_cast<unsigned int>(AXE_TEXTURE_SUB_RECTANGLE.GetWidth());
        wood_icon_screen_position.X += PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON;

        std::shared_ptr<GRAPHICS::Texture> wood_texture = Assets->GetTexture(RESOURCES::WOOD_LOG_TEXTURE_ID);
        if (wood_texture)
        {
            renderer.RenderGuiIcon(*wood_texture, WOOD_LOG_TEXTURE_SUB_RECTANGLE, wood_icon_screen_position);
        }

        // Text is rendered for "x#" to communicate how much wood has been collected.
        // For example, "x10" (no quotes) would be rendered if the player has collected
        // 10 wood logs.
        const std::string TIMES_COUNT_TEXT = "x";
        std::string wood_count_string = TIMES_COUNT_TEXT + std::to_string(Overworld->NoahPlayer->Inventory->WoodCount);
        // This text should be placed just to the right of the wood icon.
        MATH::Vector2f wood_text_top_left_screen_position_in_pixels(
            static_cast<float>(wood_icon_screen_position.X), 
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
        wood_text_top_left_screen_position_in_pixels.X += WOOD_LOG_TEXTURE_SUB_RECTANGLE.GetWidth();
        renderer.RenderText(wood_count_string, wood_text_top_left_screen_position_in_pixels, GRAPHICS::Color::BLACK);

        // RENDER COMPONENTS INDICATING HOW TO OPEN THE INVENTORY.
        // This text is rendered to the far-right of the screen so that its position isn't changed
        // if the space for other GUI elements (like the count of collected wood) changes such
        // that they could distractingly shift the position of this text.
        MATH::Vector2f TOP_RIGHT_SCREEN_POSITION_IN_PIXELS(
            renderer.Screen.WidthInPixels<float>(),
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
        const std::string OPEN_INVENTORY_TEXT = "Inventory";
        // One glyph is rendered per character.
        const size_t OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS = (Glyph::WIDTH_IN_PIXELS * OPEN_INVENTORY_TEXT.size());
        MATH::Vector2f open_inventory_text_top_left_screen_position_in_pixels = TOP_RIGHT_SCREEN_POSITION_IN_PIXELS;
        open_inventory_text_top_left_screen_position_in_pixels.X -= OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS;
        renderer.RenderText(OPEN_INVENTORY_TEXT, open_inventory_text_top_left_screen_position_in_pixels, GRAPHICS::Color::BLACK);

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
            InventoryGui.Render(renderer);
        }

        // RENDER THE TEXT BOX IF IT IS VISIBLE.
        if (MainTextBox.IsVisible)
        {
            MainTextBox.Render(renderer);
        }

        // RENDER THE SAVE DIALOG BOX IF IT IS VISIBLE.
        if (SaveDialogBoxVisible)
        {
            renderer.RenderCenteredText(
                "[ENTER] - Save\n[ESC] - Cancel",
                renderer.Screen.GetBoundingRectangle<float>());
        }
    }

    /// Checks if any modal GUI components of the HUD are currently displayed.
    /// @return True if a modal component of the HUD is displayed; false otherwise.
    bool HeadsUpDisplay::ModalComponentDisplayed() const
    {
        bool modal_component_displayed = InventoryOpened || SaveDialogBoxVisible;
        return modal_component_displayed;
    }
}
}
