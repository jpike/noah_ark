#pragma once

#include "Graphics/Sprite.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// Enumerates the different types of tiles.
    /// A regular enum is used to avoid having to do explicit static casting,
    /// which would degrade readability.  However, in order to scope the
    /// enum values and increase readability, the enum is wrapped with a struct.
    struct TileType
    {
        /// Explicit numeric values are assigned that map to tile IDs.
        enum Id
        {
            /// An invalid tile.
            INVALID = 0,
            /// A completely black tile for testing.
            BLACK_TEST = 1,
            /// A completely white tile for testing.
            WHITE_TEST = 2,
            /// A sand tile.
            SAND = 3,
            /// A grass tile.
            GRASS = 4,
            /// A water tile.
            WATER = 5,
            /// A brown dirt tile.
            BROWN_DIRT = 6,
            /// A gray stone tile.
            GRAY_STONE = 7,
            /// A tile upon when the ark can be built.
            ARK_BUILDING_PLOT = 8,
            /// An extra enum value to define count of different tile types.
            COUNT
        };
    };

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
        /// The numeric ID for the tile in a tile set.
        TileId Id;
        /// The type of this tile.
        TileType::Id Type;
        /// The sprite used for rendering the tile.
        GRAPHICS::Sprite Sprite;
    };
}