#pragma once

#include <memory>
#include "Maps/OverworldMap.h"
#include "Maps/SurroundingTileMapLoader.h"
#include "Processes/ScrollProcess.h"

/// Holds code related to maps in the game.
namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Wraps an overworld map to enhance it with scrolling
    ///         capabilities.
    ///////////////////////////////////////////////////////////
    class ScrollableOverworldMap
    {
    public:
        /// @brief  The maximum time to spend scrolling between two individual tile maps.
        static const float MAX_SCROLL_TIME_IN_SECONDS;

        /// @brief      Constructor.
        /// @param      overworldMap - The overworld map to enhance with scrolling capabilities.
        /// @param[in]  pOverworldMapSpec - The overworld map specification describing
        ///             which tile maps are adjacent to each other.  Memory is expected
        ///             to be externally managed from this class and must not be deleted
        ///             while this class is being used to load maps.
        explicit ScrollableOverworldMap(
            const std::shared_ptr<OverworldMap>& overworldMap,
            const OverworldMapSpecification* const pOverworldMapSpec);
        /// @brief  Destructor.
        ~ScrollableOverworldMap();

        /// @brief  Begins scrolling the overworld map upwards so that the above tile map
        ///         will become the current tile map when scrolling is finished.
        /// @return True if scrolling up begins successfully; false otherwise.
        bool BeginScrollingUp();
        /// @brief  Begins scrolling the overworld map downwards so that the below tile map
        ///         will become the current tile map when scrolling is finished.
        /// @return True if scrolling down begins successfully; false otherwise.
        bool BeginScrollingDown();
        /// @brief  Begins scrolling the overworld map left so that the left tile map
        ///         will become the current tile map when scrolling is finished.
        /// @return True if scrolling left begins successfully; false otherwise.
        bool BeginScrollingLeft();
        /// @brief  Begins scrolling the overworld map right so that the right tile map
        ///         will become the current tile map when scrolling is finished.
        /// @return True if scrolling right begins successfully; false otherwise.
        bool BeginScrollingRight();

        /// @brief  Checks if the overworld map is currently scrolling between two adjacent
        ///         tile maps.
        /// @return True if the overworld map is scrolling between tile maps; false otherwise.
        bool IsScrolling() const;

        /// @brief      Continues scrolling between tile maps from the current point towards the end point,
        ///             if scrolling is currently active.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time to scroll for.
        /// @return     The current point after scrolling for the provided time, if scrolling is active.
        ///             Otherwise, a default point will be returned.
        MATH::Vector2f Scroll(const float elapsedTimeInSeconds);

        /// @brief  Checks whether scrolling is complete (has reached the end point) or not.
        /// @return True if scrolling is complete; false otherwise.
        bool IsScrollingComplete() const;

        /// @brief      Updates the overworld for the new surrounding maps after scrolling has finished.
        /// @param[in]  graphicsSystem - The graphics system supplying graphics for new surrounding maps.
        void UpdateSurroundingMapsAfterScrolling(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);

    private:
        ScrollableOverworldMap(const ScrollableOverworldMap& mapToCopy);  ///< Private to disallow copying.
        ScrollableOverworldMap& operator= (const ScrollableOverworldMap& rhsMap); ///< Private to disallow assignment.

        std::shared_ptr<OverworldMap> m_overworldMap;   ///< The overworld map being scrolled.
        std::unique_ptr<PROCESSES::ScrollProcess> m_scrollProcess;  ///< The process controlling scrolling of tile maps in the overworld map, if active (nullptr otherwise).
        std::unique_ptr<MAPS::SurroundingTileMapLoader> m_surroundingMapLoader;  ///< The loader of maps surrounding the center one.
    };
}