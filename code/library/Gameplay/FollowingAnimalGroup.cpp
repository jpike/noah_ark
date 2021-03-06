#include <cmath>
#include "Gameplay/FollowingAnimalGroup.h"
#include "Objects/Noah.h"

namespace GAMEPLAY
{
    /// Adds the specified animal to the group.
    /// @param[in]  animal - The animal to add.
    void FollowingAnimalGroup::Add(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal)
    {
        // GIVE THE ANIMAL A RANDOM POSITION WITHIN THIS GROUP.
        constexpr float GROUP_HALF_DIMENSION_IN_PIXELS = DIMENSION_IN_PIXELS / 2.0f;

        float min_group_world_x_position = CurrentCenterWorldPosition.X - GROUP_HALF_DIMENSION_IN_PIXELS;
        float max_group_world_x_position = CurrentCenterWorldPosition.X + GROUP_HALF_DIMENSION_IN_PIXELS;
        float animal_x_position = RandomNumberGenerator.RandomInRange<float>(min_group_world_x_position, max_group_world_x_position);

        float min_group_world_y_position = CurrentCenterWorldPosition.Y - GROUP_HALF_DIMENSION_IN_PIXELS;
        float max_group_world_y_position = CurrentCenterWorldPosition.Y + GROUP_HALF_DIMENSION_IN_PIXELS;
        float animal_y_position = RandomNumberGenerator.RandomInRange<float>(min_group_world_y_position, max_group_world_y_position);

        animal->Sprite.SetWorldPosition(animal_x_position, animal_y_position);

        // STORE THE ANIMAL IN THIS GROUP.
        Animals.emplace_back(animal);
    }

