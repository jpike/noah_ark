#pragma once

#include <map>
#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/IGraphicsComponent.h"
#include "Maps/ITileMapData.h"
#include "Maps/Tile.h"
#include "Maps/TileMapLayers.h"
#include "Maps/Tileset.h"
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
    ///
    /// @todo   Document implementing the IGraphicsComponent interface.
    class TileMap : public GRAPHICS::IGraphicsComponent
    {
    public:
        /// @brief          Constructor.  The map won't be visible by default.
        /// @param[in]      overworld_grid_position - The position of the tile map within the overworld grid.
        /// @param[in]      top_left_world_position_in_pixels - The top-left position of the map within the world.
        /// @param[in]      map_data - The underlying map that has already been loaded.
        /// @param[in,out]  graphics_system - The graphics system used to manage graphics for this tile map.
        /// @throws std::invalid_argument - Thrown if any pointer parameters are null.
        explicit TileMap(
            const OverworldGridPosition& overworld_grid_position,
            const MATH::Vector2f& top_left_world_position_in_pixels,
            const std::shared_ptr<ITileMapData>& map_data,
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

        /// Changes the visibility of this tile map.
        /// @param[in]  visible - True if this tile map should be visible;
        ///     false if this tile map should be hidden.
        void SetVisible(const bool visible);

        // INTERFACE IMPLEMENTATION - IGraphicsComponent
        /// @copydoc    GRAPHICS::IGraphicsComponent::IsVisible
        virtual bool IsVisible() const;
        /// @copydoc    GRAPHICS::IGraphicsComponent::Render
        virtual void Render(sf::RenderTarget& render_target);
        /// @copydoc    GRAPHICS::IGraphicsComponent::Update
        virtual void Update(const float elapsed_time_in_seconds);

    private:
        TileMap(const TileMap& map_to_copy);  ///< Private to disallow copying.
        TileMap& operator= (const TileMap& rhs_map); ///< Private to disallow assignment.

        /// @brief          Populates this tile map from the provided data.
        /// @param[in]      top_left_world_position_in_pixels - The top-left position of the map within the world.
        /// @param[in]      map_data - The underlying map that has already been loaded.
        /// @param[in,out]  graphics_system - The graphics system used to manage graphics for this tile map.
        void BuildFromMapData(
            const MATH::Vector2f& top_left_world_position_in_pixels,
            const ITileMapData& map_data,
            GRAPHICS::GraphicsSystem& graphics_system);

        /// Whether or not the tile map is currently visible.
        bool Visible;
        /// The position of the tile map in the overworld map's grid.
        OverworldGridPosition OverworldPosition;
        ///< The top-left world position of the tile map.
        MATH::Vector2f TopLeftWorldPositionInPixels;
        /// The width of the map in tiles.
        unsigned int WidthInTiles;
        /// The height of the map in tiles.
        unsigned int HeightInTiles;
        /// The width (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileWidthInPixels;
        /// The height (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileHeightInPixels;
        /// The set of tiles used in this map.
        MAPS::Tileset Tileset;
        /// Layers within this tile map.
        TileMapLayers Layers;
    };
}