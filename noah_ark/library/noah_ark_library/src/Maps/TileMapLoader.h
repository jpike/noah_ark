#pragma once

#include <memory>
#include <TmxParser/Tmx.h>

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for loading Tiled TMX maps.
    ///////////////////////////////////////////////////////////
    class TileMapLoader
    {
    public:
        /// @brief  Constructor.
        explicit TileMapLoader();
        
        /// @brief  Destructor.
        ~TileMapLoader();

        /// @brief      Attempts to load a Tiled TMX from file.
        /// @param[in]  filepath - The path to the Tiled TMX file to load.
        /// @return     The Tiled TMX map, if loaded successfully; nullptr otherwise.
        std::shared_ptr<Tmx::Map> LoadMap(const std::string& filepath) const;

    private:
        TileMapLoader(const TileMapLoader& mapToCopy);  ///< Private to disallow copying.
        TileMapLoader& operator= (const TileMapLoader& rhsMap); ///< Private to disallow assignment.
    };
}