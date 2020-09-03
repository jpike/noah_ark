#include "Maps/Tileset.h"

namespace MAPS
{
    /// Creates a tileset.
    Tileset::Tileset()
    {
        // POPULATE A TILESET FROM THE TEXTURE.
        const MAPS::TileId FIRST_TILE_ID = 1;
        MAPS::TileId current_tile_id = FIRST_TILE_ID;
        // This dimensions are hardcoded to avoid needing to have to pass the actual texture resource here.
        MATH::Vector2ui tileset_texture_dimensions(128, 48);
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
                SetTile(current_tile_id, tile_texture_rect);

                // UPDATE THE TILE ID FOR THE NEXT TILE.
                ++current_tile_id;
            }
        }
    }

    /// Sets the data in the set for the tile with the specified ID.
    /// @param[in]  tile_id - The unique ID of the tile.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle within
    ///     the texture holding graphics for the tile.
    void Tileset::SetTile(
        const TileId tile_id,
        const MATH::FloatRectangle& texture_sub_rectangle)
    {
        TileTextureSubRectangles[tile_id] = texture_sub_rectangle;
    }

    /// Creates an unpositioned tile based on the data in the tileset
    /// identified by the given tile ID.
    /// @param[in]  tile_id - The ID of the type of tile to create.
    /// @return The tile, if successfully created; null otherwise.
    std::shared_ptr<Tile> Tileset::CreateTile(const TileId tile_id) const
    {
        // GET THE TEXTURE SUB-RECTANGLE FOR THE TILE.
        auto id_with_sub_rectangle = TileTextureSubRectangles.find(tile_id);
        bool sub_rectangle_exists = (TileTextureSubRectangles.cend() != id_with_sub_rectangle);
        if (!sub_rectangle_exists)
        {
            return nullptr;
        }

        // CREATE A SPRITE FOR THE TILE.
        GRAPHICS::AnimatedSprite sprite(GRAPHICS::Sprite(
            RESOURCES::AssetId::MAIN_TILESET_TEXTURE,
            id_with_sub_rectangle->second));

        // ADD ANIMATION SEQUENCES FOR WATER TILES.
        bool is_water_tile = TileType::IsForWater(static_cast<TileType::Id>(tile_id));
        if (is_water_tile)
        {
            const std::string WATER_ANIMATION_NAME = "WaterAnimation";
            const bool IS_LOOPING = true;
            const sf::Time ANIMATION_DURATION = sf::seconds(1.2f);
            const std::vector<MATH::IntRectangle> ANIMATION_FRAMES = 
            {
                MATH::IntRectangle::FromLeftTopAndDimensions(64, 0, 16, 16),
                MATH::IntRectangle::FromLeftTopAndDimensions(16, 32, 16, 16),
                MATH::IntRectangle::FromLeftTopAndDimensions(32, 32, 16, 16)
            };
            auto water_animation_sequence = std::make_shared<GRAPHICS::AnimationSequence>(
                WATER_ANIMATION_NAME,
                IS_LOOPING,
                ANIMATION_DURATION,
                ANIMATION_FRAMES);
            sprite.AddAnimationSequence(water_animation_sequence);
            sprite.UseAnimationSequence(WATER_ANIMATION_NAME);
        }

        // CREATE THE TILE.
        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tile_id, sprite);
        return tile;
    }
}