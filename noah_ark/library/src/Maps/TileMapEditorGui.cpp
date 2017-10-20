#include <stdexcept>
#include <string>
#include "Core/NullChecking.h"
#include "Debugging/DebugConsole.h"
#include "Maps/TileMapEditorGui.h"

namespace MAPS
{
    /// Creates an invisible instance of the tile map editor GUI.
    /// @param[in]  tileset_texture - The texture for the tileset used by the GUI.
    /// @throws std::exception - Thrown if the parameter is null or a tile fails to be created.
    TileMapEditorGui::TileMapEditorGui(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture) :
    Visible(false),
    Tileset(tileset_texture),
    TilesById(),
    SelectedTile()
    {
        // MAKE SURE A TILESET TEXTURE WAS PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(tileset_texture, "Tile map editor GUI provided with null tileset texture.");

        // CREATE THE TILES RENDERED BY THIS EDITOR GUI.
        for (unsigned int tile_id_number = static_cast<unsigned int>(TileType::Id::INVALID) + 1;
            tile_id_number < static_cast<unsigned int>(TileType::Id::COUNT);
            ++tile_id_number)
        {
            // CREATE THE CURRENT TILE SPRITE.
            const TileType::Id tile_id = static_cast<TileType::Id>(tile_id_number);
            const std::shared_ptr<Tile> tile = Tileset.CreateTile(tile_id);
            const bool tile_created = (nullptr != tile);
            if (!tile_created)
            {
                // PROVIDE VISIBILITY INTO THE ERROR.
                throw std::runtime_error("Failed to create tile with ID " + std::to_string(tile_id_number) + " for tile map editor.");
            }

            // STORE THE CREATED TILE.
            TilesById[tile_id] = tile;
        }
    }

    /// Updates the tile map editor GUI in response to user input.
    /// @param[in,out]  input_controller - The input controller supplying user input.
    void TileMapEditorGui::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // TOGGLE THE MAP EDITOR IF THE KEY WAS PRESSED.
        const bool map_editor_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_KEY);
        if (map_editor_key_pressed)
        {
            Visible = !Visible;
        }

        // EXIT IF THE GUI ISN'T VISIBLE.
        // If the GUI isn't visible, then it shouldn't be responsive to any more user input.
        if (!Visible)
        {
            return;
        }

        // CHECK IF A TILE IS CURRENTLY SELECTED.
        if (SelectedTile)
        {

        }
        else
        {
            // CHECK IF THE USER HAS SELECTED A TILE.
            bool pointer_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAIN_POINTER_BUTTON);
            if (pointer_button_pressed)
            {
                // GET THE SELECTED TILE (IF ONE EXISTS).
                /// \todo
                DEBUGGING::DebugConsole::WriteLine("Pointer pressed.");
                //MATH::Vector2f pointer_screen_position = input_controller.GetPointerScreenCoordinates(INPUT_CONTROL::InputController::POINTER_BUTTON);
                //SelectedTile = GetTileFromPaletteAtScreenPosition(pointer_screen_position);
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

        // RENDER THE TILESET.
        for (const auto& id_with_tile : TilesById)
        {
            // CALCULATE THE POSITION FOR RENDERING THE TILE.
            const TileType::Id tile_id = id_with_tile.first;
            const std::shared_ptr<Tile>& tile = id_with_tile.second;
            
            const MATH::FloatRectangle screen_bounds = renderer.Screen.GetBoundingRectangle<float>();
            const float screen_left_position = screen_bounds.GetLeftXPosition();
            const float tile_top_screen_position = tile_id * Tile::DIMENSION_IN_PIXELS<float>;
            const MATH::Vector2f tile_left_top_screen_position_in_pixels(
                screen_left_position,
                tile_top_screen_position);

            // RENDER THE TILE.
            renderer.RenderGuiIcon(tile->Sprite, tile_left_top_screen_position_in_pixels);
        }
    }
}
