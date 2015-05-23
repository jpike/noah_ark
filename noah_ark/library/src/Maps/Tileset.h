#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/GraphicsSystem.h"
#include "Maps/Tile.h"
#include "Maps/TilesetDescription.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A collection of tiles.
    class Tileset
    {
    public:
        /// Constructor to create an empty tileset.
        explicit Tileset();

        /// Populates the tileset based on the provided descriptions.
        /// Any previous data in this tileset will be cleared first.
        /// @param[in]  tileset_descriptions - Descriptions of tilesets to include in this tileset.
        /// @param[in,out]  graphics_system - Supplies graphics data for tiles in the set.
        void Populate(
            const std::vector<TilesetDescription>& tileset_descriptions,
            GRAPHICS::GraphicsSystem& graphics_system);

        /// Gets the tile in the set with the specified ID.
        /// @param[in]  tile_id - The ID of the tile to get.
        /// @return The tile with the specified ID or null if
        ///     no tile exists in this set with the specified ID.
        std::shared_ptr<Tile> GetTile(const TileId tile_id) const;

    private:

        /// @todo   Figure how how to deal with transparent colors?
        /// The tiles in the tileset.
        std::unordered_map< TileId, std::shared_ptr<Tile> > Tiles;
    };
}