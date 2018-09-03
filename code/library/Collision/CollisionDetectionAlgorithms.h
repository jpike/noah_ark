#pragma once

#include <memory>
#include <unordered_set>
#include <vector>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/Tile.h"
#include "Math/Rectangle.h"
#include "Objects/Axe.h"
#include "Resources/Assets.h"

/// Holds code related to handling collisions of objects in the game world.
namespace COLLISION
{
    /// Encapsulates various algorithms related to collision detection.
    class CollisionDetectionAlgorithms
    {
    public:
        // MOVEMENT.
        static MATH::Vector2f MoveObject(
            const MATH::FloatRectangle& object_world_bounding_box,
            const MATH::Vector2f& move_vector,
            const std::unordered_set<MAPS::TileType::Id>& tile_types_allowed_to_move_over,
            const bool allow_movement_over_solid_objects,
            MAPS::MultiTileMapGrid& tile_map_grid);
        static MATH::Vector2f MoveObject(
            const MATH::FloatRectangle& object_world_bounding_box,
            const CORE::Direction direction,
            const float move_speed_in_pixels_per_second,
            const sf::Time& elapsed_time,
            MAPS::MultiTileMapGrid& tile_map_grid);

        // AXE SWINGS.
        static void HandleAxeSwings(
            MAPS::MultiTileMapGrid& tile_map_grid,
            std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings,
            AUDIO::Speakers& speakers,
            RESOURCES::Assets& assets);

    private:
        // MOVEMENT.
        static MATH::Vector2f MoveObjectUp(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::MultiTileMapGrid& tile_map_grid);
        static MATH::Vector2f MoveObjectDown(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::MultiTileMapGrid& tile_map_grid);
        static MATH::Vector2f MoveObjectLeft(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::MultiTileMapGrid& tile_map_grid);
        static MATH::Vector2f MoveObjectRight(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::MultiTileMapGrid& tile_map_grid);

        // AXE SWINGS.
        static void HandleAxeCollisionsWithTrees(
            const OBJECTS::Axe& axe, 
            MAPS::MultiTileMapGrid& tile_map_grid,
            AUDIO::Speakers& speakers,
            RESOURCES::Assets& assets);

        // OBJECT-SPECIFIC COLLISION DETECTION.
        static bool CollidesWithTree(const MATH::FloatRectangle& rectangle, MAPS::MultiTileMapGrid& tile_map_grid, MATH::FloatRectangle& tree_rectangle);
    };
}
