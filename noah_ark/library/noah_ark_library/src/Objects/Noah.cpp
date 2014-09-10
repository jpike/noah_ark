#pragma once

#include "Objects/Noah.h"

using namespace OBJECTS;

// STATIC CLASS CONSTANT INITIALIZATION.
const std::string Noah::WALK_FRONT_ANIMATION_NAME = "noah_walk_front";
const std::string Noah::WALK_BACK_ANIMATION_NAME = "noah_walk_back";
const std::string Noah::WALK_LEFT_ANIMATION_NAME = "noah_walk_left";
const std::string Noah::WALK_RIGHT_ANIMATION_NAME = "noah_walk_right";

const float Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND = 64.0f;


Noah::Noah() :
    m_sprite(),
    m_collider()
{}

Noah::~Noah()
{}
        
void Noah::SetSprite(const std::shared_ptr<GRAPHICS::AnimatedSprite>& sprite)
{
    m_sprite = sprite;
}

void Noah::SetCollider(const std::shared_ptr<PHYSICS::COLLISION::BoxCollider>& collider)
{
    m_collider = collider;
}

MATH::FloatRectangle Noah::GetWorldBoundingBox()
{
    return m_collider->GetBoundingBox();
}

MATH::Vector2f Noah::GetWorldPosition() const
{
    return m_collider->GetCenterPosition();
}

void Noah::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_collider->SetCenterPosition(xPositionInPixels, yPositionInPixels);

    /// @todo   Find a better way to synchronize the positions.
    m_sprite->SetWorldPosition(xPositionInPixels, yPositionInPixels);
}

void Noah::MoveUp(const float elapsedTimeInSeconds)
{
    // MOVE THE SPRITE.
    float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
    m_collider->RequestMovement(CORE::Direction::UP, distanceToMoveInPixels);
    //m_sprite->MoveUp(distanceToMoveInPixels);

    // PLAY THE WALKING-UP ANIMATION.
    m_sprite->UseAnimationSequence(WALK_BACK_ANIMATION_NAME);
    m_sprite->Play();
}
        
void Noah::MoveDown(const float elapsedTimeInSeconds)
{
    // MOVE THE SPRITE.
    float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
    m_collider->RequestMovement(CORE::Direction::DOWN, distanceToMoveInPixels);
    //m_sprite->MoveDown(distanceToMoveInPixels);

    // PLAY THE WALKING-DOWN ANIMATION.
    m_sprite->UseAnimationSequence(WALK_FRONT_ANIMATION_NAME);
    m_sprite->Play();
}
        
void Noah::MoveLeft(const float elapsedTimeInSeconds)
{
    // MOVE THE SPRITE.
    float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
    m_collider->RequestMovement(CORE::Direction::LEFT, distanceToMoveInPixels);
    //m_sprite->MoveLeft(distanceToMoveInPixels);

    // PLAY THE WALKING-LEFT ANIMATION.
    m_sprite->UseAnimationSequence(WALK_LEFT_ANIMATION_NAME);
    m_sprite->Play();
}
        
void Noah::MoveRight(const float elapsedTimeInSeconds)
{
    // MOVE THE SPRITE.
    float distanceToMoveInPixels = GetMoveDistance(elapsedTimeInSeconds);
    m_collider->RequestMovement(CORE::Direction::RIGHT, distanceToMoveInPixels);
    //m_sprite->MoveRight(distanceToMoveInPixels);

    // PLAY THE WALKING-RIGHT ANIMATION.
    m_sprite->UseAnimationSequence(WALK_RIGHT_ANIMATION_NAME);
    m_sprite->Play();
}

void Noah::Idle()
{
    // RESET NOAH'S ANIMATION SO THAT NOAH STOPS MOVING.
    m_sprite->ResetAnimation();
}

float Noah::GetMoveDistance(const float elapsedTimeInSeconds) const
{
    float distanceToMoveInPixels = MOVE_SPEED_IN_PIXELS_PER_SECOND * elapsedTimeInSeconds;
    return distanceToMoveInPixels;
}