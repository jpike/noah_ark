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
    bool CollidesWithTree(MAPS::Overworld& overworld, const MATH::FloatRectangle& rectangle, MATH::FloatRectangle& tree_rectangle);

    MATH::Vector2f MoveUpWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveDownWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveLeftWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveRightWithCollisionDetection(MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box);

    MATH::Vector2f MoveWithCollisionDetection(
        MAPS::Overworld& overworld,
        const sf::Time& elapsed_time,
        const CORE::Direction direction,
        const float move_speed_in_pixels_per_second,
        const MATH::FloatRectangle& object_world_bounding_box);

    void HandleAxeCollisionsWithTrees(const OBJECTS::Axe& axe, MAPS::Overworld& overworld, RESOURCES::Assets& assets);

    void HandleAxeSwingCollisions(
        MAPS::Overworld& overworld, 
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings,
        RESOURCES::Assets& assets);
}
