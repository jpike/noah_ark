#pragma once

#include <memory>
#include "Graphics/Sprite.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// A 2D graphical tile intended to be used as a building block for tile maps.
    class Tile
    {
    public:
        /// Constructor to create an invalid tile.
        explicit Tile();
        /// Constructor.
        /// @param[in]  tile_id - A numeric ID identifying the tile in a tileset.
        /// @param[in]  sprite - The sprite resource used for rendering the tile.
        explicit Tile(
            const unsigned int tile_id,
            const std::shared_ptr<GRAPHICS::Sprite>& sprite);
        /// @brief      Copy constructor.
        /// @param[in]  tile_to_copy - The tile to copy.
        explicit Tile(const Tile& tile_to_copy);
        /// @brief  Destructor.
        ~Tile();

        /// @brief      Assignment operator.
        /// @param[in]  rhs_tile - The tile to assign from.
        Tile& operator= (const Tile& rhs_tile);

        /// @brief  Determines if the tile is walkable or not.
        /// @return Whether or not this tile is walkable.
        bool IsWalkable() const;

        /// @brief  Gets the height of the tile, in pixels.
        /// @return The height of the tile in pixels.
        float GetHeightInPixels() const;
        
        /// @brief  Gets the left X position of the tile's bounding box, in world pixel coordinates.
        /// @return The left X position of the tile.
        float GetLeftXPosition() const;
        /// @brief  Gets the right X posision of the tile's bounding box, in world pixel coordinates.
        /// @return The right X position of the tile.
        float GetRightXPosition() const;

        /// @brief  Gets the top Y position of the tile's bounding box, in world pixel coordinates.
        /// @return The top Y position of the tile.
        float GetTopYPosition() const;
        /// @brief  Gets the bottom Y position of the tile's bounding box, in world pixel coordinates.
        /// @return The bottom Y position of the tile.
        float GetBottomYPosition() const;

    private:
        /// @brief      Helper method for copying a tile.
        /// @param[in]  tile_to_copy - The tile to copy.
        void Copy(const Tile& tile_to_copy);

        unsigned int TileId;  ///< The numeric ID for the tile in a tile set.
        std::shared_ptr<GRAPHICS::Sprite> Sprite;   ///< The sprite used for rendering the tile.
    };
}