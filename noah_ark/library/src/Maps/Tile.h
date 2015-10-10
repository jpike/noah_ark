#pragma once

#include "Graphics/Sprite.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// A 2D graphical tile intended to be used as a building block for tile maps.
    /// Tile's are positioned in the world based on their top-left positions.
    class Tile
    {
    public:
        // CONSTRUCTION.
        /// Constructor to create an invalid tile.
        explicit Tile();
        /// Constructor accepting all of a tile's data.
        /// @param[in]  id - A numeric ID identifying the tile in a tileset.
        /// @param[in]  sprite - The sprite used for rendering the tile.
        explicit Tile(
            const TileId id,
            const GRAPHICS::Sprite& sprite);

        // POSITIONING.
        /// Sets the world position of the top-left of the tile.
        /// @param[in]  world_x_position - The world x position of the left of the tile.
        /// @param[in]  world_y_position - The world y position of the top of the tile.
        /// @todo   Rethink if tiles should instead be positioned by center
        /// since this is inconsistent with other positioning.
        void SetWorldPosition(const float world_x_position, const float world_y_position);

        // DIMENSIONS.
        /// Gets the width of the tile, in pixels.
        /// @return The width of the tile in pixels.
        float GetWidthInPixels() const;
        /// Gets the height of the tile, in pixels.
        /// @return The height of the tile in pixels.
        float GetHeightInPixels() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        TileId Id;  ///< The numeric ID for the tile in a tile set.
        GRAPHICS::Sprite Sprite;   ///< The sprite used for rendering the tile.
    };
}