#pragma once

#include <functional>
#include "Maps/SurroundingTileMapLoader.h"

using namespace MAPS;

SurroundingTileMapLoader::SurroundingTileMapLoader(const OverworldMapSpecification* const pOverworldMapSpec) :
    m_pOverworldMapSpec(pOverworldMapSpec),
    m_mapLoader(),
    m_topTmxMap(),
    m_bottomTmxMap(),
    m_leftTmxMap(),
    m_rightTmxMap(),
    m_topTmxLoader(),
    m_bottomTmxLoader(),
    m_leftTmxLoader(),
    m_rightTmxLoader()
{}
        
SurroundingTileMapLoader::~SurroundingTileMapLoader()
{
    // Nullify the pointer to avoid accidental memory corruption later.
    // Freeing the memory is a responsibility external to this class.
    m_pOverworldMapSpec = nullptr;
}

void SurroundingTileMapLoader::StartLoadingTopTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition)
{
    // RESET THE TOP TILE MAP.
    m_topTmxMap.reset();

    // CHECK IF A TOP MAP EXISTS.
    // Determine the position of the top map in the overworld grid.
    MATH::Vector2ui topTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    topTileMapOverworldGridPosition.Y--;
    // These checks are needed to prevent an exception.
    bool topTileMapExists = m_pOverworldMapSpec->PositionInRange(
        topTileMapOverworldGridPosition.Y,
        topTileMapOverworldGridPosition.X);
    if (topTileMapExists)
    {
        // Get the filepath to the top tile map.
        std::string topTileMapFilepath = m_pOverworldMapSpec->GetTileMapFilepath(
            topTileMapOverworldGridPosition.Y,
            topTileMapOverworldGridPosition.X);

        // Launch a separate thread for loading the top map.  A separate thread isn't forced
        // to avoid exceptions on systems where additional threads may not be available.
        auto topTileMapLoadingFunction = std::bind(&MAPS::TileMapLoader::LoadMap, &m_mapLoader, topTileMapFilepath);
        m_topTmxLoader = std::async(topTileMapLoadingFunction);
    }
}

void SurroundingTileMapLoader::StartLoadingBottomTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition)
{
    // RESET THE BOTTOM TILE MAP.
    m_bottomTmxMap.reset();

    // CHECK IF A BOTTOM MAP EXISTS.
    // Determine the position of the bottom map in the overworld grid.
    MATH::Vector2ui bottomTileOverworldGridPosition = centerTileMapOverworldGridPosition;
    bottomTileOverworldGridPosition.Y++;
    // These checks are needed to prevent an exception.
    bool bottomTileMapExists = m_pOverworldMapSpec->PositionInRange(
        bottomTileOverworldGridPosition.Y,
        bottomTileOverworldGridPosition.X);
    if (bottomTileMapExists)
    {
        // Get the filepath to the bottom tile map.
        std::string bottomTileMapFilepath = m_pOverworldMapSpec->GetTileMapFilepath(
            bottomTileOverworldGridPosition.Y,
            bottomTileOverworldGridPosition.X);

        // Launch a separate thread for loading the bottom map.  A separate thread isn't forced
        // to avoid exceptions on systems where additional threads may not be available.
        auto bottomTileMapLoadingFunction = std::bind(&MAPS::TileMapLoader::LoadMap, &m_mapLoader, bottomTileMapFilepath);
        m_bottomTmxLoader = std::async(bottomTileMapLoadingFunction);
    }
}

void SurroundingTileMapLoader::StartLoadingLeftTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition)
{
    // RESET THE LEFT TILE MAP.
    m_leftTmxMap.reset();

    // CHECK IF A LEFT MAP EXISTS.
    // Determine the position of the left map in the overworld grid.
    MATH::Vector2ui leftTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    leftTileMapOverworldGridPosition.X--;
    // These checks are needed to prevent an exception.
    bool leftTileMapExists = m_pOverworldMapSpec->PositionInRange(
        leftTileMapOverworldGridPosition.Y,
        leftTileMapOverworldGridPosition.X);
    if (leftTileMapExists)
    {
        // Get the filepath to the left tile map.
        std::string leftTileMapFilepath = m_pOverworldMapSpec->GetTileMapFilepath(
            leftTileMapOverworldGridPosition.Y,
            leftTileMapOverworldGridPosition.X);

        // Launch a separate thread for loading the left map.  A separate thread isn't forced
        // to avoid exceptions on systems where additional threads may not be available.
        auto leftTileMapLoadingFunction = std::bind(&MAPS::TileMapLoader::LoadMap, &m_mapLoader, leftTileMapFilepath);
        m_leftTmxLoader = std::async(leftTileMapLoadingFunction);
    }
}

