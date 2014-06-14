#pragma once

#include <map>
#include <memory>
#include <TmxParser/Tmx.h>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/IGraphicsComponent.h"
#include "Maps/Tile.h"
#include "Math/Vector2.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A single 2D map composed of individual tiles and any objects
    ///         that may located on the map.
    ///
    ///         It is constructed from data in a Tiled TMX map.
    ///
    ///         @todo This class implements the IGraphicsComponent interface
    ///         to allow individual tile maps to be completely included
    ///         or excluded from rendering, as opposed to individual tiles.
    ///         This allows improved rendering efficiency by reducing the
    ///         number of components to be examined for rendering from 
    ///         a large number of individual tiles to a small number of
    ///         larger tile maps.
    ///////////////////////////////////////////////////////////
    class TileMap// : public GRAPHICS::IGraphicsComponent
    {
    public:
        /// @brief          Constructor.
        /// @param[in]      topLeftWorldPositionInPixels - The top-left position of the map within the world.
        /// @param[in]      map - The underlying map that has already been loaded.
        /// @param[in,out]  graphicsSystem - The graphics system used to manage graphics for this tile map.
        explicit TileMap(
            const MATH::Vector2f& topLeftWorldPositionInPixels,
            const std::shared_ptr<Tmx::Map>& map, 
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        
        /// @brief  Destructor.
        virtual ~TileMap();

    private:
        TileMap(const TileMap& mapToCopy);  ///< Private to disallow copying.
        TileMap& operator= (const TileMap& rhsMap); ///< Private to disallow assignment.

        /// @brief          Populates this tile map with data from the provided TMX map.
        /// @param[in]      topLeftWorldPositionInPixels - The top-left position of the map within the world.
        /// @param[in]      map - The underlying map that has already been loaded.
        /// @param[in,out]  graphicsSystem - The graphics system used to manage graphics for this tile map.
        void BuildFromTmxMap(
            const MATH::Vector2f& topLeftWorldPositionInPixels,
            const std::shared_ptr<Tmx::Map>& map, 
            GRAPHICS::GraphicsSystem& graphicsSystem);

        /// @brief  Tiles within the map, stored by their 2D positions relative to each other.
        ///         The x (horizontal) coordinate comes first, followed by the y (vertical) coordinate.
        ///         For example, the tile at (0,0) is the top-left tile.  The tile at (1,0) is right of the top-left tile.
        ///         The tile at (0,1) is below the top-left tile.
        std::vector< std::vector< std::shared_ptr<Tile> > > m_tiles;
    };
}