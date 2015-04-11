#pragma once

#include "Maps/SurroundingTileMapLoader.h"

namespace SURROUNDING_MAP_LOADER_TESTS
{

    TEST_CASE( "Load the top surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingTopTileMap][GetTopTileMap]" )
    {
        // SETUP.
        // Load the default testing overworld map specification.
        MAPS::OverworldMapSpecification overworld_specification;
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map.xml";
        bool specification_loaded_successfully = overworld_specification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
        /// @todo   Move loading and verification of this overworld map specification to a helper function.
        REQUIRE(specification_loaded_successfully);

        // Create the surrounding tile map loader.
        MAPS::SurroundingTileMapLoader surrounding_map_loader(&overworld_specification);

        // EXECUTE.
        // Test loading and retrieving the top map.
        const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
        surrounding_map_loader.StartLoadingTopTileMap(CENTER_MAP_GRID_POSITION);
        std::shared_ptr<MAPS::TiledMapJsonFile> top_map = surrounding_map_loader.GetTopTileMap();

        // VERIFY.
        /// @todo   Create helper function(s) to verify data from the test overworld map specification.
        ///         More verification should be added here.
        bool top_map_loaded_successfully = (nullptr != top_map);
        REQUIRE(top_map_loaded_successfully);
    }

    TEST_CASE( "Load the bottom surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingBottomTileMap][GetBottomTileMap]" )
    {
        // SETUP.
        // Load the default testing overworld map specification.
        MAPS::OverworldMapSpecification overworld_specification;
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map.xml";
        bool specification_loaded_successfully = overworld_specification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
        /// @todo   Move loading and verification of this overworld map specification to a helper function.
        REQUIRE(specification_loaded_successfully);

        // Create the surrounding tile map loader.
        MAPS::SurroundingTileMapLoader surrounding_map_loader(&overworld_specification);

        // EXECUTE.
        // Test loading and retrieving the bottom map.
        const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
        surrounding_map_loader.StartLoadingBottomTileMap(CENTER_MAP_GRID_POSITION);
        std::shared_ptr<MAPS::TiledMapJsonFile> bottom_map = surrounding_map_loader.GetBottomTileMap();

        // VERIFY.
        /// @todo   Create helper function(s) to verify data from the test overworld map specification.
        ///         More verification should be added here.
        bool bottom_map_loaded_successfully = (nullptr != bottom_map);
        REQUIRE(bottom_map_loaded_successfully);
    }

    TEST_CASE( "Load the left surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingLeftTileMap][GetLeftTileMap]" )
    {
        // SETUP.
        // Load the default testing overworld map specification.
        MAPS::OverworldMapSpecification overworld_specification;
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map.xml";
        bool specification_loaded_successfully = overworld_specification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
        /// @todo   Move loading and verification of this overworld map specification to a helper function.
        REQUIRE(specification_loaded_successfully);

        // Create the surrounding tile map loader.
        MAPS::SurroundingTileMapLoader surrounding_map_loader(&overworld_specification);

        // EXECUTE.
        // Test loading and retrieving the left map.
        const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
        surrounding_map_loader.StartLoadingLeftTileMap(CENTER_MAP_GRID_POSITION);
        std::shared_ptr<MAPS::TiledMapJsonFile> left_map = surrounding_map_loader.GetLeftTileMap();

        // VERIFY.
        /// @todo   Create helper function(s) to verify data from the test overworld map specification.
        ///         More verification should be added here.
        bool left_map_loaded_successfully = (nullptr != left_map);
        REQUIRE(left_map_loaded_successfully);
    }

    TEST_CASE( "Load the right surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingRightTileMap][GetRightTileMap]" )
    {
        // SETUP.
        // Load the default testing overworld map specification.
        MAPS::OverworldMapSpecification overworld_specification;
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map.xml";
        bool specification_loaded_successfully = overworld_specification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
        /// @todo   Move loading and verification of this overworld map specification to a helper function.
        REQUIRE(specification_loaded_successfully);

        // Create the surrounding tile map loader.
        MAPS::SurroundingTileMapLoader surrounding_map_loader(&overworld_specification);

        // EXECUTE.
        // Test loading and retrieving the right map.
        const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
        surrounding_map_loader.StartLoadingRightTileMap(CENTER_MAP_GRID_POSITION);
        std::shared_ptr<MAPS::TiledMapJsonFile> right_map = surrounding_map_loader.GetRightTileMap();

        // VERIFY.
        /// @todo   Create helper function(s) to verify data from the test overworld map specification.
        ///         More verification should be added here.
        bool right_map_loaded_successfully = (nullptr != right_map);
        REQUIRE(right_map_loaded_successfully);
    }

}