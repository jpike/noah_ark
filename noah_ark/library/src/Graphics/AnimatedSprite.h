#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Graphics/AnimationSequence.h"
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
        // CONSTRUCTION/DESTRUCTION.
        /// @todo   Temporary constructor to get things compiling...should be removed/changed later.
        explicit AnimatedSprite() {}
        /// Constructor to wrap the provided sprite.
        /// @param[in]  sprite - The sprite being animated.  Must not be null.
        /// @throws std::invalid_argument - Thrown if the provided sprite is null.
        explicit AnimatedSprite(const std::shared_ptr<GRAPHICS::Sprite>& sprite);
        /// Destructor.
        virtual ~AnimatedSprite();

        // INTERFACE IMPLEMENTATION - IGraphicsComponent.
        /// @copydoc    IGraphicsComponent::IsVisible
        virtual bool IsVisible() const;
        /// @copydoc    IGraphicsComponent::SetVisible
        virtual void SetVisible(const bool is_visible);
        /// @copydoc    IGraphicsComponent::Render
        virtual void Render(sf::RenderTarget& render_target);
        /// @copydoc    IGraphicsComponent::Update
        virtual void Update(const float elapsed_time_in_seconds);

        // INTERFACE IMPLEMENTATION - ITransformable.
        /// @copydoc    ITransformable::SetPositionComponent
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& position_component);

        // POSITIONING.
        /// Gets the world position of the sprite.
        /// @return The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;
        /// Sets the world position of the sprite.
        /// @param[in]  x_position_in_pixels - The x-coordinate of the sprite in the world.
        /// @param[in]  y_position_in_pixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels);   
        /// Moves the sprite's world position up the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveUp(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position down the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveDown(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position left the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveLeft(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position right the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveRight(const float distance_to_move_in_pixels);

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

    private:
        // PRIVATE METHODS.
        /// Returns the animation sequence currently in use.
        /// If the current animation sequence name is not mapped to a valid
        /// sequence, then nullptr may be returned.
        /// @return     The current animation sequence, if it exists.
        std::shared_ptr<AnimationSequence> GetCurrentAnimationSequence();

        // MEMBER VARIABLES.
        std::shared_ptr<GRAPHICS::Sprite> Sprite;   ///< The underlying sprite being animated.
        /// A mapping of animation sequence names to actual animation sequences.
        /// Allows switching between different animation sequences.
        std::unordered_map< std::string, std::shared_ptr<AnimationSequence> > AnimationSequences;
        std::string CurrentAnimationSequenceName; ///< The name of the animation sequence currently being displayed.
    };
}