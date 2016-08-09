#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

namespace GRAPHICS
{
    /// Constructor to create an uninitialized animated sprite.
    /// Exact properties are expected to be set later.
    AnimatedSprite::AnimatedSprite() :
        Sprite(),
        AnimationSequences(),
        CurrentAnimationSequenceName()
    {}

    /// Constructor to wrap the provided sprite.
    /// @param[in]  sprite - The sprite being animated.
    AnimatedSprite::AnimatedSprite(const GRAPHICS::Sprite& sprite) :
        Sprite(sprite),
        AnimationSequences(),
        CurrentAnimationSequenceName()
    {}

    /// Constructor to create an animated sprite with a single initial animation sequence.
    /// @param[in]  texture - The texture to use for the sprite.
    /// @param[in]  animation_sequence - The initial animation sequence for the sprite.
    ///     The first frame of the animation sequence defines the initial sub-rectangle
    ///     of the texture to use for the sprite.
    /// @throws std::exception - Thrown if a parameter is invalid.
    AnimatedSprite::AnimatedSprite(
        const std::shared_ptr<GRAPHICS::Texture>& texture,
        const std::shared_ptr<GRAPHICS::AnimationSequence>& animation_sequence) :
        Sprite(),
        AnimationSequences(),
        CurrentAnimationSequenceName()
    {
        // MAKE SURE A TEXTURE WAS PROVIDED.
        bool texture_provided = (nullptr != texture);
        if (!texture_provided)
        {
            throw std::invalid_argument("Must provide texture when creating animated sprite.");
        }

        // MAKE SURE AN ANIMATION SEQUENCE WAS PROVIDED.
        bool animation_sequence_provided = (nullptr != animation_sequence);
        if (!animation_sequence_provided)
        {
            throw std::invalid_argument("Must provide animation sequence when creating animated sprite.");
        }

        // MAKE SURE AT LEAST ONE FRAME OF THE ANIMATION SEQUENCE EXISTS.
        bool animation_sequence_has_frame = (animation_sequence->GetFrameCount() > 0);
        if (!animation_sequence_has_frame)
        {
            throw std::invalid_argument("Animation sequence must have at least one frame when creating animated sprite.");
        }

        // CREATE THE SPRITE WITH THE FIRST TEXTURE RECTANGLE FROM THE ANIMATION SEQUENCE.
        MATH::IntRectangle first_frame_rectangle = animation_sequence->GetCurrentFrame();
        MATH::FloatRectangle texture_subrectangle = MATH::FloatRectangle::FromCenterAndDimensions(
            static_cast<float>(first_frame_rectangle.GetCenterXPosition()),
            static_cast<float>(first_frame_rectangle.GetCenterYPosition()),
            static_cast<float>(first_frame_rectangle.GetWidth()),
            static_cast<float>(first_frame_rectangle.GetHeight()));
        Sprite = GRAPHICS::Sprite(texture, texture_subrectangle);

        // ADD THE INITIAL ANIMATION SEQUENCE TO THIS SPRITE.
        AddAnimationSequence(animation_sequence->AnimationName, animation_sequence);
        UseAnimationSequence(animation_sequence->AnimationName);
    }

    /// Renders the sprite to the provided render target.
    /// @param[in,out]  render_target - The target to render to.
    void AnimatedSprite::Render(sf::RenderTarget& render_target) const
    {
        // RENDER THE SPRITE.
        Sprite.Render(render_target);
    }

    /// Updates the animation based on the elapsed time.
    /// @param[in]  elapsed_time_in_seconds - The elapsed time by which to update the animation.
    void AnimatedSprite::Update(const float elapsed_time_in_seconds)
    {
        // CHECK IF AN ANIMATION IS PLAYING.
        std::shared_ptr<AnimationSequence> current_animation = GetCurrentAnimationSequence();
        bool animation_playing = (nullptr != current_animation) && current_animation->IsPlaying();

        // UPDATE THE ANIMATION IF IT IS PLAYING.
        if (animation_playing)
        {
            // UPDATE THE ANIMATION BASED ON THE PROVIDED TIME.
            sf::Time elapsedTime = sf::seconds(elapsed_time_in_seconds);
            current_animation->Progress(elapsedTime);
        }

        // APPLY ANY ANIMATIONS TO THE SPRITE.
        bool animation_exists = (nullptr != current_animation);
        if (animation_exists)
        {
            // Make sure the correct portion of the sprite's texture for the
            // current frame of animation gets displayed.
            MATH::IntRectangle animation_rectangle = current_animation->GetCurrentFrame();
            Sprite.SetTextureRectangle(animation_rectangle);
        }
    }

    /// Gets the world position of the sprite.
    /// @return The world position of the sprite, in pixels.
    MATH::Vector2f AnimatedSprite::GetWorldPosition() const
    {
        MATH::Vector2f world_position = Sprite.GetWorldPosition();
        return world_position;
    }

