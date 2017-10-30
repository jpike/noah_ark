#include "Maps/TileMap.h"

namespace MAPS
{
    /// Creates an empty tile map.  Parameters have default values to allow
    /// for default construction.
    /// @param[in]  grid_row_index - The 0-based index (from the top) of
    ///     the tile map as located in its grid.
    /// @param[in]  grid_column_index - The 0-based index (from the left) of
    ///     the tile map as located in its grid.
    /// @param[in]  center_world_position - The world position of the center
    ///     of the tile map.
    /// @param[in]  dimensions_in_tiles - The dimensions of the map in tiles.
    /// @param[in]  tile_dimension_in_pixels - The dimensions (both width and height)
    ///     of an individual tile in the map (in units of pixels).
    TileMap::TileMap(
        const unsigned int grid_row_index,
        const unsigned int grid_column_index,
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    GridRowIndex(grid_row_index),
    GridColumnIndex(grid_column_index),
    Ground(center_world_position, dimensions_in_tiles, tile_dimension_in_pixels),
    Trees(),
    FallingFood(),
    FoodOnGround(),
    DustClouds(),
    WoodLogs(),
    ArkPieces()
    {}

    /// Gets the center world position of the tile map.
    /// @return The center world position of the tile map.
    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        return Ground.CenterWorldPosition;
    }

    /// Gets the dimensions (width, height) of the tile map, in units of tiles.
    /// @return The dimensions of the tile map, in tiles.
    MATH::Vector2ui TileMap::GetDimensionsInTiles() const
    {
        MATH::Vector2ui dimensions_in_tiles(
            Ground.Tiles.GetWidth(),
            Ground.Tiles.GetHeight());

        return dimensions_in_tiles;
    }

    /// Gets the bounding box of the tile map in world coordinates.
    /// @return The world bounding box of the tile map.
    MATH::FloatRectangle TileMap::GetWorldBoundingBox() const
    {
        // GET THE CENTER OF THE TILE MAP.
        MATH::Vector2f center_world_position = GetCenterWorldPosition();

        // GET THE FULL DIMENSIONS OF THE TILE MAP.
        MATH::Vector2ui dimensions_in_tiles = GetDimensionsInTiles();
        float width_in_pixels = static_cast<float>(dimensions_in_tiles.X * Ground.TileDimensionInPixels);
        float height_in_pixels = static_cast<float>(dimensions_in_tiles.Y * Ground.TileDimensionInPixels);

        // FORM THE WORLD BOUNDING BOX.
        MATH::FloatRectangle world_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            center_world_position.X,
            center_world_position.Y,
            width_in_pixels,
            height_in_pixels);
        return world_bounding_box;
    }

    /// Gets the tile that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile to retrieve.
    /// @return The tile at the specified location, if one exists; null otherwise.
    std::shared_ptr<MAPS::Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        std::shared_ptr<MAPS::Tile> tile = Ground.GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }

    /// Gets an ark piece at the specified world position, if one exists.
    /// @param[in]  world_position - The world position for which to retrieve an ark piece.
    /// @return The ark piece at the specified world position, if one eixsts; null otherwise.
    OBJECTS::ArkPiece* TileMap::GetArkPieceAtWorldPosition(const MATH::Vector2f& world_position)
    {
        // SEARCH FOR AN ARK PIECE THAT CONTAINS THE SPECIFIED WORLD POSITION.
        for (auto& ark_piece : ArkPieces)
        {
            // CHECK IF THE CURRENT ARK PIECE INCLUDES THE WORLD POSITION.
            MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
            bool ark_piece_contains_world_position = ark_piece_bounding_box.Contains(world_position.X, world_position.Y);
            if (ark_piece_contains_world_position)
            {
                // RETURN THE ARK PIECE.
                return &ark_piece;
            }
        }

        // INDICATE THAT NO ARK PIECE COULD BE FOUND.
        return nullptr;
    }
}