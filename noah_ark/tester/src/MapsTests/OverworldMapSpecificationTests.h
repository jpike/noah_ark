#pragma once

#include <vector>
#include "Maps/OverworldMapSpecification.h"

namespace OVERWORLD_MAP_SPECIFICATION_TESTS
{

    TEST_CASE( "Load specification from file.  Success.", "[OverworldMapSpecification][LoadFromFile]" )
    {
        // SETUP.
        MAPS::OverworldMapSpecification overworldSpecification;

        // EXECUTE.
        // Load from a known specification file.
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map.xml";
        bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);

        // VERIFY.
        // Verify that the return value indicates success.
        REQUIRE(true == specificationLoadedSuccessfully);

        // Verify the size of the overworld.
        const unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 3;
        unsigned int actual_overworld_width_in_tile_maps = overworldSpecification.GetOverworldWidthInTileMaps();
        REQUIRE(OVERWORLD_WIDTH_IN_TILE_MAPS == actual_overworld_width_in_tile_maps);

        const unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 3;
        unsigned int actual_overworld_height_in_tile_maps = overworldSpecification.GetOverworldHeightInTileMaps();
        REQUIRE(OVERWORLD_HEIGHT_IN_TILE_MAPS == actual_overworld_height_in_tile_maps);

        // Verify that the proper map data was loaded.
        /// @todo   Update this to use C++11 initialization lists once Visual Studio supports them.
        std::vector< std::vector<std::string> > expected_tile_map_filepaths;
        // Row 0.
        std::vector<std::string> row_0_tile_maps;
        row_0_tile_maps.push_back("res/maps/testing/test_map_0_0.tmx");
        row_0_tile_maps.push_back("res/maps/testing/test_map_1_0.tmx");
        row_0_tile_maps.push_back("res/maps/testing/test_map_2_0.tmx");
        expected_tile_map_filepaths.push_back(row_0_tile_maps);
        // Row 1.
        std::vector<std::string> row_1_tile_maps;
        row_1_tile_maps.push_back("res/maps/testing/test_map_0_1.tmx");
        row_1_tile_maps.push_back("res/maps/testing/test_map_1_1.tmx");
        row_1_tile_maps.push_back("res/maps/testing/test_map_2_1.tmx");
        expected_tile_map_filepaths.push_back(row_1_tile_maps);
        // Row 2.
        std::vector<std::string> row_2_tile_maps;
        row_2_tile_maps.push_back("res/maps/testing/test_map_0_2.tmx");
        row_2_tile_maps.push_back("res/maps/testing/test_map_1_2.tmx");
        row_2_tile_maps.push_back("res/maps/testing/test_map_2_2.tmx");
        expected_tile_map_filepaths.push_back(row_2_tile_maps);

        for (unsigned int tileMapRow = 0; tileMapRow < expected_tile_map_filepaths.size(); ++tileMapRow)
        {
            for (unsigned int tileMapColumn = 0; tileMapColumn < expected_tile_map_filepaths[tileMapRow].size(); ++tileMapColumn)
            {
                std::string expected_tile_map_filepath = expected_tile_map_filepaths[tileMapRow][tileMapColumn];
                std::string actual_tile_map_filepath = overworldSpecification.GetTileMapFilepath(tileMapRow, tileMapColumn);

                REQUIRE(expected_tile_map_filepath == actual_tile_map_filepath);
            }
        }

        // Verify that the starting map is at the expected position.
        const MATH::Vector2ui EXPECTED_STARTING_TILE_MAP_POSITION(1, 1);
        MATH::Vector2ui actual_starting_tile_map_position = overworldSpecification.GetStartingTileMapPosition();
        REQUIRE(EXPECTED_STARTING_TILE_MAP_POSITION == actual_starting_tile_map_position);

        // Verify that the expected starting map was specified.
        const std::string& EXPECTED_STARTING_MAP_FILEPATH = expected_tile_map_filepaths[1][1];
        std::string actual_starting_tile_map_filepath = overworldSpecification.GetStartingTileMapFilepath();
        REQUIRE(EXPECTED_STARTING_MAP_FILEPATH == actual_starting_tile_map_filepath);
    }

    TEST_CASE( "Load map with single row.  Success.", "[OverworldMapSpecification][LoadFromFile]" )
    {
        // SETUP.
        MAPS::OverworldMapSpecification overworldSpecification;

        // EXECUTE.
        // Load from a known specification file.
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map_one_row.xml";
        bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);

        // VERIFY.
        // Verify that the return value indicates success.
        REQUIRE(true == specificationLoadedSuccessfully);

        // Verify the size of the overworld.
        const unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 3;
        unsigned int actual_overworld_width_in_tile_maps = overworldSpecification.GetOverworldWidthInTileMaps();
        REQUIRE(OVERWORLD_WIDTH_IN_TILE_MAPS == actual_overworld_width_in_tile_maps);

        const unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 1;
        unsigned int actual_overworld_height_in_tile_maps = overworldSpecification.GetOverworldHeightInTileMaps();
        REQUIRE(OVERWORLD_HEIGHT_IN_TILE_MAPS == actual_overworld_height_in_tile_maps);

        // Verification of the actual data in the tile map is left to another test.
        // It can be added later here if necessary.
    }

    TEST_CASE( "Load map with single column.  Success.", "[OverworldMapSpecification][LoadFromFile]" )
    {
        // SETUP.
        MAPS::OverworldMapSpecification overworldSpecification;

        // EXECUTE.
        // Load from a known specification file.
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map_one_column.xml";
        bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);

        // VERIFY.
        // Verify that the return value indicates success.
        REQUIRE(true == specificationLoadedSuccessfully);

        // Verify the size of the overworld.
        const unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 1;
        unsigned int actual_overworld_width_in_tile_maps = overworldSpecification.GetOverworldWidthInTileMaps();
        REQUIRE(OVERWORLD_WIDTH_IN_TILE_MAPS == actual_overworld_width_in_tile_maps);

        const unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 3;
        unsigned int actual_overworld_height_in_tile_maps = overworldSpecification.GetOverworldHeightInTileMaps();
        REQUIRE(OVERWORLD_HEIGHT_IN_TILE_MAPS == actual_overworld_height_in_tile_maps);

        // Verification of the actual data in the tile map is left to another test.
        // It can be added later here if necessary.
    }

    TEST_CASE( "Load map with single row and column.  Success.", "[OverworldMapSpecification][LoadFromFile]" )
    {
        // SETUP.
        MAPS::OverworldMapSpecification overworldSpecification;

        // EXECUTE.
        // Load from a known specification file.
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/testing/test_overworld_map_one_map.xml";
        bool specificationLoadedSuccessfully = overworldSpecification.LoadFromFile(TEST_OVERWORLD_SPECIFICATION_FILEPATH);

        // VERIFY.
        // Verify that the return value indicates success.
        REQUIRE(true == specificationLoadedSuccessfully);

        // Verify the size of the overworld.
        const unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 1;
        unsigned int actual_overworld_width_in_tile_maps = overworldSpecification.GetOverworldWidthInTileMaps();
        REQUIRE(OVERWORLD_WIDTH_IN_TILE_MAPS == actual_overworld_width_in_tile_maps);

        const unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 1;
        unsigned int actual_overworld_height_in_tile_maps = overworldSpecification.GetOverworldHeightInTileMaps();
        REQUIRE(OVERWORLD_HEIGHT_IN_TILE_MAPS == actual_overworld_height_in_tile_maps);

        // Verification of the actual data in the tile map is left to another test.
        // It can be added later here if necessary.
    }

}