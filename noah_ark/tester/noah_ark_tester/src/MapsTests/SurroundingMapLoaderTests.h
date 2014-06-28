#pragma once

// This guard is needed to avoid issues where catch is already included.
// Otherwise, we can't split up tests into multiple files.
#ifndef CATCH_CONFIG_MAIN
#include <catch.hpp>
#endif

#include "Maps/SurroundingTileMapLoader.h"

TEST_CASE( "Load the top surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingTopTileMap][GetTopTileMap]" )
{
    // SETUP.
    // Load the default testing overworld map specification.
    MAPS::OverworldMapSpecification overworldSpecification;
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.xml";
    bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    /// @todo   Move loading and verification of this overworld map specification to a helper function.
    REQUIRE(specificationLoadedSuccessfully);

    // Create the surrounding tile map loader.
    MAPS::SurroundingTileMapLoader surroundingMapLoader(&overworldSpecification);

    // EXECUTE.
    // Test loading and retrieving the top map.
    const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
    surroundingMapLoader.StartLoadingTopTileMap(CENTER_MAP_GRID_POSITION);
    std::shared_ptr<Tmx::Map> topTmxMap = surroundingMapLoader.GetTopTileMap();

    // VERIFY.
    /// @todo   Create helper function(s) to verify data from the test overworld map specification.
    ///         More verification should be added here.
    bool topMapLoadedSuccessfully = (nullptr != topTmxMap);
    REQUIRE(topMapLoadedSuccessfully);

    // Verify the filepath.  Note that the filename should return more than just as name.
    const std::string EXPECTED_TOP_TILE_MAP_FILEPATH = "res/maps/test_map_1_0.tmx";
    std::string actual_top_tile_map_filepath = topTmxMap->GetFilename();
    REQUIRE(EXPECTED_TOP_TILE_MAP_FILEPATH == actual_top_tile_map_filepath);
}

TEST_CASE( "Load the bottom surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingBottomTileMap][GetBottomTileMap]" )
{
    // SETUP.
    // Load the default testing overworld map specification.
    MAPS::OverworldMapSpecification overworldSpecification;
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.xml";
    bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    /// @todo   Move loading and verification of this overworld map specification to a helper function.
    REQUIRE(specificationLoadedSuccessfully);

    // Create the surrounding tile map loader.
    MAPS::SurroundingTileMapLoader surroundingMapLoader(&overworldSpecification);

    // EXECUTE.
    // Test loading and retrieving the bottom map.
    const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
    surroundingMapLoader.StartLoadingBottomTileMap(CENTER_MAP_GRID_POSITION);
    std::shared_ptr<Tmx::Map> bottomTmxMap = surroundingMapLoader.GetBottomTileMap();

    // VERIFY.
    /// @todo   Create helper function(s) to verify data from the test overworld map specification.
    ///         More verification should be added here.
    bool bottomMapLoadedSuccessfully = (nullptr != bottomTmxMap);
    REQUIRE(bottomMapLoadedSuccessfully);

    // Verify the filepath.  Note that the filename should return more than just as name.
    const std::string EXPECTED_BOTTOM_TILE_MAP_FILEPATH = "res/maps/test_map_1_2.tmx";
    std::string actual_bottom_tile_map_filepath = bottomTmxMap->GetFilename();
    REQUIRE(EXPECTED_BOTTOM_TILE_MAP_FILEPATH == actual_bottom_tile_map_filepath);
}

TEST_CASE( "Load the left surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingLeftTileMap][GetLeftTileMap]" )
{
    // SETUP.
    // Load the default testing overworld map specification.
    MAPS::OverworldMapSpecification overworldSpecification;
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.xml";
    bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    /// @todo   Move loading and verification of this overworld map specification to a helper function.
    REQUIRE(specificationLoadedSuccessfully);

    // Create the surrounding tile map loader.
    MAPS::SurroundingTileMapLoader surroundingMapLoader(&overworldSpecification);

    // EXECUTE.
    // Test loading and retrieving the left map.
    const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
    surroundingMapLoader.StartLoadingLeftTileMap(CENTER_MAP_GRID_POSITION);
    std::shared_ptr<Tmx::Map> leftTmxMap = surroundingMapLoader.GetLeftTileMap();

    // VERIFY.
    /// @todo   Create helper function(s) to verify data from the test overworld map specification.
    ///         More verification should be added here.
    bool leftMapLoadedSuccessfully = (nullptr != leftTmxMap);
    REQUIRE(leftMapLoadedSuccessfully);

    // Verify the filepath.  Note that the filename should return more than just as name.
    const std::string EXPECTED_LEFT_TILE_MAP_FILEPATH = "res/maps/test_map_0_1.tmx";
    std::string actual_left_tile_map_filepath = leftTmxMap->GetFilename();
    REQUIRE(EXPECTED_LEFT_TILE_MAP_FILEPATH == actual_left_tile_map_filepath);
}

TEST_CASE( "Load the right surrounding map.  Success.", "[SurroundingTileMapLoader][StartLoadingRightTileMap][GetRightTileMap]" )
{
    // SETUP.
    // Load the default testing overworld map specification.
    MAPS::OverworldMapSpecification overworldSpecification;
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.xml";
    bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    /// @todo   Move loading and verification of this overworld map specification to a helper function.
    REQUIRE(specificationLoadedSuccessfully);

    // Create the surrounding tile map loader.
    MAPS::SurroundingTileMapLoader surroundingMapLoader(&overworldSpecification);

    // EXECUTE.
    // Test loading and retrieving the right map.
    const MATH::Vector2ui CENTER_MAP_GRID_POSITION(1, 1);
    surroundingMapLoader.StartLoadingRightTileMap(CENTER_MAP_GRID_POSITION);
    std::shared_ptr<Tmx::Map> rightTmxMap = surroundingMapLoader.GetRightTileMap();

    // VERIFY.
    /// @todo   Create helper function(s) to verify data from the test overworld map specification.
    ///         More verification should be added here.
    bool rightMapLoadedSuccessfully = (nullptr != rightTmxMap);
    REQUIRE(rightMapLoadedSuccessfully);

    // Verify the filepath.  Note that the filename should return more than just as name.
    const std::string EXPECTED_RIGHT_TILE_MAP_FILEPATH = "res/maps/test_map_2_1.tmx";
    std::string actual_right_tile_map_filepath = rightTmxMap->GetFilename();
    REQUIRE(EXPECTED_RIGHT_TILE_MAP_FILEPATH == actual_right_tile_map_filepath);
}