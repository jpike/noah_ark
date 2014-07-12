#include <stdexcept>
#include "Graphics/AnimationSequence.h"

using namespace GRAPHICS;

AnimationSequence::AnimationSequence(hgeAnimation* const pAnimation) :
    m_pAnimation(pAnimation)
{
    // REQUIRE THAT THE ANIMATION RESOURCE ISN'T NULL.
    bool animationProvided = (nullptr != m_pAnimation);
    if (!animationProvided)
    {
        throw std::invalid_argument("Animation resource provided for the animation sequence cannot be null.");
    }
}
        
AnimationSequence::~AnimationSequence()
{
    // NULLIFY THE ANIMATION RESOURCE POINTER.
    // It is the responsibility of external code to actually free the memory.
    m_pAnimation = nullptr;
}

void AnimationSequence::Render(const float worldXPositionInPixels, const float worldYPositionInPixels)
{
    m_pAnimation->Render(worldXPositionInPixels, worldYPositionInPixels);
}

void AnimationSequence::Update(const float elapsedTimeInSeconds)
{
    m_pAnimation->Update(elapsedTimeInSeconds);
}

void AnimationSequence::SetZPosition(const float zPosition)
{
    m_pAnimation->SetZ(zPosition);
}

hgeRect AnimationSequence::GetWorldBoundingBox(const float worldXPositionInPixels, const float worldYPositionInPixels)
{
    hgeRect worldBoundingBox(0.0f, 0.0f, 0.0f, 0.0f);
    m_pAnimation->GetBoundingBox(worldXPositionInPixels, worldYPositionInPixels, &worldBoundingBox);
    return worldBoundingBox;
}

void AnimationSequence::Play()
{
    // PLAY THE ANIMATION SEQUENCE IF IT ISN'T ALREADY PLAYING.
    bool animationAlreadyPlaying = m_pAnimation->IsPlaying();

    if (!animationAlreadyPlaying)
    {
        m_pAnimation->Play();
    }
}

void AnimationSequence::Reset()
{
    // Set the animation sequence back to its first frame.
    const int FIRST_FRAME_NUMBER = 0;
    m_pAnimation->SetFrame(FIRST_FRAME_NUMBER);

    // Stop the animation from playing.
    m_pAnimation->Stop();
}