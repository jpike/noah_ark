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
}