#pragma once

#include "Objects/Noah.h"

namespace OBJECTS
{
    // STATIC CLASS CONSTANT INITIALIZATION.
    const std::string Noah::WALK_FRONT_ANIMATION_NAME = "noah_walk_front";
    const std::string Noah::WALK_BACK_ANIMATION_NAME = "noah_walk_back";
    const std::string Noah::WALK_LEFT_ANIMATION_NAME = "noah_walk_left";
    const std::string Noah::WALK_RIGHT_ANIMATION_NAME = "noah_walk_right";

    const float Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND = 64.0f;

    Noah::Noah() :
    /// @todo   This facing direction may need to be changed...
    FacingDirection(CORE::Direction::DOWN),
    Sprite(),
    Collider(),
    Axe()
    {}

    Noah::~Noah()
    {}

    void Noah::Update(const float elapsed_time_in_seconds)
    {
        // UPDATE THE AXE.
        bool axe_exists = (nullptr != Axe);
        if (axe_exists)
        {
            Axe->Update(elapsed_time_in_seconds);
        }
    }

    void Noah::SetSprite(const std::shared_ptr<GRAPHICS::AnimatedSprite>& sprite)
    {
        Sprite = sprite;
    }

    void Noah::SetAxe(const std::shared_ptr<OBJECTS::Axe>& axe)
    {
        Axe = axe;
    }

    void Noah::SetCollider(const std::shared_ptr<PHYSICS::COLLISION::BoxCollider>& collider)
    {
        Collider = collider;
    }

    MATH::FloatRectangle Noah::GetWorldBoundingBox() const
    {
        return Collider->GetBoundingBox();
    }

    MATH::Vector2f Noah::GetWorldPosition() const
    {
        return Collider->GetCenterPosition();
    }

    void Noah::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
    {
        Collider->SetCenterPosition(xPositionInPixels, yPositionInPixels);

        /// @todo   Find a better way to synchronize the positions.
        Sprite->SetWorldPosition(xPositionInPixels, yPositionInPixels);
    }

    void Noah::MoveUp(const float elapsedTimeInSeconds)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool axe_is_swinging = (nullptr != Axe) && Axe->IsSwinging();
        if (axe_is_swinging)
        {
            // Noah can't move while the axe is swinging.
            // Movement is prevented to have the axe's position
            // remain attached to Noah.
            return;
        }

        // UPDATE NOAH'S DIRECTION.
        FacingDirection = CORE::Direction::UP;

        // MOVE THE SPRITE.
        float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
        Collider->RequestMovement(CORE::Direction::UP, distanceToMoveInPixels);
        //m_sprite->MoveUp(distanceToMoveInPixels);

