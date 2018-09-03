#pragma once

#include <memory>
#include "Core/Array2D.h"
#include "Maps/Tile.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A single layer of tiles representing the ground for an area of the world.
    /// The ground does not necessarily represent solid "ground" but may include
    /// tiles for things such as water as well.
    class GroundLayer
    {
    public:
        explicit GroundLayer(
            const MATH::Vector2f& center_world_position,
            const MATH::Vector2ui& dimensions_in_tiles,
            const unsigned int tile_dimension_in_pixels);

        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;
        
        void SetTile(
            const unsigned int x_offset_from_left_in_tiles,
            const unsigned int y_offset_from_top_in_tiles,
            const std::shared_ptr<MAPS::Tile>& tile);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The dimension (both width and height) of an individual tile in the map (in units of pixels).
        unsigned int TileDimensionInPixels;
        /// The world position of the center of the layer.
        MATH::Vector2f CenterWorldPosition;
        /// The tiles in this layer.  Stored as offsets from top-left corner,
        /// which is (0,0) (column/x,row/y).  Stored as shared pointers to support
        /// storage directly when created from a tileset.
        CORE::Array2D< std::shared_ptr<MAPS::Tile> > Tiles;
    };
}