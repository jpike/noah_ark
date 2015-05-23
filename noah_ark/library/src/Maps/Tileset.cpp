#include "Maps/Tileset.h"

namespace MAPS
{
    Tileset::Tileset() :
    Tiles()
    {}

    void Tileset::Populate(
        const std::vector<TilesetDescription>& tileset_descriptions,
        GRAPHICS::GraphicsSystem& graphics_system)
    {
        // CLEAR ANY OLD TILE DATA.
        Tiles.clear();

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
            unsigned int column_count_of_tiles = tileset_texture_dimensions.X / tileset_description.TileWidthInPixels;
            unsigned int row_count_of_tiles = tileset_texture_dimensions.Y / tileset_description.TileHeightInPixels;
            for (unsigned int tile_column_index = 0;
                tile_column_index < column_count_of_tiles;
                ++tile_column_index)
            {
                for (unsigned int tile_row_index = 0;
                    tile_row_index < row_count_of_tiles;
                    ++tile_row_index)
                {
                    // CALCULATE THE OFFSET WITHIN THE TEXTURE FOR THE CURRENT TILE.
                    float tile_left_texture_offset_in_texels = static_cast<float>(tile_column_index * tileset_description.TileWidthInPixels);
                    float tile_top_texture_offset_in_texels = static_cast<float>(tile_row_index * tileset_description.TileHeightInPixels);

                    // CREATE A SPRITE FOR THE CURRENT TILE.
                    std::shared_ptr<GRAPHICS::Sprite> tile_sprite = GRAPHICS::Sprite::Create(
                        tileset_texture,
                        tile_left_texture_offset_in_texels,
                        tile_top_texture_offset_in_texels,
                        static_cast<float>(tileset_description.TileWidthInPixels),
                        static_cast<float>(tileset_description.TileHeightInPixels));
                    bool tile_sprite_created = (nullptr != tile_sprite);
                    if (!tile_sprite_created)
                    {
                        // SKIP TO TRY CREATING THE NEXT TILE.
                        ++current_tile_id;
                        continue;
                    }

                    // STORE THE CURRENT TILE.
                    Tiles[current_tile_id] = std::make_shared<Tile>(current_tile_id, tile_sprite);

                    // UPDATE THE TILE ID FOR THE NEXT TILE.
                    ++current_tile_id;
                }
            }
        }
    }

    std::shared_ptr<Tile> Tileset::GetTile(const TileId tile_id) const
    {
        auto id_with_tile = Tiles.find(tile_id);
        bool tile_found = (Tiles.cend() != id_with_tile);
        if (tile_found)
        {
            return id_with_tile->second;
        }
        else
        {
            return nullptr;
        }
    }
}