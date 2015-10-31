#pragma once

#include "Graphics/Sprite.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// A 2D graphical tile intended to be used as a building block for tile maps.
    /// Tile's are positioned in the world based on their top-left positions.
    /// @todo   Rethink if tiles should instead be positioned by center
    /// since this is inconsistent with other positioning.
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
        void SetWorldPosition(const float world_x_position, const float world_y_position);

        // DIMENSIONS.
        /// Gets the width of the tile, in pixels.
        /// @return The width of the tile in pixels.
        float GetWidthInPixels() const;
        /// Gets the height of the tile, in pixels.
        /// @return The height of the tile in pixels.
        float GetHeightInPixels() const;

        // BOUNDARIES.
        /// Gets the left X position of the tile's bounding box, in world coordinates.
        /// @return The left X position of the tile.
        float GetLeftXPosition() const;
        /// Gets the right X position of the tile's bounding box, in world coordinates.
        /// @return The right X position of the tile.
        float GetRightXPosition() const;
        /// Gets the top Y position of the tile's bounding box, in world coordinates.
        /// @return The top Y position of the tile.
        float GetTopYPosition() const;
        /// Gets the bottomt Y position of the tile's bounding box, in world coordinates.
        /// @return The bottom Y position of the tile.
        float GetBottomYPosition() const;

        // STATE CHECKING.
        /// Determines if the tile is walkable.
        /// @return True if the tile is walkable; false otherwise.
        bool IsWalkable() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        TileId Id;  ///< The numeric ID for the tile in a tile set.
        GRAPHICS::Sprite Sprite;   ///< The sprite used for rendering the tile.
    };
}