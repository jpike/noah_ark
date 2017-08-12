#include "Objects/Food.h"

namespace OBJECTS
{
    /// Makes the food fall more based on the provided time.
    /// @param[in]  time - The time to make the food fall.
    void FallingFood::Fall(const sf::Time& time)
    {
        // CALCULATE HOW FAR TO MAKE THE FOOD FALL.
        constexpr float FALL_SPEED_IN_PIXELS_PER_SECOND = 20.0f;
        float time_in_seconds = time.asSeconds();
        float fall_distance_in_pixels = FALL_SPEED_IN_PIXELS_PER_SECOND * time_in_seconds;

        // UPDATE THE FOOD'S POSITION.
        MATH::Vector2f food_world_position = FoodItem.Sprite.GetWorldPosition();
        food_world_position.Y += fall_distance_in_pixels;
        FoodItem.Sprite.SetWorldPosition(food_world_position);
    }

    /// Checks if the food has finished falling.
    /// @return True if the food has finished falling; false otherwise.
    bool FallingFood::FinishedFalling() const
    {
        // The food has finished falling if its y position has at least
        // reached the destination position.
        MATH::Vector2f food_world_position = FoodItem.Sprite.GetWorldPosition();
        bool finished_falling = (food_world_position.Y >= DestinationWorldPosition.Y);
        return finished_falling;
    }
}
