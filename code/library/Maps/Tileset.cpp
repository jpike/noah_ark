#include "Core/NullChecking.h"
#include "Maps/Tileset.h"

namespace MAPS
{
    /// Creates a tileset.
    /// @param[in]  tileset_texture - The texture to use for the tileset; must not be null.
    /// @throws std::exception - Thrown if the texture is null.
    Tileset::Tileset(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture)
    {
        // MAKE SURE THE TEXTURE EXISTS.
        CORE::ThrowInvalidArgumentExceptionIfNull(tileset_texture, "Tileset texture must not be null.");
        
        // POPULATE A TILESET FROM THE TEXTURE.
        const MAPS::TileId FIRST_TILE_ID = 1;
        MAPS::TileId current_tile_id = FIRST_TILE_ID;
        MATH::Vector2ui tileset_texture_dimensions = tileset_texture->GetSize();
        unsigned int row_count_of_tiles = tileset_texture_dimensions.Y / Tile::DIMENSION_IN_PIXELS<unsigned int>;
        unsigned int column_count_of_tiles = tileset_texture_dimensions.X / Tile::DIMENSION_IN_PIXELS<unsigned int>;
        for (unsigned int tile_row_index = 0;
            tile_row_index < row_count_of_tiles;
            ++tile_row_index)
        {
            for (unsigned int tile_column_index = 0;
                tile_column_index < column_count_of_tiles;
                ++tile_column_index)
            {
                // CALCULATE THE OFFSET WITHIN THE TEXTURE FOR THE CURRENT TILE.
                int tile_left_texture_offset_in_texels = tile_column_index * Tile::DIMENSION_IN_PIXELS<unsigned int>;
                int tile_top_texture_offset_in_texels = tile_row_index * Tile::DIMENSION_IN_PIXELS<unsigned int>;

                // CREATE A SPRITE FOR THE CURRENT TILE.
                MATH::FloatRectangle tile_texture_rect = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    static_cast<float>(tile_left_texture_offset_in_texels),
                    static_cast<float>(tile_top_texture_offset_in_texels),
                    Tile::DIMENSION_IN_PIXELS<float>,
                    Tile::DIMENSION_IN_PIXELS<float>);

                // STORE THE CURRENT TILE.
                SetTile(current_tile_id, tileset_texture, tile_texture_rect);

                // UPDATE THE TILE ID FOR THE NEXT TILE.
                ++current_tile_id;
            }
        }
    }

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