    /// Updates the following animal group.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    void FollowingAnimalGroup::Update(const sf::Time& elapsed_time)
    {
        // MAKE SURE THE ANIMAL'S HAVE SOMEONE TO FOLLOW.
        bool animals_have_someone_to_follow = (nullptr != Noah);
        if (!animals_have_someone_to_follow)
        {
            return;
        }

        // COMPUTE THE NEW CENTER POSITION FOR THE ANIMALS.
        // They should appear right behind Noah.
        MATH::FloatRectangle noah_world_bounding_box = Noah->GetWorldBoundingBox();
        // Defaults to appearing in the same location as Noah for easier visibility into problems.
        MATH::Vector2f noah_center_world_position = noah_world_bounding_box.Center();
        MATH::Vector2f new_center_world_position = noah_center_world_position;
        switch (Noah->FacingDirection)
        {
            case GAMEPLAY::Direction::UP:
                // The animals should appear below Noah.
                new_center_world_position.Y = noah_world_bounding_box.RightBottom.Y + GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::DOWN:
                // The animals should appear above Noah.
                new_center_world_position.Y = noah_world_bounding_box.LeftTop.Y - GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::LEFT:
                // The animals should appear to the right of Noah.
                new_center_world_position.X = noah_world_bounding_box.RightBottom.X + GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::RIGHT:
                // The animals should appear to the left of Noah.
                new_center_world_position.X = noah_world_bounding_box.LeftTop.X - GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
        }

        // UPDATE THE TRACKED ELAPSED TIME FOR THIS GROUP.
        ElapsedTime += elapsed_time;

        // UPDATE THE DESTINATION CENTER POSITION OF THE GROUP.
        // All of this and the updating below is done after the code above to prevent alterations to the current
        // center world position from affecting the individual animal positions too much.  In practice,
        // it probably doesn't matter whether we do this before/after updating individual animals
        // since this method will be called frequently (once per frame) to make any small "errors"
        // not that noticeable.
        CurrentDestinationCenterWorldPosition = new_center_world_position;

        // UPDATE THE CURRENT WORLD POSITION IF IT DIFFERS FROM THE DESTINATION.
        // By slowing bringing the current in line to the destination over time, there's a smoother transition
        // should the player change facing directions, resulting in drastically different destinations.
        MATH::Vector2f group_distance_from_current_to_destination_center_positions = CurrentDestinationCenterWorldPosition - CurrentCenterWorldPosition;
        MATH::Vector2f group_direction_from_current_to_destination_center_position = MATH::Vector2f::Normalize(group_distance_from_current_to_destination_center_positions);

        constexpr float MOVE_SPEED_IN_PIXELS_PER_SECOND_TO_DESTINATION_CENTER = 32.0f;
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        float group_movement_distance_toward_destination_center = MOVE_SPEED_IN_PIXELS_PER_SECOND_TO_DESTINATION_CENTER * elapsed_time_in_seconds;
        MATH::Vector2f group_movement_toward_destination_center = MATH::Vector2f::Scale(group_movement_distance_toward_destination_center, group_direction_from_current_to_destination_center_position);

        CurrentCenterWorldPosition += group_movement_toward_destination_center;

        // UPDATE THE POSITIONS/ANIMATIONS OF ALL OF THE ANIMALS.
        // The animal positions are in global world coordinates, but if the center world position of the group
        // is changing, then they need to be adjusted to be relative to the new center rather than the old center.
        for (const std::shared_ptr<OBJECTS::Animal>& animal : Animals)
        {
            // MOVE THE ANIMAL ALONG WITH THE GROUP.
            MATH::Vector2f old_animal_world_position = animal->Sprite.GetWorldPosition();
            MATH::Vector2f new_animal_world_position = old_animal_world_position + group_movement_toward_destination_center;

            // ADD A BIT OF A DYNAMIC OFFSET TO THE ANIMAL TO MAKE IT APPEAR TO JUMP AROUND A BIT.
            constexpr float PI = 3.14159f;
            constexpr float WAVE_PERIOD = 2.0f * PI;
            constexpr float JUMP_PERIOD_IN_SECONDS = 0.5f;

            // A small offset based on the x position of the animal is used to give a bit more dynamism
            // for animals by avoiding having all animals jump at the same frequency.
            constexpr float GROUP_HALF_DIMENSION_IN_PIXELS = DIMENSION_IN_PIXELS / 2.0f;
            float min_group_world_x_position = CurrentCenterWorldPosition.X - GROUP_HALF_DIMENSION_IN_PIXELS;
            float x_relative_to_group_box = (new_animal_world_position.X - min_group_world_x_position) / DIMENSION_IN_PIXELS;
                        
            float group_movement_total_elapsed_time_in_seconds = ElapsedTime.asSeconds();
            float relative_progression_through_jump_cycle = group_movement_total_elapsed_time_in_seconds + x_relative_to_group_box;
            float elapsed_time_ratio = relative_progression_through_jump_cycle / JUMP_PERIOD_IN_SECONDS;
            float elapsed_time_converted_to_angle_range_within_one_circle = elapsed_time_ratio * WAVE_PERIOD;
            
            // A sine wave is used to control vertical jumping.
            constexpr float MAX_VERTICAL_JUMP_AMOUNT_IN_PIXELS = 0.6f;
            float sine_of_elapsed_time = std::sinf(elapsed_time_converted_to_angle_range_within_one_circle);
            float vertical_jump_amount_in_pixels = MAX_VERTICAL_JUMP_AMOUNT_IN_PIXELS * sine_of_elapsed_time;
            new_animal_world_position.Y += vertical_jump_amount_in_pixels;

            // A cosine wave is used to control horizontal jumping.
            // This is smaller than the vertical jump amount in order to be more realistic/less distracting.
            constexpr float MAX_HORIZONTAL_JUMP_AMOUNT_IN_PIXELS = 0.1f;
            float cosine_of_elapsed_time = std::cosf(elapsed_time_converted_to_angle_range_within_one_circle);
            float horizontal_jump_amount_in_pixels = MAX_HORIZONTAL_JUMP_AMOUNT_IN_PIXELS * cosine_of_elapsed_time;
            new_animal_world_position.X += horizontal_jump_amount_in_pixels;

            // KEEP THE ANIMAL FROM DRIFTING TOO FAR AWAY.
            // The jumping around above can cause animals to drift too far away over time.
            // If that occurs, they need to be brought closer back into the group.
            constexpr float MAX_DISTANCE_BETWEEN_ANIMAL_AND_GROUP = 12.0f;
            MATH::Vector2f difference_in_animal_and_group_center_positions = new_animal_world_position - CurrentCenterWorldPosition;
            float distance_between_animal_and_group_center_positions = difference_in_animal_and_group_center_positions.Length();
            bool animal_moved_too_far_away = (distance_between_animal_and_group_center_positions > MAX_DISTANCE_BETWEEN_ANIMAL_AND_GROUP);
            if (animal_moved_too_far_away)
            {
                MATH::Vector2f direction_from_group_to_animal = MATH::Vector2f::Normalize(difference_in_animal_and_group_center_positions);
                MATH::Vector2f direction_from_animal_to_group = MATH::Vector2f::Scale(-1.0f, direction_from_group_to_animal);
                // The animals move quicker back to the center of the group for quicker correction of positions.
                float animal_movement_distance_toward_destination_center = 8.0f * group_movement_distance_toward_destination_center;
                MATH::Vector2f movement_of_animal_back_to_group = MATH::Vector2f::Scale(animal_movement_distance_toward_destination_center, direction_from_animal_to_group);
                new_animal_world_position += movement_of_animal_back_to_group;
            }

            // UPDATE THE ANIMAL'S WORLD POSITION.
            animal->Sprite.SetWorldPosition(new_animal_world_position);

            // UPDATE THE ANIMAL'S ANIMATION.
            animal->Sprite.Update(elapsed_time);
        }
    }
}
