#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

using namespace GRAPHICS;

AnimatedSprite::AnimatedSprite(const std::shared_ptr<sf::Sprite>& sprite) :
    m_sprite(sprite),
    m_animator(),
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

    // SET THE SPRITE RESOURCE TO BE CENTERED IN THE MIDDLE OF ITS TEXTURE RECTANGLE.
    // All sprites within this game are expected to be positioned based on their centers.
    sf::FloatRect spriteBounds = m_sprite->getLocalBounds();
    const float spriteTextureCenterX = spriteBounds.width / 2.0f;
    const float spriteTextureCenterY = spriteBounds.height / 2.0f;
    m_sprite->setOrigin(spriteTextureCenterX, spriteTextureCenterY);
}
        
AnimatedSprite::~AnimatedSprite()
{}

bool AnimatedSprite::IsVisible() const
{
    return m_visible;
}

void AnimatedSprite::Render(sf::RenderTarget& renderTarget)
{
    // MAKE SURE THE SPRITE IS CORRECTLY POSITIONED IN THE WORLD.
    SynchronizeSpriteResourcePosition();

    // APPLY ANY ANIMATIONS TO THE SPRITE.
    m_animator.animate(*m_sprite);

    // RENDER THE SPRITE.
    renderTarget.draw(*m_sprite);
}

void AnimatedSprite::Update(const float elapsedTimeInSeconds)
{
    // UPDATE THE ANIMATOR BASED ON THE PROVIDED TIME.
    sf::Time elapsedTime = sf::seconds(elapsedTimeInSeconds);
    m_animator.update(elapsedTime);
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

void AnimatedSprite::AddAnimationSequence(
    const std::string& animationName,
    const std::shared_ptr<AnimationSequence>& animationSequence)
{
    // Store the animation sequence so that we have full access to its details later.
    m_animationSequences[animationName] = animationSequence;

    // Add the animation sequence to the animator.
    m_animator.addAnimation(
        animationSequence->GetName(),
        animationSequence->GetFrames(),
        animationSequence->GetDuration());
}

void AnimatedSprite::UseAnimationSequence(const std::string& animationName)
{
    m_currentAnimationSequenceName = animationName;
}

void AnimatedSprite::Play()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationSequenceExists = (nullptr != currentAnimationSequence);
    if (currentAnimationSequenceExists)
    {
        // CHECK IF AN ANIMATION SEQUENCE IS CURRENTLY PLAYING.
        if (m_animator.isPlayingAnimation())
        {
            // CHECK IF THE SAME ANIMATION SEQUENCE IS ALREADY PLAYING.
            std::string playingAnimationSequence = m_animator.getPlayingAnimation();
            bool sameAnimationSequencePlaying = (currentAnimationSequence->GetName() == playingAnimationSequence);
            if (sameAnimationSequencePlaying)
            {
                // Go ahead and return.  We don't want to start re-playing the animation sequence because
                // that will have the effect of restarting it, which will result in an appearance of no animation.
                return;
            }
        }

        m_animator.playAnimation(
            currentAnimationSequence->GetName(),
            currentAnimationSequence->IsLooping());
    }
}

void AnimatedSprite::ResetAnimation()
{
    // GET THE CURRENT ANIMATION SEQUENCE.
    std::shared_ptr<AnimationSequence> currentAnimationSequence = GetCurrentAnimationSequence();
    bool currentAnimationExists = (nullptr != currentAnimationSequence);
    if (currentAnimationExists)
    {
        // STOP PLAYING THE CURRENT ANIMATION SEQUENCE.
        if (m_animator.isPlayingAnimation())
        {
            // RESET THE SPRITE TO THE FIRST FRAME OF THE ANIMATION.
            // This is necessary so that the sprite isn't left with a frame in the middle of the animation sequence.
            // The resetting is done by re-starting the animation, updating it with zero seconds to the force it
            // to the ver beginning, and finally animating the actual sprite to use the animation with zero progress.
            m_animator.playAnimation(currentAnimationSequence->GetName(), currentAnimationSequence->IsLooping());
            const sf::Time BEGINNING_OF_ANIMATION = sf::seconds(0.0f);
            m_animator.update(BEGINNING_OF_ANIMATION);
            m_animator.animate(*m_sprite);
        }
        m_animator.stopAnimation();
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