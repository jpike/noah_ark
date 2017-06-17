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
    /// The entire overworld of the game.  The overworld is divided into smaller chunks
    /// of individual tile maps.
    class Overworld
    {
    public:
        // STATIC DIMENSION CONSTANTS.
        /// The width of the overworld, in tile maps.
        static const unsigned int WIDTH_IN_TILE_MAPS = 7;
        /// The height of the overworld, in tile maps.
        static const unsigned int HEIGHT_IN_TILE_MAPS = 7;

        // TILE MAP RETRIEVAL.
        const MAPS::TileMap* GetTileMap(const unsigned int row, const unsigned int column) const;
        MAPS::TileMap* GetTileMap(const unsigned int row, const unsigned int column);
        const MAPS::TileMap* GetTileMap(const float world_x_position, const float world_y_position) const;
        MAPS::TileMap* GetTileMap(const float world_x_position, const float world_y_position);

        // TILE RETRIEVAL.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Tile maps in the overworld, arranged by column/row position in a grid rather than
        /// by absolute world positions.  (0,0) (column/x,row/y) is the top-left tile map.
        CORE::Array2D<MAPS::TileMap> TileMaps = CORE::Array2D<MAPS::TileMap>(WIDTH_IN_TILE_MAPS, HEIGHT_IN_TILE_MAPS);
        /// Noah (the player) character within the world.
        std::unique_ptr<OBJECTS::Noah> NoahPlayer = nullptr;
        /// Axe swings currently occurring in the world.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > AxeSwings = {};
        /// The background music for the overworld.
        std::shared_ptr<sf::Music> BackgroundMusic = nullptr;
    };
}