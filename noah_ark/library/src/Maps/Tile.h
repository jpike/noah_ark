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
        /// Copy constructor.
        /// @param[in]  tile_to_copy - The tile to copy.
        explicit Tile(const Tile& tile_to_copy);
        /// Destructor.
        ~Tile();

        /// Assignment operator.
        /// @param[in]  rhs_tile - The tile to assign from.
        Tile& operator= (const Tile& rhs_tile);

        /// Creates a deep copy of this tile, except for
        /// any underlying texture resource.
        /// @return A deep copy of this tile.
        std::shared_ptr<Tile> Clone() const;

        /// Determines if the tile is walkable or not.
        /// @return Whether or not this tile is walkable.
        bool IsWalkable() const;

        /// Gets the width of the tile, in pixels.
        /// @return The width of the tile in pixels.
        float GetWidthInPixels() const;
        /// Gets the height of the tile, in pixels.
        /// @return The height of the tile in pixels.
        float GetHeightInPixels() const;
        
        /// Gets the left X position of the tile's bounding box, in world pixel coordinates.
        /// @return The left X position of the tile.
        float GetLeftXPosition() const;
        /// Gets the right X posision of the tile's bounding box, in world pixel coordinates.
        /// @return The right X position of the tile.
        float GetRightXPosition() const;

        /// Gets the top Y position of the tile's bounding box, in world pixel coordinates.
        /// @return The top Y position of the tile.
        float GetTopYPosition() const;
        /// Gets the bottom Y position of the tile's bounding box, in world pixel coordinates.
        /// @return The bottom Y position of the tile.
        float GetBottomYPosition() const;

        /// Sets the top-left world position of the tile, in pixels.
        /// @param[in]  top_world_position - The top world position of the tile.
        /// @param[in]  left_world_position - The left world position of the tile.
        void SetTopLeftWorldPosition(
            const float top_world_position,
            const float left_world_position);

        /// Renders the tile to the specified render target.
        /// @param[in,out]  renderTarget - The target to render to.
        void Render(sf::RenderTarget& render_target);

    private:
        /// Helper method for copying a tile.
        /// @param[in]  tile_to_copy - The tile to copy.
        void Copy(const Tile& tile_to_copy);

        unsigned int TileId;  ///< The numeric ID for the tile in a tile set.
        std::shared_ptr<GRAPHICS::Sprite> Sprite;   ///< The sprite used for rendering the tile.
    };
}