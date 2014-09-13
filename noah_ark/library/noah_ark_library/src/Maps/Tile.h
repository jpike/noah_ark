#pragma once

#include <memory>
#include "Graphics/Sprite.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A 2D graphical tile intended to be used as a building block for tile maps.
    ///////////////////////////////////////////////////////////
    class Tile
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  tileId - A numeric ID identifying the tile in a tileset.
        /// @param[in]  sprite - The sprite resource used for rendering the tile.
        explicit Tile(
            const unsigned int tileId,
            const std::shared_ptr<GRAPHICS::Sprite>& sprite);
        /// @brief      Copy constructor.
        /// @param[in]  tileToCopy - The tile to copy.
        explicit Tile(const Tile& tileToCopy);
        /// @brief  Destructor.
        ~Tile();

        /// @brief      Assignment operator.
        /// @param[in]  rhsTile - The tile to assign from.
        Tile& operator= (const Tile& rhsTile);

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
        /// @param[in]  tileToCopy - The tile to copy.
        void Copy(const Tile& tileToCopy);

        unsigned int m_tileId;  ///< The numeric ID for the tile in a tile set.
        std::shared_ptr<GRAPHICS::Sprite> m_sprite;   ///< The sprite used for rendering the tile.
    };
}