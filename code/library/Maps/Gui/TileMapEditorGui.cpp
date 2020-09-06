#include <fstream>
#include <string>
#include "Debugging/DebugConsole.h"
#include "Maps/Gui/TileMapEditorGui.h"

namespace MAPS
{
namespace GUI
{
    /// Updates the tile map editor GUI in response to user input.
    /// @param[in,out]  input_controller - The input controller supplying user input.
    void TileMapEditorGui::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // TOGGLE THE MAP EDITOR IF THE KEY WAS PRESSED.
        bool map_editor_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_KEY);
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

        // SAVE THE UPDATED MAP IF THE APPROPRIATE KEY WAS PRESSED.
        bool save_map_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_SAVE_MAP_KEY);
        if (save_map_key_pressed)
        {
            if (CurrentTileMap)
            {
                WriteTileMapFile(*CurrentTileMap);
            }
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
            // Check if the mouse button is down is done to allow quick editing without a user needing to press
            // the button separately for each individual tile.
            // The user must have already chosen a tile from the palette and then pressed the mouse button within the tile map.
            bool pointer_button_pressed = input_controller.ButtonDown(INPUT_CONTROL::InputController::MAIN_POINTER_BUTTON);
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
            renderer.RenderGuiIcon(SelectedTile->Sprite.CurrentFrameSprite, MouseScreenPosition);
        }
    }

    /// Writes the basic tile data of the provided tile map out
    /// to a file so that the updated map data can be copied into the
    /// actual game code (and thus easily update the map).
    /// @param[in]  tile_map - The tile map to write to file.
    void TileMapEditorGui::WriteTileMapFile(const MAPS::TileMap& tile_map)
    {
        // OPEN THE OUTPUT FILE.
        // The name is hardcoded since this is sufficient for now.
        // Ideally, we'd have a way to automatically update the
        // compiled-in map data without manual copying, but it
        // hasn't yet been worth the effort to overcome that.
        std::ofstream tile_map_file("edited_tile_map.txt");
        if (!tile_map_file.is_open())
        {
            // The file can't be written.
            return;
        }

        // WRITE OUT THE BASIC GRID COORDINATES OF THE TILE MAP.
        tile_map_file
            << "Column: " << tile_map.GridColumnIndex
            << "\n"
            << "Row: " << tile_map.GridRowIndex
            << "\n";

        // WRITE OUT EACH TILE ID IN THE GROUND LAYER.
        // Tile data is stored first by row.
        unsigned int map_height_in_tiles = tile_map.Ground.Tiles.GetHeight();
        unsigned int map_width_in_tiles = tile_map.Ground.Tiles.GetWidth();
        for (unsigned int tile_row_index = 0; tile_row_index < map_height_in_tiles; ++tile_row_index)
        {
            for (unsigned int tile_column_index = 0; tile_column_index < map_width_in_tiles; ++tile_column_index)
            {
                // WRITE OUT THE CURRENT TILE.
                std::shared_ptr<MAPS::Tile> tile = tile_map.Ground.Tiles(tile_column_index, tile_row_index);
                if (tile)
                {
                    // The ID is the only data that needs to be written.
                    tile_map_file << tile->Id;
                }
                else
                {
                    // Default to black tile ID to ensure that some data gets written.
                    tile_map_file << MAPS::TileType::BLACK_TEST;
                }

                // WRITE OUT A SEPARATOR BEFORE THE NEXT TILE.
                tile_map_file << ", ";
            }

            // WRITE OUT A SEPARATOR BEFORE THE NEXT ROW.
            tile_map_file << "\n";
        }
    }
}
}
