#pragma once

#include <vector>
#include "Maps/GroundLayer.h"
#include "Math/Vector2.h"
#include "Objects/ArkPiece.h"
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
        explicit TileMap(
            const unsigned int overworld_row_index = 0,
            const unsigned int overworld_column_index = 0,
            const MATH::Vector2f& center_world_position = MATH::Vector2f(),
            const MATH::Vector2ui& dimensions_in_tiles = MATH::Vector2ui(),
            const unsigned int tile_dimension_in_pixels = 0);

        // POSITIONING.
        MATH::Vector2f GetCenterWorldPosition() const;

        // DIMENSIONS.
        MATH::Vector2ui GetDimensionsInTiles() const;

        // BOUNDARIES.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // RETRIEVAL.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;
        OBJECTS::ArkPiece* GetArkPieceAtWorldPosition(const MATH::Vector2f& world_position);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The 0-based index (from the top) of the tile map as located in the overworld.
        unsigned int OverworldRowIndex;
        /// The 0-based index (from the left) of the tile map as located in the overworld.
        unsigned int OverworldColumnIndex;
        /// The ground in this tile map.
        GroundLayer Ground;
        /// Trees in this tile map.
        std::vector<OBJECTS::Tree> Trees;
        /// Dust clouds within the tile map.
        std::vector<OBJECTS::DustCloud> DustClouds;
        /// Wooden logs that have appeared in the map from chopped down trees.
        std::vector<OBJECTS::WoodLogs> WoodLogs;
        /// Pieces of the ark that may exist on this tile map.
        /// Stored in a vector currently for simplicitly at the moment,
        /// but a different data structure may be needed later for performance.
        std::vector<OBJECTS::ArkPiece> ArkPieces;
    };
}