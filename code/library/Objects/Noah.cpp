#include "ErrorHandling/NullChecking.h"
#include "Objects/Noah.h"

namespace OBJECTS
{
    // The speed is chosen just based on what has felt right so far.
    const float Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND = 64.0f;
    const std::string Noah::WALK_FRONT_ANIMATION_NAME = "noah_walk_front";
    const std::string Noah::WALK_BACK_ANIMATION_NAME = "noah_walk_back";
    const std::string Noah::WALK_LEFT_ANIMATION_NAME = "noah_walk_left";
    const std::string Noah::WALK_RIGHT_ANIMATION_NAME = "noah_walk_right";

    /// Constructor.
    Noah::Noah() :
    FacingDirection(GAMEPLAY::Direction::INVALID),
    Sprite()
    {
        // CREATE THE SPRITE FOR NOAH.
        const MATH::FloatRectangle TEXTURE_SUB_RECT = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 16, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::NOAH_TEXTURE, TEXTURE_SUB_RECT);
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECT.Center();
        Sprite.CurrentFrameSprite = sprite;

        // ADD NOAH'S ANIMATION SEQUENCES.
        const bool IS_LOOPING = true;
        const sf::Time ANIMATION_TOTAL_DURATION = sf::seconds(0.7f);

        const std::vector<MATH::IntRectangle> WALK_FRONT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_FRONT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_FRONT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_FRONT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(NOAH_WALK_FRONT_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_BACK_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 16, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 16, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(32, 16, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_BACK_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_BACK_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_BACK_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(NOAH_WALK_BACK_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_LEFT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 32, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 32, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_LEFT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_LEFT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_LEFT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(NOAH_WALK_LEFT_ANIMATION);

        const std::vector<MATH::IntRectangle> WALK_RIGHT_ANIMATION_FRAMES =
        {
            MATH::IntRectangle::FromLeftTopAndDimensions(0, 48, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(16, 48, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_RIGHT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            WALK_RIGHT_ANIMATION_NAME,
            IS_LOOPING,
            ANIMATION_TOTAL_DURATION,
            WALK_RIGHT_ANIMATION_FRAMES);
        Sprite.AddAnimationSequence(NOAH_WALK_RIGHT_ANIMATION);

        // SET NOAH TO FACE DOWN BY DEFAULT.
        Sprite.UseAnimationSequence(NOAH_WALK_FRONT_ANIMATION->AnimationName);
        FacingDirection = GAMEPLAY::Direction::DOWN;
    }

    /// Gets the world position of Noah.
    /// @return The world position of Noah.
    MATH::Vector2f Noah::GetWorldPosition() const
    {
        MATH::Vector2f world_position = Sprite.GetWorldPosition();
        return world_position;
    }

    /// Sets the world position of Noah.
    /// @param[in]  world_position - The new world position to set.
    void Noah::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        Sprite.SetWorldPosition(world_position.X, world_position.Y);
    }

    /// Gets the bounding box of Noah in the world.
    /// @return The bounding box of Noah.
    MATH::FloatRectangle Noah::GetWorldBoundingBox() const
    {
        MATH::FloatRectangle bounding_box = Sprite.GetWorldBoundingBox();
        return bounding_box;
    }

    /// Begins the walking process for Noah.  This doesn't actually move Noah
    /// but instead just updates his direction and animation.
    /// @param[in]  direction - The direction Noah should begin walking.
    /// @param[in]  walking_animation_name - The name of the walking animation to start playing for the direction.
    void Noah::BeginWalking(const GAMEPLAY::Direction direction, const std::string& walking_animation_name)
    {
        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
        FacingDirection = direction;

        // PLAY THE WALKING ANIMATION.
        Sprite.UseAnimationSequence(walking_animation_name);
        Sprite.Play();
    }

    /// Causes Noah to begin swinging his axe, if he has one and isn't already swinging it.
    /// @return The event describing the axe swing, if an axe swing is started.
    std::shared_ptr<GAMEPLAY::AxeSwingEvent> Noah::SwingAxe() const
    {
        // CHECK IF THE AXE IS ALREADY BEING SWUNG.
        // The prevent the axe from being swung too quickly,
        // it should not be possible to swing it again
        // if it is already being swung.
        bool axe_already_swinging = Inventory.Axe->IsSwinging();
        if (axe_already_swinging)
        {
            // The axe is already being swung,
            // so no new event needs to be created.
            return nullptr;
        }

        // SWING THE AXE IN THE SAME DIRECTION NOAH IS FACING.
        switch (FacingDirection)
        {
            case GAMEPLAY::Direction::UP:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().Width() / 2.0f;
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X += noah_half_width;
                axe_position.X -= 1.5f;
                axe_position.Y += 3;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingUp();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<GAMEPLAY::AxeSwingEvent> axe_swing = std::make_shared<GAMEPLAY::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case GAMEPLAY::Direction::DOWN:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The offset from Noah's position is calculated
                // to make the axe appear to come from Noah's hand.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().Width() / 2.0f;
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X -= noah_half_width;
                axe_position.X += 2;
                axe_position.Y += 2;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingDown();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<GAMEPLAY::AxeSwingEvent> axe_swing = std::make_shared<GAMEPLAY::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case GAMEPLAY::Direction::LEFT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X += 1;
                axe_position.Y += 6;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingLeft();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<GAMEPLAY::AxeSwingEvent> axe_swing = std::make_shared<GAMEPLAY::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case GAMEPLAY::Direction::RIGHT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X -= 1;
                axe_position.Y += 6;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingRight();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<GAMEPLAY::AxeSwingEvent> axe_swing = std::make_shared<GAMEPLAY::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            default:
                // Don't swing an axe since a valid direction isn't known.
                return nullptr;
        }
    }
}
