#pragma once

#include <memory>
#include <vector>
#include <SFML/System.hpp>
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Maps/Overworld.h"
#include "Math/Rectangle.h"
#include "Objects/Axe.h"
#include "Resources/Assets.h"

namespace COLLISION
{
    /// Encapsulates various algorithms related to collision detection.
    class CollisionDetectionAlgorithms
    {
    public:
        // MOVEMENT.
        static MATH::Vector2f MoveObject(
            const MATH::FloatRectangle& object_world_bounding_box,
            const CORE::Direction direction,
            const float move_speed_in_pixels_per_second,
            const sf::Time& elapsed_time,
            MAPS::Overworld& overworld);

        // AXE SWINGS.
        static void HandleAxeSwings(
            MAPS::Overworld& overworld,
            std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings,
            RESOURCES::Assets& assets);

    private:
        // MOVEMENT.
        static MATH::Vector2f MoveObjectUp(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::Overworld& overworld);
        static MATH::Vector2f MoveObjectDown(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::Overworld& overworld);
        static MATH::Vector2f MoveObjectLeft(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::Overworld& overworld);
        static MATH::Vector2f MoveObjectRight(
            const MATH::FloatRectangle& object_world_bounding_box, 
            const COLLISION::Movement& movement, 
            MAPS::Overworld& overworld);

        // AXE SWINGS.
        static void HandleAxeCollisionsWithTrees(const OBJECTS::Axe& axe, MAPS::Overworld& overworld, RESOURCES::Assets& assets);

        // OBJECT-SPECIFIC COLLISION DETECTION.
        static bool CollidesWithTree(const MATH::FloatRectangle& rectangle, MAPS::Overworld& overworld, MATH::FloatRectangle& tree_rectangle);
    };
}