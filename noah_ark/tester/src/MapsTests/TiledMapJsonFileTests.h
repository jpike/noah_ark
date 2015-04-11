#pragma once

#include <string>
#include "Maps/TiledMapJsonFile.h"

namespace TILED_MAP_JSON_FILE_TESTS
{
    TEST_CASE("Tile and map dimensions are required in the file.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_layer_tile_single.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // VALIDATE THE DIMENSIONS OF THE MAP.
        const unsigned int EXPECTED_WIDTH_IN_TILES = 3;
        unsigned int actualWidthInTiles = mapFile.GetWidthInTiles();
        REQUIRE(EXPECTED_WIDTH_IN_TILES == actualWidthInTiles);

        const unsigned int EXPECTED_HEIGHT_IN_TILES = 2;
        unsigned int actualHeightInTiles = mapFile.GetHeightInTiles();
        REQUIRE(EXPECTED_HEIGHT_IN_TILES == actualHeightInTiles);

        const unsigned int EXPECTED_TILE_WIDTH_IN_PIXELS = 16;
        unsigned int actualTileWidthInPixels = mapFile.GetTileWidthInPixels();
        REQUIRE(EXPECTED_TILE_WIDTH_IN_PIXELS == actualTileWidthInPixels);

        const unsigned int EXPECTED_TILE_HEIGHT_IN_PIXELS = 8;
        unsigned int actualTileHeightInPixels = mapFile.GetTileHeightInPixels();
        REQUIRE(EXPECTED_TILE_HEIGHT_IN_PIXELS == actualTileHeightInPixels);
    }

    TEST_CASE("A tile map may have a single tileset.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_layer_tile_single.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // VALIDATE THE TILESET.
        const unsigned int SINGLE_TILESET = 1;
        std::vector<MAPS::TilesetDescription> tilesetDescriptions = mapFile.GetTilesets();
        REQUIRE(SINGLE_TILESET == tilesetDescriptions.size());

        MAPS::TilesetDescription expectedTileset;
        expectedTileset.Name = "first_tileset";
        expectedTileset.FirstTileId = 1;
        expectedTileset.TileWidthInPixels = 16;
        expectedTileset.TileHeightInPixels = 8;
        expectedTileset.TransparentColor = "#ff00ff";

        MAPS::TilesetDescription actualTileset = tilesetDescriptions.front();
        REQUIRE(expectedTileset == actualTileset);
    }

    TEST_CASE("A tile map may have multiple tilesets.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_tileset_multiple.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // VALIDATE THAT TWO TILESETS WERE LOADED.
        const unsigned int EXPECTED_TILESET_COUNT = 2;
        std::vector<MAPS::TilesetDescription> tilesetDescriptions = mapFile.GetTilesets();
        REQUIRE(EXPECTED_TILESET_COUNT == tilesetDescriptions.size());

        // CREATE THE EXPECTED TILESETS.
        MAPS::TilesetDescription expectedFirstTileset;
        expectedFirstTileset.Name = "first_tileset";
        expectedFirstTileset.FirstTileId = 1;
        expectedFirstTileset.TileWidthInPixels = 16;
        expectedFirstTileset.TileHeightInPixels = 8;
        expectedFirstTileset.TransparentColor = "#ff00ff";

        MAPS::TilesetDescription expectedSecondTileset;
        expectedSecondTileset.Name = "second_tileset";
        expectedSecondTileset.FirstTileId = 2;
        expectedSecondTileset.TileWidthInPixels = 17;
        expectedSecondTileset.TileHeightInPixels = 9;
        expectedSecondTileset.TransparentColor = "#00ff00";

        // VALIDATE THE TILESETS.
        const unsigned int FIRST_TILESET_INDEX = 0;
        MAPS::TilesetDescription actualFirstTileset = tilesetDescriptions.at(FIRST_TILESET_INDEX);
        REQUIRE(expectedFirstTileset == actualFirstTileset);

        const unsigned int SECOND_TILESET_INDEX = 1;
        MAPS::TilesetDescription actualSecondTileset = tilesetDescriptions.at(SECOND_TILESET_INDEX);
        REQUIRE(expectedSecondTileset == actualSecondTileset);
    }

