#include "Maps/Tileset.h"

namespace MAPS
{
    Tileset::Tileset() :
    TileTextures(),
    TileTextureSubRectangles()
    {}

    void Tileset::SetTile(
        const TileId tile_id,
        const std::shared_ptr<GRAPHICS::Texture>& texture,
        const sf::IntRect& texture_sub_rectangle)
    {
        TileTextures[tile_id] = texture;
        TileTextureSubRectangles[tile_id] = texture_sub_rectangle;
    }

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
        /// @todo   This needs to be cleaner.
        std::shared_ptr<sf::Sprite> sprite_resource = std::make_shared<sf::Sprite>(
            *id_with_texture->second->GetTextureResource(),
            id_with_sub_rectangle->second);
        GRAPHICS::Sprite sprite(sprite_resource);

        // CREATE THE TILE.
        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tile_id, sprite);
        return tile;
    }
}