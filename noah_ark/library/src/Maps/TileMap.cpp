#include "Maps/TileMap.h"

namespace MAPS
{
    TileMap::TileMap(
        const unsigned int overworld_row_index,
        const unsigned int overworld_column_index,
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    OverworldRowIndex(overworld_row_index),
    OverworldColumnIndex(overworld_column_index),
    Ground(center_world_position, dimensions_in_tiles),
    Trees()
    {}

    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        return Ground.CenterWorldPosition;
    }

    MATH::Vector2ui TileMap::GetDimensionsInTiles() const
    {
        MATH::Vector2ui dimensions_in_tiles(
            Ground.Tiles.GetWidth(),
            Ground.Tiles.GetHeight());

        return dimensions_in_tiles;
    }

    MATH::FloatRectangle TileMap::GetWorldBoundingBox() const
    {
        MATH::Vector2f center_world_position = GetCenterWorldPosition();

        /// @todo   This probably shouldn't be hardcoded...Perhaps include in overworld map file and pass in during construction.
        const float TILE_DIMENSION_IN_PIXELS = 16.0f;
        MATH::Vector2ui dimensions_in_tiles = GetDimensionsInTiles();
        MATH::Vector2f dimensions_in_pixels;
        dimensions_in_pixels.X = static_cast<float>(dimensions_in_tiles.X) * TILE_DIMENSION_IN_PIXELS;
        dimensions_in_pixels.Y = static_cast<float>(dimensions_in_tiles.Y) * TILE_DIMENSION_IN_PIXELS;

        MATH::FloatRectangle world_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            center_world_position.X,
            center_world_position.Y,
            dimensions_in_pixels.X,
            dimensions_in_pixels.Y);
        return world_bounding_box;
    }

    std::shared_ptr<MAPS::Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        std::shared_ptr<MAPS::Tile> tile = Ground.GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }
}