#pragma once

#include <map>
#include "Maps/Tile.h"
#include "Math/Vector2.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// \brief  A single 2D map composed of individual tiles and any objects
    ///         that may located on the map.
    ///////////////////////////////////////////////////////////
    class TileMap
    {
    public:
        /// @brief  Constructor.
        explicit TileMap();
        /// @brief  Destructor.
        ~TileMap();

    private:
        TileMap(const TileMap& mapToCopy);  ///< Private to disallow copying.
        TileMap& operator= (const TileMap& rhsMap); ///< Private to disallow assignment.

        /// Tiles within the map, keyed off by their 2D positions relative to each other.
        /// For example, the tile at (0,0) is the top-left tile.  The tile at (1,0) is right of the top-left tile.
        /// The tile at (0,1) is below the top-left tile.
        std::map<MATH::Vector2ui, Tile> m_tiles;
    };
}