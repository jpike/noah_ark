#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Graphics/AnimationSequence.h"
#include "Graphics/IGraphicsComponent.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An animated graphical sprite.
    ///////////////////////////////////////////////////////////
    class AnimatedSprite : public IGraphicsComponent
    {
    public:
        /// @brief      Constructor.  The sprite is visible by default.
        explicit AnimatedSprite();
        
        /// @brief  Destructor.
        virtual ~AnimatedSprite();

        /// @copydoc    IGraphicsComponent::IsVisible() const
        virtual bool IsVisible() const;

        /// @copydoc    IGraphicsComponent::Render()
        virtual void Render();

        /// @copydoc    IGraphicsComponent::Update(const float elapsedTimeInSeconds)
        virtual void Update(const float elapsedTimeInSeconds);

        /// @copydoc    IGraphicsComponent::SetZPosition(const float zPosition)
        virtual void SetZPosition(const float zPosition);

        /// @brief      Gets the world position of the sprite.
        /// @return     The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;

        /// @brief      Sets the world position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief      Gets the bounding box of the sprite in the world.
        /// @return     The bounding box of the sprite.
        hgeRect GetWorldBoundingBox();

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

        /// @brief  Resets the current animation to its first frame and stops it from playing.
        void ResetAnimation();

    private:
        AnimatedSprite(const AnimatedSprite& spriteToCopy); ///< Private to disallow copying.
        AnimatedSprite& operator=(const AnimatedSprite& rhsSprite); ///< Private to disallow copying.
        
        /// @brief      Returns the animation sequence currently in use.
        ///             If the current animation sequence name is not mapped to a valid
        ///             sequence, then nullptr may be returned.
        /// @return     The current animation sequence, if it exists.
        std::shared_ptr<AnimationSequence> GetCurrentAnimationSequence();

        /// @brief  A mapping of animation sequence names to actual animation sequences.
        std::unordered_map< std::string, std::shared_ptr<AnimationSequence> > m_animationSequences;

        std::string m_currentAnimationSequenceName; ///< The name of the animation sequence currently being displayed.

        bool m_visible; ///< Whether or not the sprite is visible.
        MATH::Vector2f m_worldPositionInPixels;  ///< The position of the sprite in the world.
        float m_zPosition;  ///< The z-position of the sprite.
    };
}