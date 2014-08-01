#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Maps/TileMap.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for building complete TileMaps from Tiled
    ///         TMX maps.  This class only contains static utility
    ///         methods and is not intended to be instantiated.
    ///
    /// @todo   Refactor this so that it can be instantiated.  This
    ///         will allow it to access a graphics system.
    ///////////////////////////////////////////////////////////
    class TileMapBuilder
    {
    public:

        /// @brief          Builds a complete tile map for the TMX map that is positioned above
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the top map.
        /// @param[in]      topTmxMap - The TMX map providing data for the tile map to create.
        /// @param[in,out]  graphicsSystem - The graphics system supplying graphics for the tile map.
        /// @return         The complete tile map for the TMX map, if successfully built.
        static std::shared_ptr<MAPS::TileMap> BuildTopTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& topTmxMap,
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);

        /// @brief          Builds a complete tile map for the TMX map that is positioned below
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the below map.
        /// @param[in]      bottomTmxMap - The TMX map providing data for the tile map to create.
        /// @param[in,out]  graphicsSystem - The graphics system supplying graphics for the tile map.
        /// @return         The complete tile map for the TMX map, if successfully built.
        static std::shared_ptr<MAPS::TileMap> BuildBottomTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& bottomTmxMap,
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);

        /// @brief          Builds a complete tile map for the TMX map that is positioned left of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the left map.
        /// @param[in]      leftTmxMap - The TMX map providing data for the tile map to create.
        /// @param[in,out]  graphicsSystem - The graphics system supplying graphics for the tile map.
        /// @return         The complete tile map for the TMX map, if successfully built.
        static std::shared_ptr<MAPS::TileMap> BuildLeftTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& leftTmxMap,
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);

        /// @brief          Builds a complete tile map for the TMX map that is positioned right of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the right map.
        /// @param[in]      rightTmxMap - The TMX map providing data for the tile map to create.
        /// @param[in,out]  graphicsSystem - The graphics system supplying graphics for the tile map.
        /// @return         The complete tile map for the TMX map, if successfully built.
        static std::shared_ptr<MAPS::TileMap> BuildRightTileMap(
            const MAPS::TileMap& centerMap,
            const std::shared_ptr<Tmx::Map>& rightTmxMap,
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);

    private:
        /// @brief  Constructor.  Private to disallow construction.
        TileMapBuilder() {};
        /// @brief  Destructor.
        ~TileMapBuilder() {};
    };
}