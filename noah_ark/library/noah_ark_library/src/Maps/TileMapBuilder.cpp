#include "Maps/TileMapBuilder.h"

using namespace MAPS;

std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildTopTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& topTmxMap,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // CHECK IF A TOP TMX MAP WAS PROVIDED.
    bool topTmxMapExists = (nullptr != topTmxMap);
    if (!topTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE TOP MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui topTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    topTileMapOverworldGridPosition.Y--;

    // CALCULATE THE WORLD POSITION OF THE TOP MAP.
    float topMapHeightInTiles = static_cast<float>(topTmxMap->GetHeight());
    float topMapTileHeightInPixels = static_cast<float>(topTmxMap->GetTileHeight());
    float topMapHeightInPixels = topMapHeightInTiles * topMapTileHeightInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float topMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y - topMapHeightInPixels;
    MATH::Vector2f topTileMapTopLeftWorldPosition(
        centerTileMapTopLeftWorldPosition.X,
        topMapWorldYPosition);

    // CREATE THE TOP TILE MAP.
    std::shared_ptr<MAPS::TileMap> topTileMap = std::make_shared<MAPS::TileMap>(
        topTileMapOverworldGridPosition,
        topTileMapTopLeftWorldPosition,
        topTmxMap,
        graphicsSystem);
    return topTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildBottomTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& bottomTmxMap,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // CHECK IF A BOTTOM TMX MAP WAS PROVIDED.
    bool bottomTmxMapExists = (nullptr != bottomTmxMap);
    if (!bottomTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE BOTTOM MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui bottomTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    bottomTileMapOverworldGridPosition.Y++;

    // CALCULATE THE WORLD POSITION OF THE BOTTOM MAP.
    float centerMapHeightInTiles = static_cast<float>(centerMap.GetHeightInTiles());
    float centerMapTileHeightInPixels = static_cast<float>(centerMap.GetTileHeightInPixels());
    float centerMapHeightInPixels = centerMapHeightInTiles * centerMapTileHeightInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float bottomMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y + centerMapHeightInPixels;
    MATH::Vector2f bottomTileMapTopLeftWorldPosition(
        centerTileMapTopLeftWorldPosition.X,
        bottomMapWorldYPosition);

    // CREATE THE BOTTOM TILE MAP.
    std::shared_ptr<MAPS::TileMap> bottomTileMap = std::make_shared<MAPS::TileMap>(
        bottomTileMapOverworldGridPosition,
        bottomTileMapTopLeftWorldPosition,
        bottomTmxMap,
        graphicsSystem);
    return bottomTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildLeftTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& leftTmxMap,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // CHECK IF A LEFT TMX MAP WAS PROVIDED.
    bool leftTmxMapExists = (nullptr != leftTmxMap);
    if (!leftTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE LEFT MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui leftTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    leftTileMapOverworldGridPosition.X--;

    // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
    float leftMapWidthInTiles = static_cast<float>(leftTmxMap->GetWidth());
    float leftMapTileWidthInPixels = static_cast<float>(leftTmxMap->GetTileWidth());
    float leftMapWidthInPixels = leftMapWidthInTiles * leftMapTileWidthInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float leftMapWorldXPosition = centerTileMapTopLeftWorldPosition.X - leftMapWidthInPixels;
    MATH::Vector2f leftTileMapTopLeftWorldPosition(
        leftMapWorldXPosition,
        centerTileMapTopLeftWorldPosition.Y);

    // CREATE THE LEFT TILE MAP.
    std::shared_ptr<MAPS::TileMap> leftTileMap = std::make_shared<MAPS::TileMap>(
        leftTileMapOverworldGridPosition,
        leftTileMapTopLeftWorldPosition,
        leftTmxMap,
        graphicsSystem);
    return leftTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapBuilder::BuildRightTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& rightTmxMap,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // CHECK IF A RIGHT TMX MAP WAS PROVIDED.
    bool rightTmxMapExists = (nullptr != rightTmxMap);
    if (!rightTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE RIGHT MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui rightTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    rightTileMapOverworldGridPosition.X++;

    // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
    float centerMapWidthInTiles = static_cast<float>(centerMap.GetWidthInTiles());
    float centerMapTileWidthInPixels = static_cast<float>(centerMap.GetTileWidthInPixels());
    float centerMapWidthInPixels = centerMapWidthInTiles * centerMapTileWidthInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float rightMapWorldXPosition = centerTileMapTopLeftWorldPosition.X + centerMapWidthInPixels;
    MATH::Vector2f rightTileMapTopLeftWorldPosition(
        rightMapWorldXPosition,
        centerTileMapTopLeftWorldPosition.Y);

    // CREATE THE RIGHT TILE MAP.
    std::shared_ptr<MAPS::TileMap> rightTileMap = std::make_shared<MAPS::TileMap>(
        rightTileMapOverworldGridPosition,
        rightTileMapTopLeftWorldPosition,
        rightTmxMap,
        graphicsSystem);
    return rightTileMap;
}