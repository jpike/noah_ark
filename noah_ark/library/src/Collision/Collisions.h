#pragma once

#include <memory>
#include <vector>
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Maps/Overworld.h"
#include "Math/Rectangle.h"
#include "Objects/Axe.h"

namespace COLLISION
{
    bool CollidesWithTree(MAPS::Overworld& overworld, const MATH::FloatRectangle& rectangle, MATH::FloatRectangle& tree_rectangle);

    MATH::Vector2f MoveUpWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveDownWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveLeftWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveRightWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveWithCollisionDetection(
        MAPS::Overworld& overworld,
        const float elapsed_time_in_seconds,
        const CORE::Direction direction,
        const float move_speed_in_pixels_per_second,
        const MATH::FloatRectangle& object_world_bounding_box);

    void HandleAxeCollisionsWithTrees(const OBJECTS::Axe& axe, MAPS::Overworld& overworld, RESOURCES::Assets& assets);

    void HandleAxeSwingCollisions(
        MAPS::Overworld& overworld, 
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings,
        RESOURCES::Assets& assets);
}
