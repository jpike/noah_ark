#include <stdexcept>
#include "Maps/TileMap.h"

namespace MAPS
{
    TileMap::TileMap(
        const OverworldGridPosition& overworld_grid_position,
        const MATH::Vector2f& top_left_world_position_in_pixels,
        const std::shared_ptr<ITileMapData>& map_data,
        std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system) :
    Tiles(),
    MapData(map_data),
    OverworldPosition(overworld_grid_position),
    TopLeftWorldPositionInPixels(top_left_world_position_in_pixels)
    {
        // MAKE MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != MapData);
        if (!map_data_exists)
        {
            throw std::runtime_error("Cannot construct tile map with null map data.");
        }

        // MAKE SURE A GRAPHICS SYSTEM WAS PROVIDED.
        bool graphics_system_exists = (nullptr != graphics_system);
        if (!graphics_system_exists)
        {
            throw std::runtime_error("Cannot construct tile map with null graphics system.");
        }

        // BUILD THIS TILEMAP FROM THE MAP DATA.
        BuildFromMapData(TopLeftWorldPositionInPixels, *MapData, *graphics_system);
    }

    TileMap::~TileMap()
    {
        // Nothing else to do.
    }

    OverworldGridPosition TileMap::GetOverworldGridPosition() const
    {
        return OverworldPosition;
    }

    MATH::Vector2f TileMap::GetTopLeftWorldPosition() const
    {
        return TopLeftWorldPositionInPixels;
    }

    MATH::Vector2f TileMap::GetBottomRightWorldPosition() const
    {
        // CALCULATE THE BOTTOM RIGHT POSITION FROM OTHER TILE MAP PROPERTIES.
        // Calculate the bottom position.
        float map_height_in_tiles = static_cast<float>(GetHeightInTiles());
        float tile_height_in_pixels = static_cast<float>(GetTileHeightInPixels());
        float map_height_in_pixels = map_height_in_tiles * tile_height_in_pixels;
        float bottom_world_position = TopLeftWorldPositionInPixels.Y + map_height_in_pixels;

        // Calculate the right position.
        float map_width_in_tiles = static_cast<float>(GetWidthInTiles());
        float tile_width_in_pixels = static_cast<float>(GetTileWidthInPixels());
        float map_width_in_pixels = map_width_in_tiles * tile_width_in_pixels;
        float right_world_position = TopLeftWorldPositionInPixels.X + map_width_in_pixels;

        // Return the bottom right position.
        MATH::Vector2f bottom_right_world_position(right_world_position, bottom_world_position);
        return bottom_right_world_position;
    }

    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        // GET THE BOUNDING CORNERS OF THE TILE MAP.
        MATH::Vector2f top_left_world_position = GetTopLeftWorldPosition();
        MATH::Vector2f bottom_right_world_position = GetBottomRightWorldPosition();

        // CALCULATE THE CENTER USING THE MIDPOINT FORMULA.
        float center_x_position = (top_left_world_position.X + bottom_right_world_position.X) / 2.0f;
        float center_y_position = (top_left_world_position.Y + bottom_right_world_position.Y) / 2.0f;

