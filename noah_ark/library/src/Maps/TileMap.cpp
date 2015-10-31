#include "Maps/TileMap.h"

namespace MAPS
{
    TileMap::TileMap(
        const unsigned int overworld_row_index,
        const unsigned int overworld_column_index,
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    OverworldRowIndex(overworld_row_index),
    OverworldColumnIndex(overworld_column_index),
    Ground(center_world_position, dimensions_in_tiles, tile_dimension_in_pixels),
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

    std::shared_ptr<MAPS::Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        std::shared_ptr<MAPS::Tile> tile = Ground.GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }
}