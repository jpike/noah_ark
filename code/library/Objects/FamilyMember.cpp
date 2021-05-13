#include "Collision/CollisionDetectionAlgorithms.h"
#include "Objects/FamilyMember.h"

namespace OBJECTS
{
    /// Constructor to initialize a family member of the corresponding type at the given position.
    /// @param[in]  type - The type of the family member.
    /// @param[in]  world_position - The world position of the family member.
    /// @param[in]  map_grid - The map grid the family member is in.
    FamilyMember::FamilyMember(const Type type, const MATH::Vector2f& world_position, const MAPS::MultiTileMapGrid* const map_grid) :
    Identity(type),
    FacingDirection(GAMEPLAY::Direction::DOWN),
    Sprite(),
    MapGrid(map_grid)
    {
        // CREATE THE SPRITE BASED ON THE TYPE OF FAMILY MEMBER.
        RESOURCES::AssetId texture_id = RESOURCES::AssetId::NOAH_TEXTURE;
        switch (Identity)
        {
            case NOAH_WIFE:
                texture_id = RESOURCES::AssetId::NOAH_WIFE_TEXTURE;
                break;
            case SHEM:
                texture_id = RESOURCES::AssetId::SHEM_TEXTURE;
                break;
            case SHEM_WIFE:
                texture_id = RESOURCES::AssetId::SHEM_WIFE_TEXTURE;
                break;
            case HAM:
                texture_id = RESOURCES::AssetId::HAM_TEXTURE;
                break;
            case HAM_WIFE:
                texture_id = RESOURCES::AssetId::HAM_WIFE_TEXTURE;
                break;
            case JAPHETH:
                texture_id = RESOURCES::AssetId::JAPHETH_TEXTURE;
                break;
            case JAPHETH_WIFE:
                texture_id = RESOURCES::AssetId::JAPHETH_WIFE_TEXTURE;
                break;
        }

        // CREATE THE SPRITE FOR THE FAMILY MEMBER.
        const MATH::FloatRectangle TEXTURE_SUB_RECT = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 16, 16);
        GRAPHICS::Sprite sprite(texture_id, TEXTURE_SUB_RECT);
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

    /// Begins the walking process for the family member.  This doesn't actually move the family member
    /// but instead just updates his direction and animation.
    /// @param[in]  direction - The direction the family member should begin walking.
    /// @param[in]  walking_animation_name - The name of the walking animation to start playing for the direction.
    void FamilyMember::BeginWalking(const GAMEPLAY::Direction direction, const std::string& walking_animation_name)
    {
        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
        FacingDirection = direction;

        // PLAY THE WALKING ANIMATION.
        Sprite.UseAnimationSequence(walking_animation_name);
        Sprite.Play();
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
        constexpr unsigned int MAX_PERCENTAGE_FOR_CHANGING_FACING_DIRECTION = 20;
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
