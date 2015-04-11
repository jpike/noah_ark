#pragma once

#include <map>
#include <memory>
#include <TmxParser/Tmx.h>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/IGraphicsComponent.h"
#include "Maps/Tile.h"
#include "Maps/TiledMapJsonFile.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// Represents the position of a tile map in the overworld's "grid".
    /// These positions are relative to each other.  For example, a map
    /// at x,y position 1,1 is to the right of a map at position 0,1, and
    /// a map at position 0,0 is above a map at position 0,1.
    typedef MATH::Vector2ui OverworldGridPosition;
    
    /// A single 2D map composed of individual tiles and any objects
    /// that may located on the map.
    ///
    /// It is constructed from data in a Tiled map.
    class TileMap
    {
    public:
        /// @brief          Constructor.
        /// @param[in]      overworld_grid_position - The position of the tile map within the overworld grid.
        /// @param[in]      top_left_world_position_in_pixels - The top-left position of the map within the world.
        /// @param[in]      map_data - The underlying map that has already been loaded.
        /// @param[in,out]  graphics_system - The graphics system used to manage graphics for this tile map.
        explicit TileMap(
            const OverworldGridPosition& overworld_grid_position,
            const MATH::Vector2f& top_left_world_position_in_pixels,
            const TiledMapJsonFile& map_data,
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system);
        
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

        /// @brief  Gets the center world position of the tile map, in pixels.
        /// @return The center position of the map.
        MATH::Vector2f GetCenterWorldPosition() const;

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

        /// @brief  Gets the width and height of the tile map, in pixels.
        /// @return The size of the tile map.
        MATH::Vector2f GetSizeInPixels() const;

        /// @brief      Gets the tile at the specified world position.
        /// @param[in]  world_x_position - The world X position at which to get the tile.
        /// @param[in]  world_y_position - The world Y position at which to get the tile.
        /// @return     The tile at the specified world position, if one exists.
        std::shared_ptr<Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;

    private:
        TileMap(const TileMap& mapToCopy);  ///< Private to disallow copying.
        TileMap& operator= (const TileMap& rhsMap); ///< Private to disallow assignment.

        /// @brief          Populates this tile map from the provided data.
        /// @param[in]      top_left_world_position_in_pixels - The top-left position of the map within the world.
        /// @param[in]      map_data - The underlying map that has already been loaded.
        /// @param[in,out]  graphics_system - The graphics system used to manage graphics for this tile map.
        void BuildFromMapData(
            const MATH::Vector2f& top_left_world_position_in_pixels,
            const TiledMapJsonFile& map_data,
            GRAPHICS::GraphicsSystem& graphics_system);

        /// @brief  Tiles within the map, stored by their 2D positions relative to each other.
        ///         The x (horizontal) coordinate comes first, followed by the y (vertical) coordinate.
        ///         For example, the tile at (0,0) is the top-left tile.  The tile at (1,0) is right of the top-left tile.
        ///         The tile at (0,1) is below the top-left tile.
        std::vector< std::vector< std::shared_ptr<Tile> > > Tiles;

        TiledMapJsonFile MapData; ///< The map file used to create this map.

        OverworldGridPosition OverworldPosition;    ///< The position of the tile map in the overworld map's grid.
        MATH::Vector2f TopLeftWorldPositionInPixels;  ///< The top-left world position of the tile map.
    };
}