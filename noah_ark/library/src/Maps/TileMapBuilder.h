#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Maps/TiledMapJsonFile.h"
#include "Maps/TileMap.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for building complete TileMaps from Tiled maps.
    ///////////////////////////////////////////////////////////
    class TileMapBuilder
    {
    public:
        /// @brief      Constructor.
        /// @param      graphics_system - The graphics system supplying graphics for tile maps
        ///             built by this class.
        explicit TileMapBuilder(const std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system);
        /// @brief  Destructor.
        ~TileMapBuilder();

        /// Builds a complete tile map for the map file that is positioned at the specified location.
        /// @param[in]  overworld_grid_position - The position of the tile map within the overworld grid.
        /// @param[in]  top_left_world_position_in_pixels - The top-left position of the map within the world.
        /// @param[in]  map_data - The data for the tile map to create.
        /// @return     The complete tile map for the map file, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildTileMap(
            const OverworldGridPosition& overworld_grid_position,
            const MATH::Vector2f& top_left_world_position_in_pixels,
            const TiledMapJsonFile& map_data);

        /// @brief          Builds a complete tile map for the TMX map that is positioned above
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the top map.
        /// @param[in]      top_map_data - The data for the tile map to create.
        /// @return         The complete tile map for the data, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildTopTileMap(
            const MAPS::TileMap& center_map,
            const std::shared_ptr<TiledMapJsonFile>& top_map_data);

        /// @brief          Builds a complete tile map for the TMX map that is positioned below
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the below map.
        /// @param[in]      bottom_map_data - The data for the tile map to create.
        /// @return         The complete tile map for the data, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildBottomTileMap(
            const MAPS::TileMap& center_map,
            const std::shared_ptr<TiledMapJsonFile>& bottom_map_data);

        /// @brief          Builds a complete tile map for the TMX map that is positioned left of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the left map.
        /// @param[in]      left_map_data - The data for the tile map to create.
        /// @return         The complete tile map for the data, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildLeftTileMap(
            const MAPS::TileMap& center_map,
            const std::shared_ptr<TiledMapJsonFile>& left_map_data);

        /// @brief          Builds a complete tile map for the TMX map that is positioned right of
        ///                 the provided center map.
        /// @param[in]      centerMap - The center map providing a reference point for positioning
        ///                 the right map.
        /// @param[in]      right_map_data - The data for the tile map to create.
        /// @return         The complete tile map for the data, if successfully built.
        std::shared_ptr<MAPS::TileMap> BuildRightTileMap(
            const MAPS::TileMap& center_map,
            const std::shared_ptr<TiledMapJsonFile>& right_map_data);

    private:
        TileMapBuilder(const TileMapBuilder& builderToCopy);    ///< Private to disallow copying.
        TileMapBuilder& operator=(const TileMapBuilder& rhsBuilder);    ///< Private to disallow assignment.

        std::shared_ptr<GRAPHICS::GraphicsSystem> GraphicsSystem; ///< The graphics system supplying graphics for tile maps built by this class.
    };
}