#include <memory>
#include <TmxParser/Tmx.h>
#include "Maps/OverworldMap.h"
#include "Maps/TileMapBuilder.h"
#include "Maps/TileMapLoader.h"

using namespace MAPS;

OverworldMap::OverworldMap() :
    m_currentTileMap(),
    m_topTileMap(),
    m_bottomTileMap(),
    m_leftTileMap(),
    m_rightTileMap()
{
    // Nothing else to do.
}

OverworldMap::~OverworldMap()
{
    // Nothing else to do.
}

void OverworldMap::PopulateFromSpecification(
    const OverworldMapSpecification& overworldSpec,
    const OverworldGridPosition& startingTileMapOverworldGridPosition,
    const MATH::Vector2f& startingTileMapTopLeftWorldPosition,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // CREATE THE STARTING TILE MAP.
    MAPS::TileMapLoader mapLoader;
    std::shared_ptr<Tmx::Map> startingTmxMap = mapLoader.LoadMap(overworldSpec.GetStartingTileMapFilepath());
    std::shared_ptr<MAPS::TileMap> startingTileMap = std::make_shared<MAPS::TileMap>(
        startingTileMapOverworldGridPosition,
        startingTileMapTopLeftWorldPosition,
        startingTmxMap, 
        graphicsSystem);
    SetCurrentTileMap(startingTileMap);

    // CREATE THE TOP TILE MAP, IF ONE EXISTS.
    // Determine the position of the top map in the overworld grid.
    MATH::Vector2ui topTileMapOverworldGridPosition = startingTileMapOverworldGridPosition;
    topTileMapOverworldGridPosition.Y--;
    // These checks are needed to prevent an exception.
    bool topTileMapExists = overworldSpec.PositionInRange(
        topTileMapOverworldGridPosition.Y,
        topTileMapOverworldGridPosition.X);
    if (topTileMapExists)
    {
        // Get the filepath to the top tile map.
        std::string topTileMapFilepath = overworldSpec.GetTileMapFilepath(
            topTileMapOverworldGridPosition.Y,
            topTileMapOverworldGridPosition.X);

        // Create the top tile map in the overworld.
        std::shared_ptr<Tmx::Map> topTmxMap = mapLoader.LoadMap(topTileMapFilepath);
        std::shared_ptr<MAPS::TileMap> topTileMap = TileMapBuilder::BuildTopTileMap(
            *startingTileMap, 
            topTmxMap, 
            graphicsSystem);
        SetTopTileMap(topTileMap);
    }

    // CREATE THE BOTTOM TILE MAP, IF ONE EXISTS.
    // Determine the position of the bottom map in the overworld grid.
    MATH::Vector2ui bottomTileOverworldGridPosition = startingTileMapOverworldGridPosition;
    bottomTileOverworldGridPosition.Y++;
    // These checks are needed to prevent an exception.
    bool bottomTileMapExists = overworldSpec.PositionInRange(
        bottomTileOverworldGridPosition.Y,
        bottomTileOverworldGridPosition.X);
    if (bottomTileMapExists)
    {
        // Get the filepath to the bottom tile map.
        std::string bottomTileMapFilepath = overworldSpec.GetTileMapFilepath(
            bottomTileOverworldGridPosition.Y,
            bottomTileOverworldGridPosition.X);

        // Create the bottom tile map in the overworld.
        std::shared_ptr<Tmx::Map> bottomTmxMap = mapLoader.LoadMap(bottomTileMapFilepath);
        std::shared_ptr<MAPS::TileMap> bottomTileMap = TileMapBuilder::BuildBottomTileMap(
            *startingTileMap, 
            bottomTmxMap,
            graphicsSystem);
        SetBottomTileMap(bottomTileMap);
    }

    // CREATE THE LEFT TILE MAP, IF ONE EXISTS.
    // Determine the position of the left map in the overworld grid.
    MATH::Vector2ui leftTileMapOverworldGridPosition = startingTileMapOverworldGridPosition;
    leftTileMapOverworldGridPosition.X--;
    // These checks are needed to prevent an exception.
    bool leftTileMapExists = overworldSpec.PositionInRange(
        leftTileMapOverworldGridPosition.Y,
        leftTileMapOverworldGridPosition.X);
    if (leftTileMapExists)
    {
        // Get the filepath to the left tile map.
        std::string leftTileMapFilepath = overworldSpec.GetTileMapFilepath(
            leftTileMapOverworldGridPosition.Y,
            leftTileMapOverworldGridPosition.X);

        // Create the left tile map in the overworld.
        std::shared_ptr<Tmx::Map> leftTmxMap = mapLoader.LoadMap(leftTileMapFilepath);
        std::shared_ptr<MAPS::TileMap> leftTileMap = TileMapBuilder::BuildLeftTileMap(
            *startingTileMap, 
            leftTmxMap,
            graphicsSystem);
        SetLeftTileMap(leftTileMap);
    }

    // CREATE THE RIGHT TILE MAP, IF ONE EXISTS.
    // Determine the position of the right map in the overworld grid.
    MATH::Vector2ui rightTileMapOverworldGridPosition = startingTileMapOverworldGridPosition;
    rightTileMapOverworldGridPosition.X++;
    // These checks are needed to prevent an exception.
    bool rightTileMapExists = overworldSpec.PositionInRange(
        rightTileMapOverworldGridPosition.Y,
        rightTileMapOverworldGridPosition.X);
    if (rightTileMapExists)
    {
        // Get the filepath to the right tile map.
        std::string rightTileMapFilepath = overworldSpec.GetTileMapFilepath(
            rightTileMapOverworldGridPosition.Y,
            rightTileMapOverworldGridPosition.X);

        // Create the right tile map in the overworld.
        std::shared_ptr<Tmx::Map> rightTmxMap = mapLoader.LoadMap(rightTileMapFilepath);
        std::shared_ptr<MAPS::TileMap> rightTileMap = TileMapBuilder::BuildTopTileMap(
            *startingTileMap, 
            rightTmxMap,
            graphicsSystem);
        SetRightTileMap(rightTileMap);
    }
}

std::shared_ptr<TileMap> OverworldMap::GetCurrentTileMap() const
{
    return m_currentTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetTopTileMap() const
{
    return m_topTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetBottomTileMap() const
{
    return m_bottomTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetLeftTileMap() const
{
    return m_leftTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetRightTileMap() const
{
    return m_rightTileMap;
}

void OverworldMap::SetCurrentTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_currentTileMap = tileMap;
}

void OverworldMap::SetTopTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_topTileMap = tileMap;
}

void OverworldMap::SetBottomTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_bottomTileMap = tileMap;
}

void OverworldMap::SetLeftTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_leftTileMap = tileMap;
}

void OverworldMap::SetRightTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_rightTileMap = tileMap;
}