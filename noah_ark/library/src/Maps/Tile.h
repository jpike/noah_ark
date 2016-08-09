#pragma once

#include "Graphics/Sprite.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// A 2D graphical tile intended to be used as a building block for tile maps.
    /// Tile's are positioned in the world based on their center positions,
    /// rather than top-left corners, to be consistent with placement of
    /// other objects.
    class Tile
    {
    public:
        // CONSTRUCTION.
        explicit Tile();
        explicit Tile(
            const TileId id,
            const GRAPHICS::Sprite& sprite);

        // POSITIONING.
        void SetWorldPosition(const float world_x_position, const float world_y_position);

        // DIMENSIONS.
        float GetWidthInPixels() const;
        float GetHeightInPixels() const;

        // BOUNDARIES.
        float GetLeftXPosition() const;
        float GetRightXPosition() const;
        float GetTopYPosition() const;
        float GetBottomYPosition() const;

        // STATE CHECKING.
        bool IsWalkable() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        TileId Id;  ///< The numeric ID for the tile in a tile set.
        GRAPHICS::Sprite Sprite;   ///< The sprite used for rendering the tile.
    };
}