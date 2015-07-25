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

        /// Retrieves a tile at the offset in the ground layer.
        /// @param[in]  x_offset_from_left_in_tiles - The x offset from the left
        ///     of the layer, in tiles, of the tile to retrieve.
        /// @param[in]  y_offset_from_top_in_tiles - The y offset from the top
        ///     of the layer, in tiles, of the tile to retrieve.
        /// @return The tile at the specified position or null
        ///     if no tile exists at the position.
        std::shared_ptr<MAPS::Tile> GetTile(
            const unsigned int x_offset_from_left_in_tiles,
            const unsigned int y_offset_from_top_in_tiles) const;

        /// Gets all renderable tiles within the ground layer.
        /// @return The renderable versions of tiles in this layer.
        std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > GetRenderables() const;

        /// Gets the width and height of the ground layer, in pixels.
        /// @return The size of the ground area.
        MATH::Vector2f GetSizeInPixels() const;

    private:

        /// The world position of the center of the layer.
        MATH::Vector2f CenterWorldPosition;
        /// The dimensions of the layer in tiles.
        MATH::Vector2ui DimensionsInTiles;
        /// The tiles in this layer.
        CORE::Array2D< std::shared_ptr<MAPS::Tile> > Tiles;
    };
}