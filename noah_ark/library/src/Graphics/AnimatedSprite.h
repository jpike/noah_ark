#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Graphics/AnimationSequence.h"
#include "Graphics/IGraphicsComponent.h"
#include "Math/FloatRectangle.h"
#include "Math/ITransformable.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An animated graphical sprite.  This class enhances
    ///         the basic sprite functionality by allowing the
    ///         visible rectangle of the sprite's texture to change
    ///         over time according to frames in animation sequences.
    ///         Multiple animation sequences, each identified by
    ///         unique names, may be added to the sprite.
    ///////////////////////////////////////////////////////////
    class AnimatedSprite : public IGraphicsComponent, public MATH::ITransformable
    {
    public:
        /// @brief      Constructor.  The sprite is visible by default.
        /// @param[in]  sprite - The SFML sprite resource being animated.  Must not be null.
        ///             The sprite's origin will be set to the center of its texture rectangle,
        ///             and its position will become the world position of this animated sprite.
        /// @throws     std::invalid_argument - Thrown if the provided sprite resource is null.
        explicit AnimatedSprite(const std::shared_ptr<sf::Sprite>& sprite);
        
        /// @brief  Destructor.
        virtual ~AnimatedSprite();

        /// @copydoc    IGraphicsComponent::IsVisible() const
        virtual bool IsVisible() const;

        /// Sets whether the sprite is visible.
        /// @param[in]  is_visible - True to make the sprite visible; false to hide it.
        void SetVisible(const bool is_visible);

        /// @copydoc    IGraphicsComponent::Render(sf::RenderTarget& renderTarget)
        virtual void Render(sf::RenderTarget& renderTarget);

        /// @copydoc    IGraphicsComponent::Update(const float elapsedTimeInSeconds)
        virtual void Update(const float elapsedTimeInSeconds);

        /// @copydoc    ITransformable::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent);

        /// @brief      Gets the world position of the sprite.
        /// @return     The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;

        /// @brief      Sets the world position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief      Gets the bounding box of the sprite in the world.
        /// @return     The bounding box of the sprite.
        MATH::FloatRectangle GetWorldBoundingBox();

        /// @brief      Moves the sprite's world position up the specified distance.
        /// @param[in]  distanceToMoveInPixels - The distance to move, in pixels.
        void MoveUp(const float distanceToMoveInPixels);
        /// @brief      Moves the sprite's world position down the specified distance.
        /// @param[in]  distanceToMoveInPixels - The distance to move, in pixels.
        void MoveDown(const float distanceToMoveInPixels);
        /// @brief      Moves the sprite's world position left the specified distance.
        /// @param[in]  distanceToMoveInPixels - The distance to move, in pixels.
        void MoveLeft(const float distanceToMoveInPixels);
        /// @brief      Moves the sprite's world position right the specified distance.
        /// @param[in]  distanceToMoveInPixels - The distance to move, in pixels.
        void MoveRight(const float distanceToMoveInPixels);

        /// Sets the rotation of the sprite, relative to its local origin.
        /// @param[in]  degrees - The amount of degrees for the rotation.
        void SetRotation(const float degrees);

        /// @brief      Sets the animation sequence for the sprite with the given name.
        ///             If an animation with the specified name already exists for this
        ///             sprite, it will be overwritten.
        /// @param[in]  animationName - The name identifying the animation sequence.
        /// @param[in]  animationSequence - The animation sequence to map to the provided name.
        void AddAnimationSequence(
            const std::string& animationName,
            const std::shared_ptr<AnimationSequence>& animationSequence);

        /// @brief      Specifies the animation sequence that the sprite should use.
        /// @param[in]  animationName - The name of the animation sequence to use.
        void UseAnimationSequence(const std::string& animationName);

        /// @brief  Begins playing the current animation sequence, if it isn't already playing.
        void Play();

        /// Indicates if the sprite is currently playing an animation sequence.
        /// @return True if the sprite is playing an animation sequence; false otherwise.
        bool IsAnimating();

        /// @brief  Resets the current animation to its first frame and stops it from playing.
        void ResetAnimation();

        /// @brief      Returns the animation sequence currently in use.
        ///             If the current animation sequence name is not mapped to a valid
        ///             sequence, then nullptr may be returned.
        /// @return     The current animation sequence, if it exists.
        /// @todo   Return this to its previous position.  It probably shouldn't be exposed.
        std::shared_ptr<AnimationSequence> GetCurrentAnimationSequence();

    private:
        AnimatedSprite(const AnimatedSprite& spriteToCopy); ///< Private to disallow copying.
        AnimatedSprite& operator=(const AnimatedSprite& rhsSprite); ///< Private to disallow copying.
        
        /// @brief  Synchronizes the sprite resource's position with the overall world
        ///         position of this animated sprite.  This is necessary because the 
        ///         world position is maintained separately from the position in the
        ///         sprite resource, so the sprite resource needs to be updated anytime
        ///         the main world position changes.
        void SynchronizeSpriteResourcePosition();

        std::shared_ptr<sf::Sprite> m_sprite;   ///< The underlying sprite being animated.

        /// @brief  A mapping of animation sequence names to actual animation sequences.
        ///         Animation sequences can store additional data beyond that stored in
        ///         Thor animators, which is why they are stored in this mapping.
        std::unordered_map< std::string, std::shared_ptr<AnimationSequence> > m_animationSequences;

        std::string m_currentAnimationSequenceName; ///< The name of the animation sequence currently being displayed.

        bool m_visible; ///< Whether or not the sprite is visible.
        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels;  ///< The position of the sprite in the world.
    };
}