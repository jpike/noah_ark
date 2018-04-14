#pragma once

#include <memory>
#include <vector>
#include <SFML/Audio.hpp>
#include "Core/Array2D.h"
#include "Events/AxeSwingEvent.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"

/// Holds code related to maps in the game (primarily tile maps).
namespace MAPS
{
    /// A grid-based organizational mechanism for collecting a set of related tile maps
    /// that form a larger "map" (for example, an overworld map or interior area may
    /// be made of multiple individual tile maps).
    class MultiTileMapGrid
    {
    public:
        // CONSTRUCTION.
        explicit MultiTileMapGrid(const unsigned int width_in_tile_maps, const unsigned int height_in_tile_maps);

        // TILE MAP RETRIEVAL.
        const MAPS::TileMap* GetTileMap(const unsigned int row, const unsigned int column) const;
        MAPS::TileMap* GetTileMap(const unsigned int row, const unsigned int column);
        const MAPS::TileMap* GetTileMap(const float world_x_position, const float world_y_position) const;
        MAPS::TileMap* GetTileMap(const float world_x_position, const float world_y_position);

        // TILE RETRIEVAL.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Tile maps in the grid, arranged by column/row position in the grid rather than
        /// by absolute world positions.  (0,0) (column/x,row/y) is the top-left tile map.
        CORE::Array2D<MAPS::TileMap> TileMaps;
        /// Axe swings currently occurring in the map grid.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > AxeSwings;
    };
}