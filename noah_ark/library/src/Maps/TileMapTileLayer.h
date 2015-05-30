#pragma once

#include <string>
#include "Core/Array2D.h"
#include "Maps/ITileMapLayer.h"
#include "Maps/Tile.h"
#include "Maps/Tileset.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A single layer of tiles in a tile map.
    class TileMapTileLayer : public ITileMapLayer
    {
    public:
        /// Constructor.
        /// @param[in]  name - A name for the layer.
        /// @param[in]  top_left_world_position - The top left position
        ///     of where this tile map layer is positioned within the world.
        /// @param[in]  tile_ids - IDs for tiles in this layer.
        /// @param[in]  tileset - The set of tiles used for this layer.
        explicit TileMapTileLayer(
            const std::string& name,
            const MATH::Vector2f& top_left_world_position,
            const CORE::Array2D<TileId>& tile_ids,
            const MAPS::Tileset& tileset);
        /// Destructor.
        virtual ~TileMapTileLayer();

        /// @copydoc    ITileMapLayer::GetTileAtGridPosition
        virtual std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position,
            const unsigned int tile_y_grid_position) const;

        /// @copydoc    ITileMapLayer::Render
        virtual void Render(sf::RenderTarget& render_target);

    private:
        /// Creates an array of tiles for this layer.
        /// @param[in]  top_left_world_position - The top left position
        ///     of where this tile map layer is positioned within the world.
        /// @param[in]  tile_ids - IDs for tiles in this layer.
        /// @param[in]  tileset - The set of tiles used for this layer.
        /// @return The tiles for this layer arranged in a grid
        ///     based on their positions.
        CORE::Array2D< std::shared_ptr<Tile> > CreateTiles(
            const MATH::Vector2f& top_left_world_position,
            const CORE::Array2D<TileId>& tile_ids,
            const MAPS::Tileset& tileset) const;

        /// The name of the layer.  Primarily used for debugging purposes.
        std::string Name;
        /// The tiles in this layer.
        CORE::Array2D< std::shared_ptr<Tile> > Tiles;
    };
}
