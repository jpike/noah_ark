#pragma once

#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace MAPS
{
    // Forward-declared types to avoid circular dependencies.
    class MultiTileMapGrid;
    class TileMap;

    /// An exit point in one multi-tile-map grid that leads to a different multi-tile-map grid.
    /// Exit points defines locations that allow a player to move between different tile map grids.
    /// If an exit point exists, all fields of it are supposed to be fully populated.
    class ExitPoint
    {
    public:
        /// The bounding box of the exit point within its tile map.
        MATH::FloatRectangle BoundingBox = MATH::FloatRectangle();
        /// The new map grid that the exit point leads to.
        MultiTileMapGrid* NewMapGrid = nullptr;
        /// The starting tile map in the new map grid that the exit point leads to.
        TileMap* NewTileMap = nullptr;
        /// The new position of the player within the new tile map.
        MATH::Vector2f NewPlayerWorldPosition = MATH::Vector2f();
    };
}
