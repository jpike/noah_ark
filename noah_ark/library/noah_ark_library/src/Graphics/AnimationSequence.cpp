#include <iostream>
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

void AnimationSequence::SetZPosition(const float zPosition)
{
    m_pAnimation->SetZ(zPosition);
}

void AnimationSequence::Play()
{
    // PLAY THE ANIMATION SEQUENCE IF IT ISN'T ALREADY PLAYING.
    bool animationAlreadyPlaying = m_pAnimation->IsPlaying();
    std::cout << "AnimationSequence::Play()" << std::endl;

    if (!animationAlreadyPlaying)
    {
        std::cout << "Playing animation..." << std::endl;
        m_pAnimation->Play();
    }
}