#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/GraphicsSystem.h"
#include "Maps/Tile.h"
#include "Maps/TilesetDescription.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A collection of tile data from which tiles can be created.
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

        /// Sets the data in the set for the tile with the specified ID.
        /// @param[in]  tile_id - The unique ID of the tile.
        /// @param[in]  texture - The texture with graphics resources for the tile.
        /// @param[in]  texture_sub_rectangle - The sub-rectangle within
        ///     the texture holding graphics for the tile.
        void SetTile(
            const TileId tile_id, 
            const std::shared_ptr<GRAPHICS::Texture>& texture,
            const sf::IntRect& texture_sub_rectangle);

        /// Creates an unpositioned tile based on the data in the tileset
        /// identified by the given tile ID.
        /// @param[in]  tile_id - The ID of the type of tile to create.
        /// @return The tile, if successfully created; null otherwise.
        std::shared_ptr<Tile> CreateTile(const TileId tile_id) const;

    private:

        /// Textures for tiles mapped by tile ID.
        std::unordered_map< TileId, std::shared_ptr<GRAPHICS::Texture> > TileTextures;
        /// Sub-rectangles within textures for tiles mapped by tile ID.
        std::unordered_map<TileId, sf::IntRect> TileTextureSubRectangles;
    };
}