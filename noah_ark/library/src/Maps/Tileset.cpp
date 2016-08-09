#include "Maps/Tileset.h"

namespace MAPS
{
    /// Sets the data in the set for the tile with the specified ID.
    /// @param[in]  tile_id - The unique ID of the tile.
    /// @param[in]  texture - The texture with graphics resources for the tile.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle within
    ///     the texture holding graphics for the tile.
    void Tileset::SetTile(
        const TileId tile_id,
        const std::shared_ptr<GRAPHICS::Texture>& texture,
        const MATH::FloatRectangle& texture_sub_rectangle)
    {
        TileTextures[tile_id] = texture;
        TileTextureSubRectangles[tile_id] = texture_sub_rectangle;
    }

    /// Creates an unpositioned tile based on the data in the tileset
    /// identified by the given tile ID.
    /// @param[in]  tile_id - The ID of the type of tile to create.
    /// @return The tile, if successfully created; null otherwise.
    std::shared_ptr<Tile> Tileset::CreateTile(const TileId tile_id) const
    {
        // GET THE TEXTURE FOR THE TILE.
        auto id_with_texture = TileTextures.find(tile_id);
        bool texture_exists = (TileTextures.cend() != id_with_texture);
        if (!texture_exists)
        {
            return nullptr;
        }

        // GET THE TEXTURE SUB-RECTANGLE FOR THE TILE.
        auto id_with_sub_rectangle = TileTextureSubRectangles.find(tile_id);
        bool sub_rectangle_exists = (TileTextureSubRectangles.cend() != id_with_sub_rectangle);
        if (!sub_rectangle_exists)
        {
            return nullptr;
        }

        // CREATE A SPRITE FOR THE TILE.
        GRAPHICS::Sprite sprite(
            id_with_texture->second,
            id_with_sub_rectangle->second);

        // CREATE THE TILE.
        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tile_id, sprite);
        return tile;
    }
}