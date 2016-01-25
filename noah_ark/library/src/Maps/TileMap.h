#pragma once

#include <vector>
#include "Maps/GroundLayer.h"
#include "Math/Vector2.h"
#include "Objects/DustCloud.h"
#include "Objects/Tree.h"
#include "Objects/WoodLogs.h"

namespace MAPS
{    
    /// A single 2D map composed of individual tiles and any objects
    /// that may located on the map.
    class TileMap
    {
    public:
        // CONSTRUCTION.
        /// Creates an empty tile map.  Parameters have default values to allow
        /// for default construction.
        /// @param[in]  overworld_row_index - The 0-based index (from the top) of
        ///     the tile map as located in the overworld.
        /// @param[in]  overworld_column_index - The 0-based index (from the left) of
        ///     the tile map as located in the overworld.
        /// @param[in]  center_world_position - The world position of the center
        ///     of the tile map.
        /// @param[in]  dimensions_in_tiles - The dimensions of the map in tiles.
        /// @param[in]  tile_dimension_in_pixels - The dimensions (both width and height)
        ///     of an individual tile in the map (in units of pixels).
        explicit TileMap(
            const unsigned int overworld_row_index = 0,
            const unsigned int overworld_column_index = 0,
            const MATH::Vector2f& center_world_position = MATH::Vector2f(),
            const MATH::Vector2ui& dimensions_in_tiles = MATH::Vector2ui(),
            const unsigned int tile_dimension_in_pixels = 0);

        // POSITIONING.
        /// Gets the center world position of the tile map.
        /// @return The center world position of the tile map.
        MATH::Vector2f GetCenterWorldPosition() const;

        // DIMENSIONS.
        /// Gets the dimensions (width, height) of the tile map, in units of tiles.
        /// @return The dimensions of the tile map, in tiles.
        MATH::Vector2ui GetDimensionsInTiles() const;

        // BOUNDARIES.
        /// Gets the bounding box of the tile map in world coordinates.
        /// @return The world bounding box of the tile map.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // TILE RETRIEVAL.
        /// Gets the tile that includes the specified world coordinates.
        /// @param[in]  world_x_position - The world x position of the tile to retrieve.
        /// @param[in]  world_y_position - The world y position of the tile to retrieve.
        /// @return The tile at the specified location, if one exists; null otherwise.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The 0-based index (from the top) of the tile map as located in the overworld.
        unsigned int OverworldRowIndex;
        /// The 0-based index (from the left) of the tile map as located in the overworld.
        unsigned int OverworldColumnIndex;
        /// The ground in this tile map.
        GroundLayer Ground;
        /// Trees in this tile map.
        std::vector<OBJECTS::Tree> Trees;
        /// Dust clouds for trees that are disappearing and becoming wooden logs.
        std::vector<OBJECTS::DustCloud> TreeDustClouds;
        /// Wooden logs that have appeared in the map from chopped down trees.
        std::vector<OBJECTS::WoodLogs> WoodLogs;
    };
}