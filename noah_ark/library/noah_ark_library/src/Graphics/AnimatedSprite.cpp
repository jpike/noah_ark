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
        // RENDER IT IN THE WORLD.
        // The z-position is set in case the currently used animation sequence has changed
        // or never been used since last setting the z-position.
        currentAnimationSequence->SetZPosition(m_zPosition);
        currentAnimationSequence->Render(m_worldPositionInPixels.X, m_worldPositionInPixels.Y);
    }
}

void AnimatedSprite::Update(const float elapsedTimeInSeconds)
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationExists = (nullptr != currentAnimationSequence);
    if (currentAnimationExists)
    {
        // UPDATE IT BASED ON THE PROVIDED TIME.
        currentAnimationSequence->Update(elapsedTimeInSeconds);
    }
}

void AnimatedSprite::SetZPosition(const float zPosition)
{
    m_zPosition = zPosition;
}

MATH::Vector2f AnimatedSprite::GetWorldPosition() const
{
    return m_worldPositionInPixels;
}

void AnimatedSprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_worldPositionInPixels.X = xPositionInPixels;
    m_worldPositionInPixels.Y = yPositionInPixels;
}

hgeRect AnimatedSprite::GetWorldBoundingBox()
{
    // SET A DEFAULT BOUNDING BOX.
    // This default box should be harmless in the event
    // that no animation sequence is currently being used.
    hgeRect boundingBox(0.0f, 0.0f, 0.0f, 0.0f);

    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationExists = (nullptr != currentAnimationSequence);
    if (currentAnimationExists)
    {
        boundingBox = currentAnimationSequence->GetWorldBoundingBox(m_worldPositionInPixels.X, m_worldPositionInPixels.Y);
    }

    return boundingBox;
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

void AnimatedSprite::ResetAnimation()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationExists = (nullptr != currentAnimationSequence);
    if (currentAnimationExists)
    {
        // RESET THE CURRENT ANIMATION SEQUENCE.
        currentAnimationSequence->Reset();
    }
}

std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence()
{
    return m_animationSequences[m_currentAnimationSequenceName];
}