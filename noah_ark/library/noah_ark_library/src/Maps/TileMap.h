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
    /// @brief  Represents the position of a tile map in the overworld's "grid".
    ///         These positions are relative to each other.  For example, a map
    ///         at x,y position 1,1 is to the right of a map at position 0,1, and
    ///         a map at position 0,0 is above a map at position 0,1.
    typedef MATH::Vector2ui OverworldGridPosition;
    
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
        /// @param[in]      overworldGridPosition - The position of the tile map within the overworld grid.
        /// @param[in]      topLeftWorldPositionInPixels - The top-left position of the map within the world.
        /// @param[in]      map - The underlying map that has already been loaded.
        /// @param[in,out]  graphicsSystem - The graphics system used to manage graphics for this tile map.
        explicit TileMap(
            const OverworldGridPosition& overworldGridPosition,
            const MATH::Vector2f& topLeftWorldPositionInPixels,
            const std::shared_ptr<Tmx::Map>& map, 
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        
        /// @brief  Destructor.
        virtual ~TileMap();

        /// @brief  Gets the position of the tile map within the overworld grid.
        /// @return The overworld grid position of the tile map.
        OverworldGridPosition GetOverworldGridPosition() const;
        
        /// @brief  Gets the top-left world position of the tile map, in pixels.
        /// @return The top-left position of the map.
        MATH::Vector2f GetTopLeftWorldPosition() const;

        /// @brief  Gets the bottom-right world position of the tile map, in pixels.
        /// @return The bottom-right position of the map.
        MATH::Vector2f GetBottomRightWorldPosition() const;

        /// @brief  Gets the width of the map, in tiles.
        /// @return The width of the map, in tiles.
        unsigned int GetWidthInTiles() const;

        /// @brief  Gets the height of the map, in tiles.
        /// @return The height of the map, in tiles.
        unsigned int GetHeightInTiles() const;

        /// @brief  Gets the width of a tile in the map, in pixels.
        /// @return The width of a single tile.
        unsigned int GetTileWidthInPixels() const;

        /// @brief  Gets the height of a tile in the map, in pixels.
        /// @return The height of single tile.
        unsigned int GetTileHeightInPixels() const;

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

        std::shared_ptr<Tmx::Map> m_tmxMap; ///< The TMX map used to create this map.

        OverworldGridPosition m_overworldGridPosition;    ///< The position of the tile map in the overworld map's grid.
        MATH::Vector2f m_topLeftWorldPositionInPixels;  ///< The top-left world position of the tile map.
    };
}