#pragma once

#include <memory>
#include "Maps/TiledMapJsonFile.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for loading Tiled maps.
    ///////////////////////////////////////////////////////////
    class TileMapLoader
    {
    public:
        /// @brief  Constructor.
        explicit TileMapLoader();
        
        /// @brief  Destructor.
        ~TileMapLoader();

        /// @brief      Attempts to load a Tiled map from file.
        /// @param[in]  filepath - The path to the Tiled map file to load.
        /// @return     The Tiled map map, if loaded successfully; nullptr otherwise.
        std::shared_ptr<TiledMapJsonFile> LoadMap(const std::string& filepath) const;

    private:
        TileMapLoader(const TileMapLoader& mapToCopy);  ///< Private to disallow copying.
        TileMapLoader& operator= (const TileMapLoader& rhsMap); ///< Private to disallow assignment.
    };
}