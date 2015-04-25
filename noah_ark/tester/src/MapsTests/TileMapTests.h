#pragma once

#include "Maps/TiledMapJsonFile.h"
#include "Maps/TileMap.h"
#include "ResourcesTests/ResourceManagerTestConstants.h"

namespace TILE_MAP_TESTS
{
    TEST_CASE("Tile map fails construction if given null tile map data.", "[TileMap][Constructor]")
    {
        // DEFINE ARGUMENTS FOR THE CONSTRUCTOR.
        MAPS::OverworldGridPosition overworld_grid_position;
        MATH::Vector2f top_left_world_position_in_pixels;
        std::shared_ptr<MAPS::ITileMapData> null_map_data;

        std::shared_ptr<sf::RenderTarget> render_target = std::make_shared<sf::RenderTexture>();
        std::shared_ptr<RESOURCES::ResourceManager> resource_manager = std::make_shared<RESOURCES::ResourceManager>(
            TEST_RESOURCE_MANAGER::TEST_RESOURCES_FILEPATH);
        std::shared_ptr<GRAPHICS::GraphicsSystem> graphics_system = std::make_shared<GRAPHICS::GraphicsSystem>(
            render_target,
            resource_manager);

        // ATTEMPT CONSTRUCTION.
        bool exception_thrown = false;
        try
        {
            MAPS::TileMap tile_map(
                overworld_grid_position,
                top_left_world_position_in_pixels,
                null_map_data,
                graphics_system);
        }
        catch (const std::exception&)
        {
            exception_thrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exception_thrown);
    }

    TEST_CASE("Tile map fails construction if given null graphics system.", "[TileMap][Constructor]")
    {
        // DEFINE ARGUMENTS FOR THE CONSTRUCTOR.
        MAPS::OverworldGridPosition overworld_grid_position;
        MATH::Vector2f top_left_world_position_in_pixels;
        std::shared_ptr<MAPS::ITileMapData> map_data = std::make_shared<MAPS::TiledMapJsonFile>();
        std::shared_ptr<GRAPHICS::GraphicsSystem> null_graphics_system;

        // ATTEMPT CONSTRUCTION.
        bool exception_thrown = false;
        try
        {
            MAPS::TileMap tile_map(
                overworld_grid_position,
                top_left_world_position_in_pixels,
                map_data,
                null_graphics_system);
        }
        catch (const std::exception&)
        {
            exception_thrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exception_thrown);
    }
}