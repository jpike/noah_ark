#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Graphics/AnimationSequence.h"
#include "Graphics/Color.h"
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
        /// Constructor to create an uninitialized animated sprite.
        /// Exact properties are expected to be set later.
        explicit AnimatedSprite();
        /// Constructor to wrap the provided sprite.
        /// @param[in]  sprite - The sprite being animated.
        explicit AnimatedSprite(const GRAPHICS::Sprite& sprite);
        /// Constructor to create an animated sprite with a single initial animation sequence.
        /// @param[in]  texture - The texture to use for the sprite.
        /// @param[in]  animation_sequence - The initial animation sequence for the sprite.
        ///     The first frame of the animation sequence defines the initial sub-rectangle
        ///     of the texture to use for the sprite.
        /// @throws std::exception - Thrown if a parameter is invalid.
        explicit AnimatedSprite(
            const std::shared_ptr<GRAPHICS::Texture>& texture,
            const std::shared_ptr<GRAPHICS::AnimationSequence>& animation_sequence);

        // INTERFACE IMPLEMENTATION - IGraphicsComponent.
        /// @copydoc    IGraphicsComponent::Render
        virtual void Render(sf::RenderTarget& render_target) const;
        /// @copydoc    IGraphicsComponent::Update
        virtual void Update(const float elapsed_time_in_seconds);

        // POSITIONING.
        /// Gets the world position of the sprite.
        /// @return The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;
        /// Sets the world position of the sprite.
        /// @param[in]  world_position - The position of the sprite in the world.
        void SetWorldPosition(const MATH::Vector2f& world_position);
        /// Sets the world position of the sprite.
        /// @param[in]  x_position_in_pixels - The x-coordinate of the sprite in the world.
        /// @param[in]  y_position_in_pixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels);   

        // BOUNDING BOXES.
        /// Gets the bounding box of the sprite in the world.
        /// @return The bounding box of the sprite.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // ANIMATION.
        /// Sets the animation sequence for the sprite with the given name.
        /// If an animation with the specified name already exists for this
        /// sprite, it will be overwritten.
        /// @param[in]  animation_name - The name identifying the animation sequence.
        /// @param[in]  animation_sequence - The animation sequence to map to the provided name.
        /// @throws std::invalid_argument - Thrown if the provided animation sequence is null.
        void AddAnimationSequence(
            const std::string& animation_name,
            const std::shared_ptr<AnimationSequence>& animation_sequence);
        /// Specifies the animation sequence that the sprite should use.
        /// @param[in]  animation_name - The name of the animation sequence to use.
        void UseAnimationSequence(const std::string& animation_name);
        /// Begins playing the current animation sequence, if it isn't already playing.
        void Play();
        /// Resets the current animation to its first frame and stops it from playing.
        void ResetAnimation();
        /// Determines if an animation sequence is currently playing for the sprite.
        /// @return True if the sprite is animating; false otherwise.
        bool IsAnimating() const;
        /// Returns the animation sequence currently in use.
        /// If the current animation sequence name is not mapped to a valid
        /// sequence, then nullptr may be returned.
        /// @return     The current animation sequence, if it exists.
        std::shared_ptr<AnimationSequence> GetCurrentAnimationSequence() const;

        // ROTATION.
        /// Sets the rotation of the sprite, relative to its origin.
        /// @param[in]  angle_in_degrees - The angle for the rotation.
        void SetRotation(const float angle_in_degrees);

        // SCALING.
        /// Sets the scaling of the sprite (in both X and Y dimensions).
        /// 1.0f represents the normal, unscaled value for a dimension.
        /// @param[in]  scale - The scaling amount for the sprite.
        void SetScale(const float scale);

        // COLOR.
        /// Gets the color of the sprite.
        /// @return The color of the sprite.
        GRAPHICS::Color GetColor() const;
        /// Sets the color of the sprite.  This color modulates the
        /// sprite's texture values and can be used for things like
        /// changing its tint or opacity.
        /// @param[in]  color - The color to set for the sprite.
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