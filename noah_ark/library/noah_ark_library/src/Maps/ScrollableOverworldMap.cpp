#include "Maps/ScrollableOverworldMap.h"
#include "Maps/TileMapBuilder.h"

using namespace MAPS;

// STATIC CONSTANT INITIALIZATION.
const float ScrollableOverworldMap::MAX_SCROLL_TIME_IN_SECONDS = 3.0f;

ScrollableOverworldMap::ScrollableOverworldMap(
    const std::shared_ptr<OverworldMap>& overworldMap,
    const OverworldMapSpecification* const pOverworldMapSpec) :
    m_overworldMap(overworldMap),
    m_scrollProcess(),
    m_surroundingMapLoader()
{
    /// @todo   Throw an exception if the provided overworld map is NULL.

    // INITIALIZE THE SURROUNDING MAP LOADER.
    // This will allow surrounding tile maps to be loaded later during scrolling.
    m_surroundingMapLoader = std::unique_ptr<MAPS::SurroundingTileMapLoader>(
        new MAPS::SurroundingTileMapLoader(pOverworldMapSpec));
}

ScrollableOverworldMap::~ScrollableOverworldMap()
{
    // Nothing else to do.
}

bool ScrollableOverworldMap::BeginScrollingUp()
{
    // GET THE TILE MAPS REQUIRED TO START SCROLLING UP.
    std::shared_ptr<TileMap> currentTileMap = m_overworldMap->GetCurrentTileMap();
    std::shared_ptr<TileMap> topTileMap = m_overworldMap->GetTopTileMap();
    bool tileMapsForScrollingUpExist = (nullptr != currentTileMap && nullptr != topTileMap);
    if (!tileMapsForScrollingUpExist)
    {
        // We can't start scrolling up without both current and top tile maps.
        return false;
    }

    // GET THE STARTING AND ENDING POINTS FOR SCROLLING.
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    MATH::Vector2f topMapTopLeftPosition = topTileMap->GetTopLeftWorldPosition();

    // START THE SCROLLING PROCESS.
    m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
        new PROCESSES::ScrollProcess(
            PROCESSES::ScrollProcess::ScrollDirection::UP,
            currentMapTopLeftPosition,
            topMapTopLeftPosition,
            MAX_SCROLL_TIME_IN_SECONDS));

    // SWITCH THE CURRENT MAP WITH THE TOP MAP.
    m_overworldMap->SetBottomTileMap(currentTileMap);
    m_overworldMap->SetCurrentTileMap(topTileMap);

    // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
    // The new bottom tile map has already been loaded.
    MATH::Vector2ui newCurrentMapOverworldGridPosition = topTileMap->GetOverworldGridPosition();
    m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);

    // SCROLLING UP BEGAN SUCCESSFULLY.
    return true;
}

bool ScrollableOverworldMap::BeginScrollingDown()
{
    // GET THE TILE MAPS REQUIRED TO START SCROLLING DOWN.
    std::shared_ptr<TileMap> currentTileMap = m_overworldMap->GetCurrentTileMap();
    std::shared_ptr<TileMap> bottomTileMap = m_overworldMap->GetBottomTileMap();
    bool tileMapsForScrollingDownExist = (nullptr != currentTileMap && nullptr != bottomTileMap);
    if (!tileMapsForScrollingDownExist)
    {
        // We can't start scrolling down without both current and bottom tile maps.
        return false;
    }

    // GET THE STARTING AND ENDING POINTS FOR SCROLLING.
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    MATH::Vector2f bottomMapTopLeftPosition = bottomTileMap->GetTopLeftWorldPosition();

    // START THE SCROLLING PROCESS.
    m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
        new PROCESSES::ScrollProcess(
            PROCESSES::ScrollProcess::ScrollDirection::DOWN,
            currentMapTopLeftPosition,
            bottomMapTopLeftPosition,
            MAX_SCROLL_TIME_IN_SECONDS));

    // SWITCH THE CURRENT MAP WITH THE BOTTOM MAP.
    m_overworldMap->SetTopTileMap(currentTileMap);
    m_overworldMap->SetCurrentTileMap(bottomTileMap);

    // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
    // The new top tile map has already been loaded.
    MATH::Vector2ui newCurrentMapOverworldGridPosition = bottomTileMap->GetOverworldGridPosition();
    m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

    // SCROLLING DOWN BEGAN SUCCESSFULLY.
    return true;
}

