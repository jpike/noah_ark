#include <iostream>
#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

using namespace GRAPHICS;

AnimatedSprite::AnimatedSprite(const std::shared_ptr<sf::Sprite>& sprite) :
    m_sprite(sprite),
    m_animationSequences(),
    m_currentAnimationSequenceName(),
    m_visible(true),
    m_worldPositionInPixels(new MATH::Vector2f(0.0f, 0.0f))
{
    // REQUIRE THAT THE SPRITE ISN'T NULL.
    bool spriteProvided = (nullptr != sprite);
    if (!spriteProvided)
    {
        throw std::invalid_argument("Sprite provided for the animated sprite cannot be null.");
    }

    // SET THE WORLD POSITION TO MATCH THAT OF THE PROVIDED SPRITE.
    // It was not set in the initialization list because the sprite pointer may have been null.
    const sf::Vector2f& spritePosition = m_sprite->getPosition();
    m_worldPositionInPixels->X = spritePosition.x;
    m_worldPositionInPixels->Y = spritePosition.y;
}
        
AnimatedSprite::~AnimatedSprite()
{}

bool AnimatedSprite::IsVisible() const
{
    return m_visible;
}

void AnimatedSprite::SetVisible(const bool is_visible)
{
    m_visible = is_visible;
}

void AnimatedSprite::Render(sf::RenderTarget& renderTarget)
{
    // MAKE SURE THE SPRITE IS CORRECTLY POSITIONED IN THE WORLD.
    SynchronizeSpriteResourcePosition();

    // APPLY ANY ANIMATIONS TO THE SPRITE.
    std::shared_ptr<AnimationSequence> current_animation = GetCurrentAnimationSequence();
    bool animation_exists = (nullptr != current_animation);
    if (animation_exists)
    {
        sf::IntRect animation_rectangle = current_animation->GetCurrentFrame();
        m_sprite->setTextureRect(animation_rectangle);
    }

    // RENDER THE SPRITE.
    renderTarget.draw(*m_sprite);
}

void AnimatedSprite::Update(const float elapsedTimeInSeconds)
{
    // CHECK IF AN ANIMATION IS PLAYING.
    std::shared_ptr<AnimationSequence> current_animation = GetCurrentAnimationSequence();
    bool animation_playing = (nullptr != current_animation) && current_animation->IsPlaying();

    // UPDATE THE ANIMATION IF IT IS PLAYING.
    if (animation_playing)
    {
        // UPDATE THE ANIMATION BASED ON THE PROVIDED TIME.
        sf::Time elapsedTime = sf::seconds(elapsedTimeInSeconds);
        current_animation->Progress(elapsedTime);
    }
}

void AnimatedSprite::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
{
    // MAKE SURE THE PROVIDED POSITION COMPONENT EXISTS.
    bool newPositionComponentExists = (nullptr != positionComponent);
    if (!newPositionComponentExists)
    {
        throw std::invalid_argument("Position component provided for the animated sprite cannot be null.");
    }

    // UPDATE THE SPRITE TO USE THE NEW POSITION COMPONENT.
    m_worldPositionInPixels = positionComponent;
    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

MATH::Vector2f AnimatedSprite::GetWorldPosition() const
{
    return (*m_worldPositionInPixels);
}

void AnimatedSprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_worldPositionInPixels->X = xPositionInPixels;
    m_worldPositionInPixels->Y = yPositionInPixels;

    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

MATH::FloatRectangle AnimatedSprite::GetWorldBoundingBox()
{
    // GET THE SPRITE'S BOUNDING BOX.
    MATH::FloatRectangle boundingBox(m_sprite->getGlobalBounds());

    return boundingBox;
}

void AnimatedSprite::MoveUp(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels->Y -= distanceToMoveInPixels;

    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

void AnimatedSprite::MoveDown(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels->Y += distanceToMoveInPixels;

    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

void AnimatedSprite::MoveLeft(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels->X -= distanceToMoveInPixels;

    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

void AnimatedSprite::MoveRight(const float distanceToMoveInPixels)
{
    m_worldPositionInPixels->X += distanceToMoveInPixels;

    // The sprite resource needs to be kept in-sync.
    SynchronizeSpriteResourcePosition();
}

void AnimatedSprite::SetRotation(const float degrees)
{
    m_sprite->setRotation(degrees);
}

void AnimatedSprite::AddAnimationSequence(
    const std::string& animationName,
    const std::shared_ptr<AnimationSequence>& animationSequence)
{
    // Store the animation sequence so that we have full access to its details later.
    m_animationSequences[animationName] = animationSequence;
}

void AnimatedSprite::UseAnimationSequence(const std::string& animationName)
{
    m_currentAnimationSequenceName = animationName;
}

void AnimatedSprite::Play()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
    bool current_animation_exists = (nullptr != current_animation_sequence);
    if (current_animation_exists)
    {
        // CHECK IF AN ANIMATION SEQUENCE IS CURRENTLY PLAYING.
        if (current_animation_sequence->IsPlaying())
        {
            // Go ahead and return.  We don't want to start re-playing the animation sequence because
            // that will have the effect of restarting it, which will result in an appearance of no animation.
            return;
        }

        current_animation_sequence->Start();
    }
}

bool AnimatedSprite::IsAnimating()
{
    // CHECK IF A CURRENT ANIMATION SEQUENCE EXISTS.
    std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
    bool current_animation_exists = (nullptr != current_animation_sequence);
    if (current_animation_exists)
    {
        bool animation_playing = current_animation_sequence->IsPlaying();
        return animation_playing;
    }
    else
    {
        // No animation sequence is playing.
        return false;
    }
}

void AnimatedSprite::ResetAnimation()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
    bool current_animation_exists = (nullptr != current_animation_sequence);
    if (current_animation_exists)
    {
        // STOP PLAYING THE CURRENT ANIMATION SEQUENCE.
        if (current_animation_sequence->IsPlaying())
        {
            // RESET THE SPRITE TO THE FIRST FRAME OF THE ANIMATION.
            // This is necessary so that the sprite isn't left with a frame in the middle of the animation sequence.
            current_animation_sequence->Reset();
            sf::IntRect first_frame = current_animation_sequence->GetCurrentFrame();
            m_sprite->setTextureRect(first_frame);
        }
    }
}

void AnimatedSprite::SynchronizeSpriteResourcePosition()
{
    m_sprite->setPosition(m_worldPositionInPixels->X, m_worldPositionInPixels->Y);
}

std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence()
{
    return m_animationSequences[m_currentAnimationSequenceName];
}