    TEST_CASE("A tile map may have a tile layer.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_layer_tile_single.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // VALIDATE THE TILE LAYER.
        const unsigned int SINGLE_LAYER = 1;
        std::vector<MAPS::TileMapLayerDescription> layerDescriptions = mapFile.GetLayers();
        REQUIRE(SINGLE_LAYER == layerDescriptions.size());

        MAPS::TileMapLayerDescription expectedLayer;
        expectedLayer.Name = "FirstLayer";
        expectedLayer.Type = MAPS::TileMapLayerType::TILE_LAYER;
        expectedLayer.TileIds = {
            1, 2, 3,
            4, 5, 6 };

        MAPS::TileMapLayerDescription actualLayer = layerDescriptions.front();
        REQUIRE(expectedLayer == actualLayer);
    }

    TEST_CASE("A tile map may have an object layer.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_layer_object_single.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // VALIDATE THAT A SINGLE LAYER WAS LOADED.
        const unsigned int SINGLE_LAYER = 1;
        std::vector<MAPS::TileMapLayerDescription> layerDescriptions = mapFile.GetLayers();
        REQUIRE(SINGLE_LAYER == layerDescriptions.size());

        // CREATE THE EXPECTED OBJECT LAYER.
        MAPS::TileMapLayerDescription expectedLayer;
        expectedLayer.Name = "FirstLayer";
        expectedLayer.Type = MAPS::TileMapLayerType::OBJECT_LAYER;

        // Add the first object.
        MAPS::TileMapObjectDescription firstObject;
        firstObject.Name = "Object1";
        firstObject.Type = "ObjectType1";
        firstObject.TopLeftPositionInPixels.X = 432;
        firstObject.TopLeftPositionInPixels.Y = 81;
        firstObject.WidthInPixels = 16;
        firstObject.HeightInPixels = 31;
        expectedLayer.Objects.push_back(firstObject);

        // Add the second object.
        MAPS::TileMapObjectDescription secondObject;
        secondObject.Name = "Object2";
        secondObject.Type = "ObjectType2";
        secondObject.TopLeftPositionInPixels.X = 347;
        secondObject.TopLeftPositionInPixels.Y = 269;
        secondObject.WidthInPixels = 38;
        secondObject.HeightInPixels = 37;
        expectedLayer.Objects.push_back(secondObject);

        // VALIDATE THE LAYER.
        MAPS::TileMapLayerDescription actualLayer = layerDescriptions.front();
        REQUIRE(expectedLayer == actualLayer);
    }

    TEST_CASE("A tile map may have multiple layers.", "[TileMapJsonFile]")
    {
        // LOAD THE TEST MAP.
        const std::string MAP_FILEPATH = "res/maps/testing/test_layer_multiple.json";
        MAPS::TiledMapJsonFile mapFile;
        bool fileLoaded = mapFile.Load(MAP_FILEPATH);
        REQUIRE(fileLoaded);

        // CREATE THE FIRST EXPECTED LAYER.
        MAPS::TileMapLayerDescription expectedFirstLayer;
        expectedFirstLayer.Name = "FirstLayer";
        expectedFirstLayer.Type = MAPS::TileMapLayerType::TILE_LAYER;
        expectedFirstLayer.TileIds = {
            1, 2, 3,
            4, 5, 6 };

        // CREATE THE SECOND EXPECTED LAYER.
        MAPS::TileMapLayerDescription expectedSecondLayer;
        expectedSecondLayer.Name = "SecondLayer";
        expectedSecondLayer.Type = MAPS::TileMapLayerType::OBJECT_LAYER;

        // Add the first object.
        MAPS::TileMapObjectDescription firstObject;
        firstObject.Name = "Object1";
        firstObject.Type = "ObjectType1";
        firstObject.TopLeftPositionInPixels.X = 432;
        firstObject.TopLeftPositionInPixels.Y = 81;
        firstObject.WidthInPixels = 16;
        firstObject.HeightInPixels = 31;
        expectedSecondLayer.Objects.push_back(firstObject);

        // Add the second object.
        MAPS::TileMapObjectDescription secondObject;
        secondObject.Name = "Object2";
        secondObject.Type = "ObjectType2";
        secondObject.TopLeftPositionInPixels.X = 347;
        secondObject.TopLeftPositionInPixels.Y = 269;
        secondObject.WidthInPixels = 38;
        secondObject.HeightInPixels = 37;
        expectedSecondLayer.Objects.push_back(secondObject);

        // VALIDATE THE LAYERS.
        std::vector<MAPS::TileMapLayerDescription> expectedLayerDescriptions = { expectedFirstLayer, expectedSecondLayer };
        std::vector<MAPS::TileMapLayerDescription> actualLayerDescriptions = mapFile.GetLayers();
        REQUIRE(expectedLayerDescriptions == actualLayerDescriptions);
    }
}