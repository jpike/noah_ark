#include "Maps/TileMap.h"

namespace MAPS
{
    TileMap::TileMap(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
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