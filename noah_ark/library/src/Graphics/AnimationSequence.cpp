#include "Graphics/AnimationSequence.h"

using namespace GRAPHICS;

AnimationSequence::AnimationSequence(
    const std::string& name,
    const bool isLooping,
    const sf::Time& totalDuration,
    const thor::FrameAnimation& frameAnimation) :
    m_animationName(name),
    m_isLooping(isLooping),
    m_totalDuration(totalDuration),
    m_frameAnimation(frameAnimation)
{}
        
AnimationSequence::~AnimationSequence()
{}

std::string AnimationSequence::GetName() const
{
    return m_animationName;
}

bool AnimationSequence::IsLooping() const
{
    return m_isLooping;
}

sf::Time AnimationSequence::GetDuration() const
{
    return m_totalDuration;
}

thor::FrameAnimation AnimationSequence::GetFrames() const
{
    return m_frameAnimation;
}