void SurroundingTileMapLoader::StartLoadingRightTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition)
{
    // RESET THE RIGHT TILE MAP.
    m_rightTmxMap.reset();

    // CHECK IF A RIGHT MAP EXISTS.
    // Determine the position of the right map in the overworld grid.
    MATH::Vector2ui rightTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    rightTileMapOverworldGridPosition.X++;
    // These checks are needed to prevent an exception.
    bool rightTileMapExists = m_pOverworldMapSpec->PositionInRange(
        rightTileMapOverworldGridPosition.Y,
        rightTileMapOverworldGridPosition.X);
    if (rightTileMapExists)
    {
        // Get the filepath to the right tile map.
        std::string rightTileMapFilepath = m_pOverworldMapSpec->GetTileMapFilepath(
            rightTileMapOverworldGridPosition.Y,
            rightTileMapOverworldGridPosition.X);

        // Launch a separate thread for loading the right map.  A separate thread isn't forced
        // to avoid exceptions on systems where additional threads may not be available.
        auto rightTileMapLoadingFunction = std::bind(&MAPS::TileMapLoader::LoadMap, &m_mapLoader, rightTileMapFilepath);
        m_rightTmxLoader = std::async(rightTileMapLoadingFunction);
    }
}

std::shared_ptr<Tmx::Map> SurroundingTileMapLoader::GetTopTileMap()
{
    // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
    try
    {
        // Verify that the future for the top TMX map is valid.
        if (m_topTmxLoader.valid())
        {
            // Return the loaded map.
            m_topTmxMap = m_topTmxLoader.get();
            return m_topTmxMap;
        }
        else
        {
            // Return the previously loaded map (if one exists).
            return m_topTmxMap;
        }
    }
    catch (const std::exception&)
    {
        // No map could be loaded.
        return nullptr;
    }
}

std::shared_ptr<Tmx::Map> SurroundingTileMapLoader::GetBottomTileMap()
{
    // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
    try
    {
        // Verify that the future for the bottom TMX map is valid.
        if (m_bottomTmxLoader.valid())
        {
            // Return the loaded map.
            m_bottomTmxMap = m_bottomTmxLoader.get();
            return m_bottomTmxMap;
        }
        else
        {
            // Return the previously loaded map (if one exists).
            return m_bottomTmxMap;
        }
    }
    catch (const std::exception&)
    {
        // No map could be loaded.
        return nullptr;
    }
}

std::shared_ptr<Tmx::Map> SurroundingTileMapLoader::GetLeftTileMap()
{
    // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
    try
    {
        // Verify that the future for the left TMX map is valid.
        if (m_leftTmxLoader.valid())
        {
            // Return the loaded map.
            m_leftTmxMap = m_leftTmxLoader.get();
            return m_leftTmxMap;
        }
        else
        {
            // Return the previously loaded map (if one exists).
            return m_leftTmxMap;
        }
    }
    catch (const std::exception&)
    {
        // No map could be loaded.
        return nullptr;
    }
}

std::shared_ptr<Tmx::Map> SurroundingTileMapLoader::GetRightTileMap()
{
    // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
    try
    {
        // Verify that the future for the right TMX map is valid.
        if (m_rightTmxLoader.valid())
        {
            // Return the loaded map.
            m_rightTmxMap = m_rightTmxLoader.get();
            return m_rightTmxMap;
        }
        else
        {
            // Return the previously loaded map (if one exists).
            return m_rightTmxMap;
        }
    }
    catch (const std::exception&)
    {
        // No map could be loaded.
        return nullptr;
    }
}
