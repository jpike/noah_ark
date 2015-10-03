#pragma once

#include <memory>
#include <vector>
#include "Core/Array2D.h"
#include "Graphics/IGraphicsComponent.h"
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
        /// Constructor.
        /// @param[in]  center_world_position - The world position of the center
        ///     of the ground layer.
        /// @param[in]  dimensions_in_tiles - The dimensions of the layer in tiles.
        explicit GroundLayer(
            const MATH::Vector2f& center_world_position,
            const MATH::Vector2ui& dimensions_in_tiles);

        /// Populates the tile at the specified offset in the ground layer.
        /// @param[in]  x_offset_from_left_in_tiles - The x offset from the left of
        ///     the layer, in tiles.
        /// @param[in]  y_offset_from_top_in_tiles - The y offset from the top of
        ///     the layer, in tiles.
        /// @param[in]  tile - The tile to set at the specified position.  Its
        ///     world position will be populated to reflect its positioning
        ///     within this layer within the larger world.
        void SetTile(
            const unsigned int x_offset_from_left_in_tiles,
            const unsigned int y_offset_from_top_in_tiles,
            const std::shared_ptr<MAPS::Tile>& tile);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The world position of the center of the layer.
        MATH::Vector2f CenterWorldPosition;
        /// The tiles in this layer.  Stored as offsets from top-left corner,
        /// which is (0,0) (column/x,row/y).
        CORE::Array2D< std::shared_ptr<MAPS::Tile> > Tiles;
    };
}