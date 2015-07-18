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
        std::shared_ptr<GRAPHICS::Sprite> sprite = std::make_shared<GRAPHICS::Sprite>(sprite_resource);

        // CREATE THE TILE.
        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tile_id, sprite);
        return tile;
    }

    void Tileset::Populate(
        const std::vector<TilesetDescription>& tileset_descriptions,
        GRAPHICS::GraphicsSystem& graphics_system)
    {
        // CLEAR ANY OLD TILE DATA.
        TileTextures.clear();
        TileTextureSubRectangles.clear();

        // POPULATE THIS TILESET FROM EACH DESCRIPTION.
        for (const auto& tileset_description : tileset_descriptions)
        {
            // LOAD THE TEXTURE FOR THE CURRENT DESCRIPTION.
            std::shared_ptr<GRAPHICS::Texture> tileset_texture = graphics_system.GetTexture(tileset_description.Name);
            bool texture_loaded = (nullptr != tileset_texture);
            if (!texture_loaded)
            {
                // SKIP TO THE NEXT TILESET DESCRIPTION.
                continue;
            }

            // CREATE AND STORE EACH TILE IN THE CURRENT DESCRIPTION.
            TileId current_tile_id = tileset_description.FirstTileId;
            MATH::Vector2ui tileset_texture_dimensions = tileset_texture->GetSize();
            /// @todo   Avoid truncation?
            unsigned int row_count_of_tiles = tileset_texture_dimensions.Y / tileset_description.TileHeightInPixels;
            unsigned int column_count_of_tiles = tileset_texture_dimensions.X / tileset_description.TileWidthInPixels;
            for (unsigned int tile_row_index = 0;
                tile_row_index < row_count_of_tiles;
                ++tile_row_index)
            {
                for (unsigned int tile_column_index = 0;
                    tile_column_index < column_count_of_tiles;
                    ++tile_column_index)
                {
                    // CALCULATE THE OFFSET WITHIN THE TEXTURE FOR THE CURRENT TILE.
                    int tile_left_texture_offset_in_texels = tile_column_index * tileset_description.TileWidthInPixels;
                    int tile_top_texture_offset_in_texels = tile_row_index * tileset_description.TileHeightInPixels;

                    // CREATE A SPRITE FOR THE CURRENT TILE.
                    sf::IntRect tile_texture_rect(
                        tile_left_texture_offset_in_texels,
                        tile_top_texture_offset_in_texels,
                        tileset_description.TileWidthInPixels,
                        tileset_description.TileHeightInPixels);

                    // STORE THE CURRENT TILE.
                    SetTile(current_tile_id, tileset_texture, tile_texture_rect);

                    // UPDATE THE TILE ID FOR THE NEXT TILE.
                    ++current_tile_id;
                }
            }
        }
    }
}