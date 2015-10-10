#pragma once

#include <vector>
#include "Maps/GroundLayer.h"
#include "Math/Vector2.h"
#include "Objects/Tree.h"

namespace MAPS
{    
    /// A single 2D map composed of individual tiles and any objects
    /// that may located on the map.
    class TileMap
    {
    public:
        // CONSTRUCTION.
        /// Creates an empty tile map.
        /// @param[in]  overworld_row_index - The 0-based index (from the top) of
        ///     the tile map as located in the overworld.
        /// @param[in]  overworld_column_index - The 0-based index (from the left) of
        ///     the tile map as located in the overworld.
        /// @param[in]  center_world_position - The world position of the center
        ///     of the tile map.
        /// @param[in]  dimensions_in_tiles - The dimensions of the map in tiles.
        explicit TileMap(
            const unsigned int overworld_row_index,
            const unsigned int overworld_column_index,
            const MATH::Vector2f& center_world_position,
            const MATH::Vector2ui& dimensions_in_tiles);

        // POSITIONING.
        /// Gets the center world position of the tile map.
        /// @return The center world position of the tile map.
        MATH::Vector2f GetCenterWorldPosition() const;

        // DIMENSIONS.
        /// Gets the dimensions (width, height) of the tile map, in units of tiles.
        /// @return The dimensions of the tile map, in tiles.
        MATH::Vector2ui GetDimensionsInTiles() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The 0-based index (from the top) of the tile map as located in the overworld.
        unsigned int OverworldRowIndex;
        /// The 0-based index (from the left) of the tile map as located in the overworld.
        unsigned int OverworldColumnIndex;
        /// The ground in this tile map.
        GroundLayer Ground;
        /// Trees in this tile map.
        std::vector<OBJECTS::Tree> Trees;
    };
}