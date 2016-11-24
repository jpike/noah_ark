#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/System.hpp>
#include "Graphics/AnimationSequence.h"
#include "Graphics/Color.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    /// An animated graphical sprite.  This class enhances
    /// the basic sprite functionality by allowing the
    /// visible rectangle of the sprite's texture to change
    /// over time according to frames in animation sequences.
    /// Multiple animation sequences, each identified by
    /// unique names, may be added to the sprite.
    class AnimatedSprite
    {
    public:
        // CONSTRUCTION.
        explicit AnimatedSprite();
        explicit AnimatedSprite(const GRAPHICS::Sprite& sprite);
        explicit AnimatedSprite(
            const std::shared_ptr<GRAPHICS::Texture>& texture,
            const std::shared_ptr<GRAPHICS::AnimationSequence>& animation_sequence);

        // RENDERING/UPDATING.
        void Render(Screen& screen) const;
        void Update(const sf::Time& elapsed_time);

        // POSITIONING.
        MATH::Vector2f GetWorldPosition() const;
        void SetWorldPosition(const MATH::Vector2f& world_position);
        void SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels);   

        // BOUNDING BOXES.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // ANIMATION.
        void AddAnimationSequence(const std::shared_ptr<AnimationSequence>& animation_sequence);
        void UseAnimationSequence(const std::string& animation_name);
        void Play();
        void ResetAnimation();
        bool IsAnimating() const;
        std::shared_ptr<AnimationSequence> GetCurrentAnimationSequence() const;

        // ROTATION.
        void SetRotation(const float angle_in_degrees);

        // SCALING.
        void SetScale(const float scale);

        // COLOR.
        GRAPHICS::Color GetColor() const;
        void SetColor(const GRAPHICS::Color& color);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        GRAPHICS::Sprite Sprite;   ///< The underlying sprite being animated.

    private:
        // MEMBER VARIABLES.
        /// A mapping of animation sequence names to actual animation sequences.
        /// Allows switching between different animation sequences.
        std::unordered_map< std::string, std::shared_ptr<AnimationSequence> > AnimationSequences;
        std::string CurrentAnimationSequenceName; ///< The name of the animation sequence currently being displayed.
    };
}