bool ScrollableOverworldMap::BeginScrollingLeft()
{
    // GET THE TILE MAPS REQUIRED TO START SCROLLING LEFT.
    std::shared_ptr<TileMap> currentTileMap = m_overworldMap->GetCurrentTileMap();
    std::shared_ptr<MAPS::TileMap> leftTileMap = m_overworldMap->GetLeftTileMap();
    bool tileMapsForScrollingLeftExist = (nullptr != currentTileMap && nullptr != leftTileMap);
    if (!tileMapsForScrollingLeftExist)
    {
        // We can't start scrolling left without both current and left tile maps.
        return false;
    }

    // GET THE STARTING AND ENDING POINTS FOR SCROLLING.
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    MATH::Vector2f leftMapTopLeftPosition = leftTileMap->GetTopLeftWorldPosition();

    // START THE SCROLLING PROCESS.
    m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
        new PROCESSES::ScrollProcess(
            PROCESSES::ScrollProcess::ScrollDirection::LEFT,
            currentMapTopLeftPosition,
            leftMapTopLeftPosition,
            MAX_SCROLL_TIME_IN_SECONDS));

    // SWITCH THE CURRENT MAP WITH THE LEFT MAP.
    m_overworldMap->SetRightTileMap(currentTileMap);
    m_overworldMap->SetCurrentTileMap(leftTileMap);

    // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
    // The new right tile map has already been loaded.
    MATH::Vector2ui newCurrentMapOverworldGridPosition = leftTileMap->GetOverworldGridPosition();
    m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

    // SCROLLING LEFT BEGAN SUCCESSFULLY.
    return true;
}

bool ScrollableOverworldMap::BeginScrollingRight()
{
    // GET THE TILE MAPS REQUIRED TO START SCROLLING RIGHT.
    std::shared_ptr<TileMap> currentTileMap = m_overworldMap->GetCurrentTileMap();
    std::shared_ptr<MAPS::TileMap> rightTileMap = m_overworldMap->GetRightTileMap();
    bool tileMapsForScrollingRightExist = (nullptr != currentTileMap && nullptr != rightTileMap);
    if (!tileMapsForScrollingRightExist)
    {
        // We can't start scrolling right without both current and right tile maps.
        return false;
    }

    // GET THE STARTING AND ENDING POINTS FOR SCROLLING.
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    MATH::Vector2f rightMapTopLeftPosition = rightTileMap->GetTopLeftWorldPosition();

    // START THE SCROLLING PROCESS.
    m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
        new PROCESSES::ScrollProcess(
            PROCESSES::ScrollProcess::ScrollDirection::RIGHT,
            currentMapTopLeftPosition,
            rightMapTopLeftPosition,
            MAX_SCROLL_TIME_IN_SECONDS));

    // SWITCH THE CURRENT MAP WITH THE RIGHT MAP.
    m_overworldMap->SetLeftTileMap(currentTileMap);
    m_overworldMap->SetCurrentTileMap(rightTileMap);

    // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
    // The new left tile map has already been loaded.
    MATH::Vector2ui newCurrentMapOverworldGridPosition = rightTileMap->GetOverworldGridPosition();
    m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);
    m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

    // SCROLLING RIGHT BEGAN SUCCESSFULLY.
    return true;
}

bool ScrollableOverworldMap::IsScrolling() const
{
    // Scrolling is only active if a scroll process exists.
    bool scrollingActive = (nullptr != m_scrollProcess.get());
    return scrollingActive;
}

MATH::Vector2f ScrollableOverworldMap::Scroll(const float elapsedTimeInSeconds)
{
    if (IsScrolling())
    {
        return m_scrollProcess->Scroll(elapsedTimeInSeconds);
    }
    else
    {
        return MATH::Vector2f();
    }
}

bool ScrollableOverworldMap::IsScrollingComplete() const
{
    if (IsScrolling())
    {
        return m_scrollProcess->IsComplete();
    }
    else
    {
        // There isn't a scrolling process, so no scrolling has occurred.
        // Therefore, the scrolling is effectively "complete".
        return true;
    }
}

void ScrollableOverworldMap::UpdateSurroundingMapsAfterScrolling(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem)
{
    // VERIFY THAT A CURRENT CENTER MAP EXISTS.
    // This center map is required to make any updates.
    std::shared_ptr<MAPS::TileMap> centerMap = m_overworldMap->GetCurrentTileMap();
    bool centerMapExists = (nullptr != centerMap);
    if (!centerMapExists)
    {
        return;
    }
    
    // UPDATE THE MAPS DEPENDING ON THE DIRECTION.
    // If one of the valid directions wasn't specified, then we just won't
    // update the maps since we don't know what to do.
    PROCESSES::ScrollProcess::ScrollDirection direction = m_scrollProcess->GetDirection();
    switch (direction)
    {
        // The current and bottom maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::UP:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                graphicsSystem);
            m_overworldMap->SetTopTileMap(newTopMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                graphicsSystem);
            m_overworldMap->SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                graphicsSystem);
            m_overworldMap->SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and top maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::DOWN:
        {
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                graphicsSystem);
            m_overworldMap->SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                graphicsSystem);
            m_overworldMap->SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                graphicsSystem);
            m_overworldMap->SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and right maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::LEFT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                graphicsSystem);
            m_overworldMap->SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                graphicsSystem);
            m_overworldMap->SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                graphicsSystem);
            m_overworldMap->SetLeftTileMap(newLeftTileMap);
            break;
        }
        // The current and left maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::RIGHT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                graphicsSystem);
            m_overworldMap->SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                graphicsSystem);
            m_overworldMap->SetBottomTileMap(newBottomMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                graphicsSystem);
            m_overworldMap->SetRightTileMap(newRightTileMap);
            break;
        }
    }

    /// RESET THE SCROLL PROCESS.
    /// @todo   This probably should go in a separate or better named method.
    m_scrollProcess.reset();
}