#include <string>
#include "Debugging/DebugConsole.h"
#include "Maps/Gui/TileMapEditorGui.h"

namespace MAPS
{
namespace GUI
{
    /// Creates an invisible instance of the tile map editor GUI.
    /// @param[in]  tileset_texture - The texture for the tileset used by the GUI.
    /// @throws std::exception - Thrown if the parameter is null or a tile fails to be created.
    TileMapEditorGui::TileMapEditorGui(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture) :
    Visible(false),
    CurrentTileMap(nullptr),
    TilePalette(tileset_texture),
    SelectedTile(),
    MouseScreenPosition()
    {}

    /// Updates the tile map editor GUI in response to user input.
    /// @param[in,out]  input_controller - The input controller supplying user input.
    void TileMapEditorGui::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // TOGGLE THE MAP EDITOR IF THE KEY WAS PRESSED.
        const bool map_editor_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_KEY);
        if (map_editor_key_pressed)
        {
            // TOGGLE THE MAIN GUI.
            Visible = !Visible;

            // TOGGLE THE TILE PALETTE.
            // It should be made visible if the map editor is being opened.
            TilePalette.Visible = Visible;
        }

        // EXIT IF THE GUI ISN'T VISIBLE.
        // If the GUI isn't visible, then it shouldn't be responsive to any more user input.
        if (!Visible)
        {
            return;
        }

        // TRACK THE MOUSE SCREEN POSITION.
        MouseScreenPosition = input_controller.Mouse.ScreenPosition;

        // GET ANY NEWLY SELECTED TILE FROM THE TILE PALETTE.
        std::shared_ptr<MAPS::Tile> newly_selected_tile = TilePalette.RespondToInput(input_controller);
        if (newly_selected_tile)
        {
            SelectedTile = newly_selected_tile;
        }
        else
        {
            // CHECK IF THE USER CHOSE TO UPDATE A TILE IN THE MAP.
            // The user must have already chosen a tile from the palette and then pressed the mouse button within the tile map.
            bool pointer_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAIN_POINTER_BUTTON);
            bool user_chose_to_update_tile_in_map = SelectedTile && pointer_button_pressed && CurrentTileMap;
            if (user_chose_to_update_tile_in_map)
            {
                // CALCULATE THE TILE'S OFFSET WITHIN THE CURRENT TILE MAP.
                unsigned int tile_x_offset_from_left_in_tiles = static_cast<unsigned int>(
                    MouseScreenPosition.X / MAPS::Tile::DIMENSION_IN_PIXELS<float>);
                unsigned int tile_y_offset_from_top_in_tiles = static_cast<unsigned int>(
                    MouseScreenPosition.Y / MAPS::Tile::DIMENSION_IN_PIXELS<float>);

                // SET THE TILE IN THE TILE MAP.
                std::shared_ptr<MAPS::Tile> new_tile = std::make_shared<MAPS::Tile>(
                    SelectedTile->Id,
                    SelectedTile->Sprite);
                CurrentTileMap->Ground.SetTile(
                    tile_x_offset_from_left_in_tiles,
                    tile_y_offset_from_top_in_tiles,
                    new_tile);
            }
        }
    }

    /// Renders the GUI if the GUI is visible.
    /// @param[in,out]  renderer - The renderer to use.
    void TileMapEditorGui::Render(GRAPHICS::Renderer& renderer) const
    {
        // MAKE SURE THE GUI IS VISIBLE.
        if (!Visible)
        {
            return;
        }

        // RENDER THE TILES TO SELECT FROM.
        TilePalette.Render(renderer);

        // RENDER ANY SELECTED TILE AT THE MOUSE CURSOR.
        if (SelectedTile)
        {
            renderer.RenderGuiIcon(SelectedTile->Sprite, MouseScreenPosition);
        }
    }
}
}
