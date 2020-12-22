#include "Collision/CollisionDetectionAlgorithms.h"
#include "Objects/FamilyMember.h"

namespace OBJECTS
{
    /// Constructor to initialize a family member of the corresponding type at the given position.
    /// @param[in]  type - The type of the family member.
    /// @param[in]  world_position - The world position of the family member.
    FamilyMember::FamilyMember(const Type type, const MATH::Vector2f& world_position) :
    Identity(type),
    FacingDirection(GAMEPLAY::Direction::DOWN),
    Sprite()
    {
        /// @todo CREATE THE SPRITE BASED ON THE TYPE OF FAMILY MEMBER.
        switch (Identity)
        {
            case NOAH_WIFE:
                break;
            case SHEM:
                break;
            case SHEM_WIFE:
                break;
            case HAM:
                break;
            case HAM_WIFE:
                break;
            case JAPHETH:
                break;
            case JAPHETH_WIFE:
                break;
        }

        // CREATE THE SPRITE FOR THE FAMILY MEMBER.
        /// @todo   Replace this with actual different sprites for different family members.
        const MATH::FloatRectangle TEXTURE_SUB_RECT = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 16, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::NOAH_TEXTURE, TEXTURE_SUB_RECT);
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECT.Center();
        Sprite.CurrentFrameSprite = sprite;
        Sprite.SetWorldPosition(world_position);

        // ADD ANIMATION SEQUENCES.
        const bool IS_LOOPING = true;
        const sf::Time ANIMATION_TOTAL_DURATION = sf::seconds(0.7f);

        const std::vector<MATH::IntRectangle> WALK_FRONT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> WALK_FRONT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_FRONT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_FRONT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(WALK_FRONT_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_BACK_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 16, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 16, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(32, 16, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> WALK_BACK_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_BACK_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_BACK_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(WALK_BACK_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_LEFT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 32, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 32, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> WALK_LEFT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_LEFT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_LEFT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(WALK_LEFT_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_RIGHT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 48, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 48, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> WALK_RIGHT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_RIGHT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_RIGHT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(WALK_RIGHT_ANIMATION);

        // USE THE ANIMATION SEQUENCE CORRESPONDING TO THE DEFAULT FACING DIRECTION SET ABOVE.
        Sprite.UseAnimationSequence(WALK_FRONT_ANIMATION->AnimationName);
    }

    /// Moves the family member within the specified tile map.
    /// The family member is assumed to already be within the tile map.
    /// @param[in]  tile_map - The tile map within which to move the family member.
    /// @param[in,out]  gaming_hardware - Gaming hardware supplying inputs to control movement.
    void FamilyMember::MoveWithin(const MAPS::TileMap& tile_map, HARDWARE::GamingHardware& gaming_hardware)
    {
        // DETERMINE IF THE FAMILY MEMBER SHOULD MOVE AT ALL.
        // The exact frequency at which a family member should move has been carefully tuned.
        constexpr unsigned int MAX_PERCENTAGE = 100;
        constexpr unsigned int MAX_PERCENTAGE_FOR_MOVEMENT = 20;
        unsigned int random_number_for_movement = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan(MAX_PERCENTAGE);
        bool movement_should_occur = (random_number_for_movement <= MAX_PERCENTAGE_FOR_MOVEMENT);
        if (!movement_should_occur)
        {
            Sprite.ResetAnimation();
            return;
        }

        // MOVE THE FAMILY MEMBER.
        MATH::FloatRectangle object_world_bounding_box = Sprite.GetWorldBoundingBox();
        MATH::Vector2f new_world_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
            object_world_bounding_box,
            FacingDirection,
            MOVE_SPEED_IN_PIXELS_PER_SECOND,
            gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
            *tile_map.MapGrid);
        Sprite.SetWorldPosition(new_world_position);

        // RANDOMLY CHANGE THE FAMILY MEMBER'S DIRECTION SOMETIMES.
        // Due to the presence of an INVALID enum value, it's possible that the facing direction might become invalid.
        // This is okay and not handled explicitly as it results in simpler code and also reduces the overall changes
        // in direction.  However, even this isn't quite low enough, so we use another random number to determine
        // if the facing direction could possibly change at all.
        constexpr unsigned int MAX_PERCENTAGE_FOR_CHANGING_FACING_DIRECTION = 25;
        unsigned int random_number_for_changing_direction = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan(MAX_PERCENTAGE);
        bool change_facing_direction = (random_number_for_changing_direction <= MAX_PERCENTAGE_FOR_CHANGING_FACING_DIRECTION);
        if (change_facing_direction)
        {
            FacingDirection = gaming_hardware.RandomNumberGenerator.RandomEnum<GAMEPLAY::Direction>();

            // The animation sequence should also be updated.
            switch (FacingDirection)
            {
                case GAMEPLAY::Direction::UP:
                    Sprite.UseAnimationSequence(WALK_BACK_ANIMATION_NAME);
                    break;
                case GAMEPLAY::Direction::DOWN:
                    Sprite.UseAnimationSequence(WALK_FRONT_ANIMATION_NAME);
                    break;
                case GAMEPLAY::Direction::LEFT:
                    Sprite.UseAnimationSequence(WALK_LEFT_ANIMATION_NAME);
                    break;
                case GAMEPLAY::Direction::RIGHT:
                    Sprite.UseAnimationSequence(WALK_RIGHT_ANIMATION_NAME);
                    break;
            }
        }

        // UPDATE ANIMATIONS FOR MOVEMENT.
        Sprite.Play();
        Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
    }
}
