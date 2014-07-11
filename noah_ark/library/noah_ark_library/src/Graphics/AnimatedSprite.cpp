#include "Graphics/AnimatedSprite.h"

using namespace GRAPHICS;

AnimatedSprite::AnimatedSprite() :
    m_animationSequences(),
    m_currentAnimationSequenceName(),
    m_visible(true),
    m_worldPositionInPixels(),
    m_zPosition(0.0f)
{}
        
AnimatedSprite::~AnimatedSprite()
{}

bool AnimatedSprite::IsVisible() const
{
    return m_visible;
}

void AnimatedSprite::Render()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationExists = (nullptr != currentAnimationSequence);
    if (currentAnimationExists)
    {
        currentAnimationSequence->SetZPosition(m_zPosition);
        currentAnimationSequence->Render(m_worldPositionInPixels.X, m_worldPositionInPixels.Y);
    }
}

void AnimatedSprite::SetZPosition(const float zPosition)
{
    m_zPosition = zPosition;
}

void AnimatedSprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_worldPositionInPixels.X = xPositionInPixels;
    m_worldPositionInPixels.Y = yPositionInPixels;
}

void AnimatedSprite::MoveUp(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels.Y -= distanceToMoveInPixels;
}

void AnimatedSprite::MoveDown(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels.Y += distanceToMoveInPixels;
}

void AnimatedSprite::MoveLeft(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels.X -= distanceToMoveInPixels;
}

void AnimatedSprite::MoveRight(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels.X += distanceToMoveInPixels;
}

void AnimatedSprite::AddAnimationSequence(
    const std::string& animationName,
    const std::shared_ptr<AnimationSequence>& animationSequence)
{
    m_animationSequences[animationName] = animationSequence;
}

void AnimatedSprite::UseAnimationSequence(const std::string& animationName)
{
    m_currentAnimationSequenceName = animationName;
}

void AnimatedSprite::Play()
{
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationSequenceExists = (nullptr != currentAnimationSequence);
    if (currentAnimationSequenceExists)
    {
        currentAnimationSequence->Play();
    }
}

std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence()
{
    return m_animationSequences[m_currentAnimationSequenceName];
}