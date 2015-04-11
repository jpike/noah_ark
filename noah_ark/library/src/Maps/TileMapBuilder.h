#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Maps/TileMap.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for building complete TileMaps from Tiled
    ///         TMX maps.
    ///////////////////////////////////////////////////////////
    class TileMapBuilder
    {
    public:
        /// @brief      Constructor.
        /// @param      graphicsSystem - The graphics system supplying graphics for tile maps
        ///             built by this class.
        explicit TileMapBuilder(const std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        /// @brief  Destructor.
        ~TileMapBuilder();

        /// @brief          Builds a complete tile map for the TMX map that is positioned at the specified location.
        /// @param[in]      overworldGridPosition - The position of the tile map within the overworld grid.
        /// @param[in]      topLeftWorldPositionInPixels - The top-left position of the map within the world.
        /// @param[in]      tmxMap - The TMX map providing data for the tile map to create.
        /// @return         The complete tile map for the TMX map, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildTileMap(
            const OverworldGridPosition& overworldGridPosition,
            const MATH::Vector2f& topLeftWorldPositionInPixels,
            const std::shared_ptr<Tmx::Map>& tmxMap);

        /// @brief          Builds a complete tile map for the TMX map that is positioned above
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the top map.
        /// @param[in]      topTmxMap - The TMX map providing data for the tile map to create.
        /// @return         The complete tile map for the TMX map, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildTopTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& topTmxMap);

        /// @brief          Builds a complete tile map for the TMX map that is positioned below
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the below map.
        /// @param[in]      bottomTmxMap - The TMX map providing data for the tile map to create.
        /// @return         The complete tile map for the TMX map, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildBottomTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& bottomTmxMap);

        /// @brief          Builds a complete tile map for the TMX map that is positioned left of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the left map.
        /// @param[in]      leftTmxMap - The TMX map providing data for the tile map to create.
        /// @return         The complete tile map for the TMX map, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildLeftTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& leftTmxMap);

        /// @brief          Builds a complete tile map for the TMX map that is positioned right of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the right map.
        /// @param[in]      rightTmxMap - The TMX map providing data for the tile map to create.
        /// @return         The complete tile map for the TMX map, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildRightTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& rightTmxMap);

    private:
        TileMapBuilder(const TileMapBuilder& builderToCopy);    ///< Private to disallow copying.
        TileMapBuilder& operator=(const TileMapBuilder& rhsBuilder);    ///< Private to disallow assignment.

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system supplying graphics for tile maps built by this class.
    };
}