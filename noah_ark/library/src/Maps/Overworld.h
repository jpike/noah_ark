#pragma once

#include <memory>
#include <random>
#include <string>
#include <vector>
#include "Bible/BibleVerses.h"
#include "Core/Array2D.h"
#include "Events/AxeSwingEvent.h"
#include "Graphics/Camera.h"
#include "Input/KeyboardInputController.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"

namespace MAPS
{
    /// The entire overworld of the game.  The overworld is divided into smaller chunks
    /// of individual tile maps.
    class Overworld
    {
    public:
        // CONSTRUCTION.
        explicit Overworld(
            const unsigned int width_in_tile_maps, 
            const unsigned int height_in_tile_maps,
            const unsigned int tile_map_width_in_tiles,
            const unsigned int tile_map_height_in_tiles,
            const unsigned int tile_dimension_in_pixels);

        // UPDATING.
        void Update(
            const float elapsed_time_in_seconds,
            std::random_device& random_number_generator,
            INPUT_CONTROL::KeyboardInputController& input_controller,
            std::vector<BIBLE::BibleVerse>& bible_verses_left_to_find,
            RESOURCES::Assets& assets,
            GRAPHICS::Camera& camera,
            std::string& message_for_text_box);

        // TILE MAP RETRIEVAL.
        MAPS::TileMap* GetTileMap(const unsigned int row, const unsigned int column);
        MAPS::TileMap* GetTileMap(const float world_x_position, const float world_y_position);

        // TILE RETRIEVAL.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Tile maps in the overworld, arranged by column/row position in a grid rather than
        /// by absolute world positions.  (0,0) (column/x,row/y) is the top-left tile map.
        CORE::Array2D<MAPS::TileMap> TileMaps;
        /// The width of a single tile map in the overworld (in units of tiles).
        unsigned int TileMapWidthInTiles;
        /// The height of a single tile map in the overworld (in units of tiles).
        unsigned int TileMapHeightInTiles;
        /// The dimension (both width and height) of a single tile in the overworld
        /// (in units of pixels).
        unsigned int TileDimensionInPixels;
        /// Noah (the player) character within the world.
        OBJECTS::Noah NoahPlayer;
        /// Axe swings currently occurring in the world.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > AxeSwings;
    };
}