    /// Sets the world position of the sprite.
    /// @param[in]  world_position - The position of the sprite in the world.
    void AnimatedSprite::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        Sprite.SetWorldPosition(world_position);
    }

    /// Sets the world position of the sprite.
    /// @param[in]  x_position_in_pixels - The x-coordinate of the sprite in the world.
    /// @param[in]  y_position_in_pixels - The y-coordinate of the sprite in the world.
    void AnimatedSprite::SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels)
    {
        Sprite.SetWorldPosition(x_position_in_pixels, y_position_in_pixels);
    }

    /// Gets the bounding box of the sprite in the world.
    /// @return The bounding box of the sprite.
    MATH::FloatRectangle AnimatedSprite::GetWorldBoundingBox() const
    {
        MATH::FloatRectangle boundingBox = Sprite.GetWorldBoundingBox();
        return boundingBox;
    }

    /// Sets the animation sequence for the sprite with the given name.
    /// If an animation with the specified name already exists for this
    /// sprite, it will be overwritten.
    /// @param[in]  animation_name - The name identifying the animation sequence.
    /// @param[in]  animation_sequence - The animation sequence to map to the provided name.
    /// @throws std::invalid_argument - Thrown if the provided animation sequence is null.
    void AnimatedSprite::AddAnimationSequence(
        const std::string& animation_name,
        const std::shared_ptr<AnimationSequence>& animation_sequence)
    {
        // MAKE SURE THE ANIMATION SEQUENCE ISN'T NULL.
        bool animation_sequence_exists = (nullptr != animation_sequence);
        if (!animation_sequence_exists)
        {
            throw std::invalid_argument("Cannot add null animation sequence to animated sprite.");
        }

        // STORE THE ANIMATION SEQUENCE.
        AnimationSequences[animation_name] = animation_sequence;
    }

    /// Specifies the animation sequence that the sprite should use.
    /// @param[in]  animation_name - The name of the animation sequence to use.
    void AnimatedSprite::UseAnimationSequence(const std::string& animation_name)
    {
        CurrentAnimationSequenceName = animation_name;
    }

    /// Begins playing the current animation sequence, if it isn't already playing.
    void AnimatedSprite::Play()
    {
        // GET THE CURRENT ANIMATION SEQUENCE.
        std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
        bool current_animation_exists = (nullptr != current_animation_sequence);
        if (!current_animation_exists)
        {
            // No animation exists for playing.
            return;
        }

        // CHECK IF AN ANIMATION SEQUENCE IS CURRENTLY PLAYING.
        bool animation_playing = current_animation_sequence->IsPlaying();
        if (animation_playing)
        {
            // Go ahead and return.  We don't want to start re-playing the animation sequence because
            // that will have the effect of restarting it, which will result in an appearance of no animation.
            return;
        }

        // START PLAYING THE ANIMATION.
        current_animation_sequence->Start();
    }

    /// Resets the current animation to its first frame and stops it from playing.
    void AnimatedSprite::ResetAnimation()
    {
        // GET THE CURRENT ANIMATION SEQUENCE.
        std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
        bool current_animation_exists = (nullptr != current_animation_sequence);
        if (current_animation_exists)
        {
            // STOP PLAYING THE CURRENT ANIMATION SEQUENCE.
            bool current_animation_is_playing = current_animation_sequence->IsPlaying();
            if (current_animation_is_playing)
            {
                // RESET THE SPRITE TO THE FIRST FRAME OF THE ANIMATION.
                // This is necessary so that the sprite isn't left with a frame in the middle of the animation sequence.
                current_animation_sequence->Reset();
                MATH::IntRectangle first_frame = current_animation_sequence->GetCurrentFrame();
                Sprite.SetTextureRectangle(first_frame);
            }
        }
    }

    /// Determines if an animation sequence is currently playing for the sprite.
    /// @return True if the sprite is animating; false otherwise.
    bool AnimatedSprite::IsAnimating() const
    {
        // CHECK IF AN ANIMATION SEQUENCE CURRENTLY EXISTS.
        std::shared_ptr<AnimationSequence> current_animation = GetCurrentAnimationSequence();
        bool animation_exists = (nullptr != current_animation);
        if (!animation_exists)
        {
            // The sprite is not animating without an animation sequence.
            return false;
        }

        // RETURN WHETHER OR NOT THE CURRENT ANIMATION IS PLAYING.
        bool is_animating = current_animation->IsPlaying();
        return is_animating;
    }

    /// Returns the animation sequence currently in use.
    /// If the current animation sequence name is not mapped to a valid
    /// sequence, then nullptr may be returned.
    /// @return     The current animation sequence, if it exists.
    std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence() const
    {
        // CHECK IF A CURRENT ANIMATION EXISTS.
        auto current_animation = AnimationSequences.find(CurrentAnimationSequenceName);
        bool current_animation_exists = (current_animation != AnimationSequences.cend());
        if (current_animation_exists)
        {
            // RETURN THE ANIMATION SEQUENCE.
            return current_animation->second;
        }
        else
        {
            // INDICATE THAT NO CURRENT ANIMATION EXISTS.
            return nullptr;
        }
    }

    /// Sets the rotation of the sprite, relative to its origin.
    /// @param[in]  angle_in_degrees - The angle for the rotation.
    void AnimatedSprite::SetRotation(const float angle_in_degrees)
    {
        Sprite.SetRotation(angle_in_degrees);
    }

    /// Sets the scaling of the sprite (in both X and Y dimensions).
    /// 1.0f represents the normal, unscaled value for a dimension.
    /// @param[in]  scale - The scaling amount for the sprite.
    void AnimatedSprite::SetScale(const float scale)
    {
        Sprite.SetScale(scale);
    }

    /// Gets the color of the sprite.
    /// @return The color of the sprite.
    GRAPHICS::Color AnimatedSprite::GetColor() const
    {
        return Sprite.GetColor();
    }

    /// Sets the color of the sprite.  This color modulates the
    /// sprite's texture values and can be used for things like
    /// changing its tint or opacity.
    /// @param[in]  color - The color to set for the sprite.
    void AnimatedSprite::SetColor(const GRAPHICS::Color& color)
    {
        Sprite.SetColor(color);
    }
}
