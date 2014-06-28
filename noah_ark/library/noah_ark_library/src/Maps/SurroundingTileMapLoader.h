#pragma once

#include <future>
#include <memory>
#include <TmxParser/Tmx.h>
#include "Maps/OverworldMapSpecification.h"
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
    ///         This loader only loads Tiled TMX maps, not the 
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
        /// @param[in]  pOverworldMapSpec - The overworld map specification describing
        ///             which tile maps are adjacent to each other.  Memory is expected
        ///             to be externally managed from this class and must not be deleted
        ///             while this class is being used to load maps.
        /// @throws     std::invalid_argument - Thrown if the provided overworld map specification is null.
        explicit SurroundingTileMapLoader(const OverworldMapSpecification* const pOverworldMapSpec);
        
        /// @brief  Destructor.
        ~SurroundingTileMapLoader();

        /// @brief      Starts loading the TMX map above the specified map in a separate thread.
        ///             The TMX map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  centerTileMapOverworldGridPosition - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the top tile map.
        void StartLoadingTopTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition);
        /// @brief      Starts loading the TMX map below the specified map in a separate thread.
        ///             The TMX map may be later retrieved by calling GetBelowTileMap.
        /// @param[in]  centerTileMapOverworldGridPosition - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the bottom tile map.
        void StartLoadingBottomTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition);
        /// @brief      Starts loading the TMX map left of the specified map in a separate thread.
        ///             The TMX map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  centerTileMapOverworldGridPosition - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the left tile map.
        void StartLoadingLeftTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition);
        /// @brief      Starts loading the TMX map right of the specified map in a separate thread.
        ///             The TMX map may be later retrieved by calling GetTopTileMap.
        /// @param[in]  centerTileMapOverworldGridPosition - The overworld grid position of the 
        ///             center tile map definining the reference point for which to load the right tile map.
        void StartLoadingRightTileMap(const MATH::Vector2ui& centerTileMapOverworldGridPosition);

        /// @brief      Retrieves the top tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The top tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<Tmx::Map> GetTopTileMap();
        /// @brief      Retrieves the bottom tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The bottom tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<Tmx::Map> GetBottomTileMap();
        /// @brief      Retrieves the left tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The left tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<Tmx::Map> GetLeftTileMap();
        /// @brief      Retrieves the right tile map that has been loaded.
        ///             This is a blocking call that won't return until loading has finished.
        /// @return     The right tile map, if loaded and one exists.  Nullptr otherwise.
        std::shared_ptr<Tmx::Map> GetRightTileMap();

    private:
        SurroundingTileMapLoader(const SurroundingTileMapLoader& loaderToCopy);  ///< Private to disallow copying.
        SurroundingTileMapLoader& operator= (const SurroundingTileMapLoader& rhsLoader); ///< Private to disallow assignment.

        const OverworldMapSpecification* m_pOverworldMapSpec;   ///< The specification of the overworld describing which tile maps are adjacent to each other.

        TileMapLoader m_mapLoader;  ///< The actual loader responsible for loading maps.

        std::shared_ptr<Tmx::Map> m_topTmxMap;  ///< The top TMX map, if loaded.
        std::shared_ptr<Tmx::Map> m_bottomTmxMap;   ///< The bottom TMX map, if loaded.
        std::shared_ptr<Tmx::Map> m_leftTmxMap;    ///< The left TMX map, if loaded.
        std::shared_ptr<Tmx::Map> m_rightTmxMap;   ///< The right TMX map, if loaded.

        std::future< std::shared_ptr<Tmx::Map> > m_topTmxLoader;    ///< The future for loading the top TMX map.
        std::future< std::shared_ptr<Tmx::Map> > m_bottomTmxLoader; ///< The future for loading the bottom TMX map.
        std::future< std::shared_ptr<Tmx::Map> > m_leftTmxLoader; ///< The future for loading the left TMX map.
        std::future< std::shared_ptr<Tmx::Map> > m_rightTmxLoader;  ///< The future for loading the right TMX map.
    };
}