#include <stdexcept>
#include "Debugging/DebugConsole.h"
#include "ErrorHandling/NullChecking.h"
#include "Maps/Gui/TilePalette.h"

namespace MAPS
{
namespace GUI
{
    /// Initializes an invisible palette to use the provided tileset texture.
    /// @param[in]  tileset_texture - The texture for the tileset used by the palette.
    /// @throws std::exception - Thrown if the parameter is null or a tile fails to be created.
    TilePalette::TilePalette(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture):
        Visible(false),
        Tileset(tileset_texture),
        TilesById(),
        TileScreenBoundsById()
    {
        // MAKE SURE A TILESET TEXTURE WAS PROVIDED.
        ERROR_HANDLING::ThrowInvalidArgumentExceptionIfNull(tileset_texture, "Tile palette provided with null tileset texture.");

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

            // STORE THE SCREEN BOUNDARIES OF THE TILE.
            // This is used for quick rendering and determination of intersection of a
            // tile with a screen position (such as from a mouse).
            constexpr float LEFT_OFFSET_POSITION = 0.0f;
            const float tile_top_screen_position = static_cast<float>(tile_id) * Tile::DIMENSION_IN_PIXELS<float>;
            TileScreenBoundsById[tile_id] = MATH::FloatRectangle::FromLeftTopAndDimensions(
                LEFT_OFFSET_POSITION,
                tile_top_screen_position,
                Tile::DIMENSION_IN_PIXELS<float>,
                Tile::DIMENSION_IN_PIXELS<float>);
        }
    }

    /// Updates the tile palette GUI in response to user input.
    /// @param[in,out]  input_controller - The input controller supplying user input.
    /// @return The tile selected by the user (if one was selected); null otherwise.
    std::shared_ptr<Tile> TilePalette::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // TOGGLE THE TILE PALETTE IF THE KEY WAS PRESSED.
        const bool tile_palette_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_TILE_PALETTE_KEY);
        if (tile_palette_key_pressed)
        {
            Visible = !Visible;
        }

        // EXIT IF THE GUI ISN'T VISIBLE.
        // If the GUI isn't visible, then it shouldn't be responsive to any more user input.
        if (!Visible)
        {
            // No tile was selected from the palette.
            return nullptr;
        }

        // CHECK IF THE USER HAS SELECTED A TILE.
        // The secondary pointer button is used to avoid interference with the primary pointer button
        // that is also used during map editing for placing tiles in an actual map.
        bool pointer_button_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::SECONDARY_POINTER_BUTTON);
        if (pointer_button_pressed)
        {
            // GET THE SELECTED TILE (IF ONE EXISTS).
            DEBUGGING::DebugConsole::WriteLine("Mouse pressed at position: ", input_controller.Mouse.ScreenPosition);
            std::shared_ptr<Tile> selected_tile = GetTileAtScreenPosition(input_controller.Mouse.ScreenPosition);
            return selected_tile;
        }

        // INDICATE THAT NO TILE WAS SELECTED.
        // If a tile was selected, code above should have already returned.
        return nullptr;
    }

    /// Renders the tile palette (if visible).
    /// @param[in,out]  renderer - The renderer to use.
    void TilePalette::Render(GRAPHICS::Renderer& renderer) const
    {
        // MAKE SURE THE TILE PALETTE IS VISIBLE.
        if (!Visible)
        {
            return;
        }

        // RENDER THE TILES.
        for (const auto& id_with_tile : TilesById)
        {
            // GET THE TILE AND ITS SCREEN BOUNDARIES.
            TileType::Id tile_id = id_with_tile.first;
            const std::shared_ptr<Tile>& tile = id_with_tile.second;
            const MATH::FloatRectangle& tile_screen_bounds = TileScreenBoundsById.at(tile_id);

            // RENDER THE TILE.
            renderer.RenderGuiIcon(tile->Sprite.Sprite, tile_screen_bounds.LeftTop);
        }
    }

    /// Gets the tile at the specified screen position (if one exists).
    /// @param[in]  screen_position - The screen position for which to retrieve a tile.
    /// @return The tile from the palette at the specified screen position, if one exists; null otherwise.
    std::shared_ptr<Tile> TilePalette::GetTileAtScreenPosition(const MATH::Vector2f& screen_position) const
    {
        // CHECK IF THE TILE PALETTE IS VISIBLE.
        if (!Visible)
        {
            // The palette isn't visible, so there can't be a tile as the screen position.
            return nullptr;
        }

        // SEACH FOR A TILE AT THE SCREEN POSITION.
        for (const auto& id_with_tile : TilesById)
        {
            // GET THE TILE AND ITS SCREEN BOUNDARIES.
            TileType::Id tile_id = id_with_tile.first;
            const std::shared_ptr<Tile>& tile = id_with_tile.second;
            const MATH::FloatRectangle& tile_screen_bounds = TileScreenBoundsById.at(tile_id);

            // CHECK IF THE TILE CONTAINS THE SCREEN POSITION.
            bool tile_contains_screen_position = tile_screen_bounds.Contains(screen_position.X, screen_position.Y);
            if (tile_contains_screen_position)
            {
                return tile;
            }
        }

        // INDICATE THAT NO TILE WAS FOUND.
        // If we reach this point, then no tile contained the screen position.
        return nullptr;
    }
}
}
