#pragma once

#include <map>
#include <memory>
#include "Maps/TileMap.h"
#include "Math/Vector2.h"

/// Holds code related to maps in the game.
namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Represents the entire overworld map, which is composed of many
    ///         individual tile maps (and objects on them) that are connected together.
    ///////////////////////////////////////////////////////////
    class OverworldMap
    {
    public:
        /// @brief  Constructor.
        explicit OverworldMap();
        /// @brief  Destructor.
        ~OverworldMap();

        /// @brief      Sets the current tile map displayed by the overworld.
        /// @param[in]  tileMap - The new current tile map.
        void SetCurrentTileMap(const std::shared_ptr<TileMap>& tileMap);

    private:
        OverworldMap(const OverworldMap& mapToCopy);  ///< Private to disallow copying.
        OverworldMap& operator= (const OverworldMap& rhsMap); ///< Private to disallow assignment.

        /// @brief  Tile maps within the overworld map, stored by their 2D positions relative to each other.
        ///         The x (horizontal) coordinate comes first, followed by the y (vertical) coordinate.
        ///         For example, the map at (0,0) is the top-left map.  The map at (1,0) is right of the top-left map.
        ///         The map at (0,1) is below the top-left map.
        std::vector< std::vector< std::shared_ptr<TileMap> > > m_tileMaps;

        /// @todo   Consider removing the above array and instead just using this initial map.
        std::shared_ptr<TileMap> m_currentTileMap;
    };
}