#pragma once

#include <future>
#include <memory>
#include "Maps/OverworldMapSpecification.h"
#include "Maps/TiledMapJsonFile.h"
#include "Maps/TileMap.h"
#include "Maps/TileMapLoader.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for loading tile maps surrounding
    ///         a specified center map.  It encapsulates loading
    ///         surrounding maps in additional threads to avoid
    ///         slowing down the main thread of the game.
    ///
    ///         This loader only loads Tiled maps, not the 
    ///         custom TileMap data structure of the game.
    ///         This is done because the primary reason for
    ///         multi-threaded loading of these tile maps is
    ///         access of disk resources (loading the map from file),
    ///         and additonal thread-safety would have to be added
    ///         to other classes to support full creation of the
    ///         custom TileMap data structure (and this additonal
    ///         complexity isn't warranted yet).
    ///////////////////////////////////////////////////////////
    class SurroundingTileMapLoader
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  overworld_map_spec - The overworld map specification describing
        ///             which tile maps are adjacent to each other.  Memory is expected
        ///             to be externally managed from this class and must not be deleted
        ///             while this class is being used to load maps.
        /// @throws     std::invalid_argument - Thrown if the provided overworld map specification is null.
        explicit SurroundingTileMapLoader(const OverworldMapSpecification* const overworld_map_spec);
        
        /// @brief  Destructor.
        ~SurroundingTileMapLoader();

        /// @brief      Starts loading the map above the specified map in a separate thread.
        ///             The map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  center_tile_map_overworld_grid_position - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the top tile map.
        void StartLoadingTopTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position);
        /// @brief      Starts loading the map below the specified map in a separate thread.
        ///             The map may be later retrieved by calling GetBelowTileMap.
        /// @param[in]  center_tile_map_overworld_grid_position - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the bottom tile map.
        void StartLoadingBottomTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position);
        /// @brief      Starts loading the map left of the specified map in a separate thread.
        ///             The map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  center_tile_map_overworld_grid_position - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the left tile map.
        void StartLoadingLeftTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position);
        /// @brief      Starts loading the map right of the specified map in a separate thread.
        ///             The map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  center_tile_map_overworld_grid_position - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the right tile map.
        void StartLoadingRightTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position);

        /// @brief      Retrieves the top tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The top tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<TiledMapJsonFile> GetTopTileMap();
        /// @brief      Retrieves the bottom tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The bottom tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<TiledMapJsonFile> GetBottomTileMap();
        /// @brief      Retrieves the left tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The left tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<TiledMapJsonFile> GetLeftTileMap();
        /// @brief      Retrieves the right tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The right tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<TiledMapJsonFile> GetRightTileMap();

    private:
        SurroundingTileMapLoader(const SurroundingTileMapLoader& other);  ///< Private to disallow copying.
        SurroundingTileMapLoader& operator= (const SurroundingTileMapLoader& rhs); ///< Private to disallow assignment.

        const OverworldMapSpecification* OverworldMapSpec;   ///< The specification of the overworld describing which tile maps are adjacent to each other.

        TileMapLoader MapLoader;  ///< The actual loader responsible for loading maps.

        std::shared_ptr<TiledMapJsonFile> TopMap;  ///< The top map, if loaded.
        std::shared_ptr<TiledMapJsonFile> BottomMap;   ///< The bottom map, if loaded.
        std::shared_ptr<TiledMapJsonFile> LeftMap;    ///< The left map, if loaded.
        std::shared_ptr<TiledMapJsonFile> RightMap;   ///< The right map, if loaded.

        std::future< std::shared_ptr<TiledMapJsonFile> > TopMapLoader;    ///< The future for loading the top map.
        std::future< std::shared_ptr<TiledMapJsonFile> > BottomMapLoader; ///< The future for loading the bottom map.
        std::future< std::shared_ptr<TiledMapJsonFile> > LeftMapLoader; ///< The future for loading the left map.
        std::future< std::shared_ptr<TiledMapJsonFile> > RightMapLoader;  ///< The future for loading the right map.
    };
}