        // PLAY THE WALKING-UP ANIMATION.
        Sprite->UseAnimationSequence(WALK_BACK_ANIMATION_NAME);
        Sprite->Play();
    }

    void Noah::MoveDown(const float elapsedTimeInSeconds)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool axe_is_swinging = (nullptr != Axe) && Axe->IsSwinging();
        if (axe_is_swinging)
        {
            // Noah can't move while the axe is swinging.
            // Movement is prevented to have the axe's position
            // remain attached to Noah.
            return;
        }

        // UPDATE NOAH'S DIRECTION.
        FacingDirection = CORE::Direction::DOWN;

        // MOVE THE SPRITE.
        float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
        Collider->RequestMovement(CORE::Direction::DOWN, distanceToMoveInPixels);
        //m_sprite->MoveDown(distanceToMoveInPixels);

        // PLAY THE WALKING-DOWN ANIMATION.
        Sprite->UseAnimationSequence(WALK_FRONT_ANIMATION_NAME);
        Sprite->Play();
    }

    void Noah::MoveLeft(const float elapsedTimeInSeconds)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool axe_is_swinging = (nullptr != Axe) && Axe->IsSwinging();
        if (axe_is_swinging)
        {
            // Noah can't move while the axe is swinging.
            // Movement is prevented to have the axe's position
            // remain attached to Noah.
            return;
        }

        // UPDATE NOAH'S DIRECTION.
        FacingDirection = CORE::Direction::LEFT;

        // MOVE THE SPRITE.
        float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
        Collider->RequestMovement(CORE::Direction::LEFT, distanceToMoveInPixels);
        //m_sprite->MoveLeft(distanceToMoveInPixels);

        // PLAY THE WALKING-LEFT ANIMATION.
        Sprite->UseAnimationSequence(WALK_LEFT_ANIMATION_NAME);
        Sprite->Play();
    }

    void Noah::MoveRight(const float elapsedTimeInSeconds)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool axe_is_swinging = (nullptr != Axe) && Axe->IsSwinging();
        if (axe_is_swinging)
        {
            // Noah can't move while the axe is swinging.
            // Movement is prevented to have the axe's position
            // remain attached to Noah.
            return;
        }

        // UPDATE NOAH'S DIRECTION.
        FacingDirection = CORE::Direction::RIGHT;

        // MOVE THE SPRITE.
        float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
        Collider->RequestMovement(CORE::Direction::RIGHT, distanceToMoveInPixels);
        //m_sprite->MoveRight(distanceToMoveInPixels);

        // PLAY THE WALKING-RIGHT ANIMATION.
        Sprite->UseAnimationSequence(WALK_RIGHT_ANIMATION_NAME);
        Sprite->Play();
    }

    void Noah::Idle()
    {
        // RESET NOAH'S ANIMATION SO THAT NOAH STOPS MOVING.
        Sprite->ResetAnimation();
    }

    std::shared_ptr<EVENTS::AxeSwingEvent> Noah::SwingAxe() const
    {
        // CHECK IF NOAH HAS AN AXE.
        bool axe_exists = (nullptr != Axe);
        if (!axe_exists)
        {
            // An axe can't be swung since it isn't equipped.
            return nullptr;
        }

        // CHECK IF THE AXE IS ALREADY BEING SWUNG.
        // The prevent the axe from being swung too quickly,
        // it should not be possible to swing it again
        // if it is already being swung.
        bool axe_already_swinging = Axe->IsSwinging();
        if (axe_already_swinging)
        {
            // The axe is already being swung,
            // so no new event needs to be created.
            return nullptr;
        }

        // SWING THE AXE IN THE SAME DIRECTION NOAH IS FACING.
        switch (FacingDirection)
        {
            case CORE::Direction::UP:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_height = GetWorldBoundingBox().GetHeight() / 2.0f;
                float axe_half_height = Axe->GetSprite()->GetBoundingBox().GetHeight() / 2.0f;
                //axe_position.Y -= (noah_half_height);
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                axe_position.X += noah_half_width;
                axe_position.X -= 1.5f;
                axe_position.Y += 3;
                Axe->SetWorldPosition(axe_position);
                
                // START SWINGING THE AXE.
                Axe->SwingUp();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Axe);
                return axe_swing;
            }
            case CORE::Direction::DOWN:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The offset from Noah's position is calculated
                // to make the axe appear to come from Noah's hand.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_height = GetWorldBoundingBox().GetHeight() / 2.0f;
                float axe_half_height = Axe->GetSprite()->GetBoundingBox().GetHeight() / 2.0f;
                //axe_position.Y += (noah_half_height);
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                axe_position.X -= noah_half_width;
                axe_position.X += 2;
                axe_position.Y += 2;
                Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Axe->SwingDown();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Axe);
                return axe_swing;
            }
            case CORE::Direction::LEFT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                float axe_half_width = Axe->GetSprite()->GetBoundingBox().GetWidth() / 2.0f;
                //axe_position.X -= (noah_half_width);
                axe_position.X += 1;
                axe_position.Y += 6;
                Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Axe->SwingLeft();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Axe);
                return axe_swing;
            }
            case CORE::Direction::RIGHT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                float axe_half_width = Axe->GetSprite()->GetBoundingBox().GetWidth() / 2.0f;
                //axe_position.X += (noah_half_width);
                axe_position.X -= 1;
                axe_position.Y += 6;
                Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Axe->SwingRight();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Axe);
                return axe_swing;
            }
            default:
                // Don't swing an axe since a valid direction isn't known.
                return nullptr;
        }
    }

    float Noah::GetMoveDistance(const float elapsedTimeInSeconds) const
    {
        float distanceToMoveInPixels = MOVE_SPEED_IN_PIXELS_PER_SECOND * elapsedTimeInSeconds;
        return distanceToMoveInPixels;
    }
}
