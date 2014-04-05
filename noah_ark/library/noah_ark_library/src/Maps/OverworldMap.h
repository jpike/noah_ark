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
        /// @todo   Remove the graphics system parameter once properly loading the overworld map is supported.
        explicit OverworldMap(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        /// @brief  Destructor.
        ~OverworldMap();

    private:
        OverworldMap(const OverworldMap& mapToCopy);  ///< Private to disallow copying.
        OverworldMap& operator= (const OverworldMap& rhsMap); ///< Private to disallow assignment.

        /// @brief  Tile maps within the overworld map, stored by their 2D positions relative to each other.
        ///         The x (horizontal) coordinate comes first, followed by the y (vertical) coordinate.
        ///         For example, the map at (0,0) is the top-left map.  The map at (1,0) is right of the top-left map.
        ///         The map at (0,1) is below the top-left map.
        std::vector< std::vector< std::shared_ptr<TileMap> > > m_tileMaps;
    };
}