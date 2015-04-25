#include "Maps/TileMapBuilder.h"

namespace MAPS
{
    TileMapBuilder::TileMapBuilder(const std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system) :
        GraphicsSystem(graphics_system)
    {}

    TileMapBuilder::~TileMapBuilder()
    {}

    std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildTileMap(
        const OverworldGridPosition& overworld_grid_position,
        const MATH::Vector2f& top_left_world_position_in_pixels,
        const std::shared_ptr<ITileMapData>& map_data)
    {
        std::shared_ptr<MAPS::TileMap> tile_map = std::make_shared<MAPS::TileMap>(
            overworld_grid_position,
            top_left_world_position_in_pixels,
            map_data,
            GraphicsSystem);

        return tile_map;
    }

    std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildTopTileMap(
        const MAPS::TileMap& center_map,
        const std::shared_ptr<ITileMapData>& top_map_data)
    {
        // CHECK IF MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != top_map_data);
        if (!map_data_exists)
        {
            // No tile map can be constructed without map data.
            return nullptr;
        }

        // CALCULATE THE GRID POSITION OF THE TOP MAP.
        MATH::Vector2ui center_tile_map_overwrld_grid_position = center_map.GetOverworldGridPosition();
        MATH::Vector2ui top_tile_map_overworld_grid_position = center_tile_map_overwrld_grid_position;
        top_tile_map_overworld_grid_position.Y--;

        // CALCULATE THE WORLD POSITION OF THE TOP MAP.
        float top_map_height_in_tiles = static_cast<float>(top_map_data->GetHeightInTiles());
        float top_map_tile_height_in_pixels = static_cast<float>(top_map_data->GetTileHeightInPixels());
        float top_map_height_in_pixels = top_map_height_in_tiles * top_map_tile_height_in_pixels;
        MATH::Vector2f center_tile_map_top_left_world_position = center_map.GetTopLeftWorldPosition();
        float topMapWorldYPosition = center_tile_map_top_left_world_position.Y - top_map_height_in_pixels;
        MATH::Vector2f top_tile_map_top_left_world_position(
            center_tile_map_top_left_world_position.X,
            topMapWorldYPosition);

        // CREATE THE TOP TILE MAP.
        std::shared_ptr<MAPS::TileMap> top_tile_map = std::make_shared<MAPS::TileMap>(
            top_tile_map_overworld_grid_position,
            top_tile_map_top_left_world_position,
            top_map_data,
            GraphicsSystem);
        return top_tile_map;
    }

    std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildBottomTileMap(
        const MAPS::TileMap& center_map,
        const std::shared_ptr<ITileMapData>& bottom_map_data)
    {
        // CHECK IF MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != bottom_map_data);
        if (!map_data_exists)
        {
            // No tile map can be constructed without map data.
            return nullptr;
        }

        // CALCULATE THE GRID POSITION OF THE BOTTOM MAP.
        MATH::Vector2ui center_tile_map_overworld_grid_position = center_map.GetOverworldGridPosition();
        MATH::Vector2ui bottom_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        bottom_tile_map_overworld_grid_position.Y++;

        // CALCULATE THE WORLD POSITION OF THE BOTTOM MAP.
        float center_map_height_in_tiles = static_cast<float>(center_map.GetHeightInTiles());
        float center_map_tile_height_in_pixels = static_cast<float>(center_map.GetTileHeightInPixels());
        float center_map_height_in_pixels = center_map_height_in_tiles * center_map_tile_height_in_pixels;
        MATH::Vector2f center_tile_map_top_left_world_position = center_map.GetTopLeftWorldPosition();
        float bottom_map_world_y_position = center_tile_map_top_left_world_position.Y + center_map_height_in_pixels;
        MATH::Vector2f bottom_tile_map_top_left_world_position(
            center_tile_map_top_left_world_position.X,
            bottom_map_world_y_position);

        // CREATE THE BOTTOM TILE MAP.
        std::shared_ptr<MAPS::TileMap> bottom_tile_map = std::make_shared<MAPS::TileMap>(
            bottom_tile_map_overworld_grid_position,
            bottom_tile_map_top_left_world_position,
            bottom_map_data,
            GraphicsSystem);
        return bottom_tile_map;
    }

    std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildLeftTileMap(
        const MAPS::TileMap& center_map,
        const std::shared_ptr<ITileMapData>& left_map_data)
    {
        // CHECK IF MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != left_map_data);
        if (!map_data_exists)
        {
            // No tile map can be constructed without map data.
            return nullptr;
        }

        // CALCULATE THE GRID POSITION OF THE LEFT MAP.
        MATH::Vector2ui center_tile_map_overworld_grid_position = center_map.GetOverworldGridPosition();
        MATH::Vector2ui left_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        left_tile_map_overworld_grid_position.X--;

        // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
        float left_map_width_in_tiles = static_cast<float>(left_map_data->GetWidthInTiles());
        float left_map_tile_width_in_pixels = static_cast<float>(left_map_data->GetTileWidthInPixels());
        float left_map_width_in_pixels = left_map_width_in_tiles * left_map_tile_width_in_pixels;
        MATH::Vector2f center_tile_map_top_left_world_position = center_map.GetTopLeftWorldPosition();
        float left_map_world_x_position = center_tile_map_top_left_world_position.X - left_map_width_in_pixels;
        MATH::Vector2f left_tile_map_top_left_world_position(
            left_map_world_x_position,
            center_tile_map_top_left_world_position.Y);

        // CREATE THE LEFT TILE MAP.
        std::shared_ptr<MAPS::TileMap> left_tile_map = std::make_shared<MAPS::TileMap>(
            left_tile_map_overworld_grid_position,
            left_tile_map_top_left_world_position,
            left_map_data,
            GraphicsSystem);
        return left_tile_map;
    }

    std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildRightTileMap(
        const MAPS::TileMap& center_map,
        const std::shared_ptr<ITileMapData>& right_map_data)
    {
        // CHECK IF MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != right_map_data);
        if (!map_data_exists)
        {
            // No tile map can be constructed without map data.
            return nullptr;
        }

        // CALCULATE THE GRID POSITION OF THE RIGHT MAP.
        MATH::Vector2ui center_tile_map_overworld_grid_position = center_map.GetOverworldGridPosition();
        MATH::Vector2ui right_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        right_tile_map_overworld_grid_position.X++;

        // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
        float center_map_width_in_tiles = static_cast<float>(center_map.GetWidthInTiles());
        float center_map_tile_width_in_pixels = static_cast<float>(center_map.GetTileWidthInPixels());
        float center_map_width_in_pixels = center_map_width_in_tiles * center_map_tile_width_in_pixels;
        MATH::Vector2f center_tile_map_top_left_world_position = center_map.GetTopLeftWorldPosition();
        float right_map_world_position = center_tile_map_top_left_world_position.X + center_map_width_in_pixels;
        MATH::Vector2f rightTileMapTopLeftWorldPosition(
            right_map_world_position,
            center_tile_map_top_left_world_position.Y);

        // CREATE THE RIGHT TILE MAP.
        std::shared_ptr<MAPS::TileMap> right_tile_map = std::make_shared<MAPS::TileMap>(
            right_tile_map_overworld_grid_position,
            rightTileMapTopLeftWorldPosition,
            right_map_data,
            GraphicsSystem);
        return right_tile_map;
    }
}