        MATH::Vector2f center_position(center_x_position, center_y_position);
        return center_position;
    }

    unsigned int TileMap::GetWidthInTiles() const
    {
        return MapData->GetWidthInTiles();
    }

    unsigned int TileMap::GetHeightInTiles() const
    {
        return MapData->GetHeightInTiles();
    }

    unsigned int TileMap::GetTileWidthInPixels() const
    {
        return MapData->GetTileWidthInPixels();
    }

    unsigned int TileMap::GetTileHeightInPixels() const
    {
        return MapData->GetTileHeightInPixels();
    }

    MATH::Vector2f TileMap::GetSizeInPixels() const
    {
        // GET THE WIDTH IN PIXELS.
        float map_width_in_tiles = static_cast<float>(GetWidthInTiles());
        float tile_width_in_pixels = static_cast<float>(GetTileWidthInPixels());
        float map_width_in_pixels = map_width_in_tiles * tile_width_in_pixels;

        // GET THE HEIGHT IN PIXELS.
        float map_height_in_tiles = static_cast<float>(GetHeightInTiles());
        float tile_height_in_pixels = static_cast<float>(GetTileHeightInPixels());
        float map_height_in_pixels = map_height_in_tiles * tile_height_in_pixels;

        // RETURN THE MAP SIZE IN PIXELS.
        MATH::Vector2f size_in_pixels(map_width_in_pixels, map_height_in_pixels);
        return size_in_pixels;
    }

    std::shared_ptr<Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        // CHECK IF THE WORLD POSITION IS WITHIN THE BOUNDS OF THIS TILE MAP.
        MATH::Vector2f center_world_position = GetCenterWorldPosition();
        MATH::Vector2f size_in_pixels = GetSizeInPixels();
        MATH::FloatRectangle tile_map_bounding_rect(
            center_world_position.X,
            center_world_position.Y,
            size_in_pixels.X,
            size_in_pixels.Y);
        bool tile_position_within_map = tile_map_bounding_rect.Contains(world_x_position, world_y_position);
        if (!tile_position_within_map)
        {
            // The requested tile position isn't within this map.
            return nullptr;
        }

        // ADJUST THE WORLD POSITIONS TO BE RELATIVE TO THIS TILE MAP.
        // The top-left corner of this map will serve as the new origin.
        MATH::Vector2f top_left_map_world_position = GetTopLeftWorldPosition();
        float relative_tile_x_position = world_x_position - top_left_map_world_position.X;
        float relative_tile_y_position = world_y_position - top_left_map_world_position.Y;

        // CALCULATE THE GRID COORDINATES OF THE REQUESTED TILE.
        unsigned int tile_x_grid_position = static_cast<unsigned int>(relative_tile_x_position / GetTileWidthInPixels());
        unsigned int tile_y_grid_position = static_cast<unsigned int>(relative_tile_y_position / GetTileHeightInPixels());

        // VALIDATE THE GRID COORDINATES OF THE TILE.
        // Validate the X coordinate.
        bool valid_tile_x_grid_position = (tile_x_grid_position < Tiles.size());
        if (!valid_tile_x_grid_position)
        {
            // The tile grid position isn't valid, so no tile can be retrieved.
            return nullptr;
        }

        // Validate the Y coordinate.
        bool valid_tile_y_grid_position = (tile_y_grid_position < Tiles[tile_x_grid_position].size());
        if (!valid_tile_y_grid_position)
        {
            // The tile grid position isn't valid, so no tile can be retrieved.
            return nullptr;
        }

        // GET THE TILE AT THE SPECIFIED GRID COORDINATES.
        return Tiles[tile_x_grid_position][tile_y_grid_position];
    }

    void TileMap::BuildFromMapData(
        const MATH::Vector2f& top_left_world_position_in_pixels,
        const ITileMapData& map_data,
        GRAPHICS::GraphicsSystem& graphics_system)
    {
        // VERIFY THAT THE EXPECTED NUMBER OF LAYERS EXIST IN THE MAP.
        // For now, we only support a single ground layer.
        const int EXPECTED_LAYER_COUNT = 1;
        std::vector<TileMapLayerDescription> layer_descriptions = map_data.GetLayers();
        bool expected_layers_provided = (EXPECTED_LAYER_COUNT == layer_descriptions.size());
        if (!expected_layers_provided)
        {
            // Don't populate the map if it isn't properly formed.
            return;
        }

        // VERIFY THAT THE EXPECTED NUMBER OF TILESETS EXIST IN THE MAP.
        // For now, we only support a single tileset.
        const int EXPECTED_TILESET_COUNT = 1;
        std::vector<TilesetDescription> tileset_descriptions = map_data.GetTilesets();
        bool expected_tilesets_provided = (EXPECTED_TILESET_COUNT == tileset_descriptions.size());
        if (!expected_tilesets_provided)
        {
            // Don't populate the map if it isn't properly formed.
            return;
        }

        // GET THE LAYER TO USE FOR BUILDING THE MAP.
        const TileMapLayerDescription& map_layer = layer_descriptions.front();

        // GET THE TILESET TEXTURE NEEDED FOR CREATING GRAPHICS FOR THE TILES.
        const TilesetDescription& tileset_description = tileset_descriptions.front();
        std::shared_ptr<GRAPHICS::Texture> tileset_texture = graphics_system.GetTexture(tileset_description.Name);
        bool tileset_texture_exists = (nullptr != tileset_texture);
        if (!tileset_texture_exists)
        {
            return;
        }

        // GET THE TILESET IMAGE INFORMATION SO THAT WE KNOW HOW TO CONVERT A TILE ID TO PIXEL COORDINATES OF A TILE.
        MATH::Vector2ui tileset_image_size = tileset_texture->GetSize();
        unsigned int tileset_image_width_in_pixels = tileset_image_size.X;
        unsigned int tileset_image_height_in_pixels = tileset_image_size.Y;

        // POPULATE THE MAP FROM THE ONLY LAYER.
        unsigned int layer_width_in_tiles = map_data.GetWidthInTiles();
        unsigned int layer_height_in_tiles = map_data.GetHeightInTiles();
        // Reserve space in the container for all columns of tiles to avoid later reallocations.
        Tiles.resize(layer_width_in_tiles);
        for (unsigned int x_position_in_tiles = 0; x_position_in_tiles < layer_width_in_tiles; ++x_position_in_tiles)
        {
            // Reserve space for all rows of tiles in the current row to avoid later reallocations.
            Tiles.at(x_position_in_tiles).resize(layer_height_in_tiles);
            for (unsigned int y_position_in_tiles = 0; y_position_in_tiles < layer_height_in_tiles; ++y_position_in_tiles)
            {
                // GET INFORMATION NEEDED TO CREATE THE TILE.
                /// @todo   Correct things so an adjustment by -1 isn't necessary.
                /// This issue popped up in the switch from TMX to JSON file formats.
                TileId tile_id = map_layer.TileIds(x_position_in_tiles, y_position_in_tiles) - 1;

                unsigned int tile_width_in_pixels = tileset_description.TileWidthInPixels;
                unsigned int tile_height_in_pixels = tileset_description.TileHeightInPixels;

                /// @todo   Move this logic to a helper function.
                float tileset_x_position = static_cast<float>((tile_width_in_pixels * tile_id) % tileset_image_width_in_pixels);
                unsigned int tiles_per_row = (tileset_image_width_in_pixels / tile_width_in_pixels);
                float tileset_y_position = static_cast<float>((tile_height_in_pixels * (tile_id / tiles_per_row)) % tileset_image_height_in_pixels);

                // CREATE A SPRITE FOR THE TILE.
                std::shared_ptr<GRAPHICS::Sprite> tile_sprite = graphics_system.CreateSprite(
                    tileset_texture,
                    tileset_x_position,
                    tileset_y_position,
                    static_cast<float>(tile_width_in_pixels),
                    static_cast<float>(tile_height_in_pixels),
                    GRAPHICS::GraphicsLayer::GROUND);

                // SET ADDITIONAL PROPERTIES OF THE TILE'S SPRITE.
                // Set the sprite's world position.
                float worldXPosition = static_cast<float>(tile_width_in_pixels * x_position_in_tiles) + top_left_world_position_in_pixels.X;
                // Position it based on its center.
                /// @todo   Re-examine commented-out code.
                /*float tileHalfWidth = static_cast<float>(tileWidthInPixels) / 2.0f;
                worldXPosition += tileHalfWidth;*/
                float worldYPosition = static_cast<float>(tile_height_in_pixels * y_position_in_tiles) + top_left_world_position_in_pixels.Y;
                // Position it based on its center.
                /*float tileHalfHeight = static_cast<float>(tileHeightInPixels) / 2.0f;
                worldYPosition += tileHalfHeight;*/
                tile_sprite->SetWorldPosition(worldXPosition, worldYPosition);

                // STORE A TILE IN THE COLLECTION OF TILES.
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tile_id, tile_sprite);
                Tiles[x_position_in_tiles][y_position_in_tiles] = tile;
            }
        }
    }
}