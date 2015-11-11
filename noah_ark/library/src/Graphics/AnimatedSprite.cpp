#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

namespace GRAPHICS
{
    AnimatedSprite::AnimatedSprite() :
    Sprite(),
    AnimationSequences(),
    CurrentAnimationSequenceName()
    {}

    AnimatedSprite::AnimatedSprite(const GRAPHICS::Sprite& sprite) :
    Sprite(sprite),
    AnimationSequences(),
    CurrentAnimationSequenceName()
    {}

    void AnimatedSprite::Render(sf::RenderTarget& render_target) const
    {
        // RENDER THE SPRITE.
        Sprite.Render(render_target);
    }

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

    MATH::Vector2f AnimatedSprite::GetWorldPosition() const
    {
        MATH::Vector2f world_position = Sprite.GetWorldPosition();
        return world_position;
    }

    void AnimatedSprite::SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels)
    {
        Sprite.SetWorldPosition(x_position_in_pixels, y_position_in_pixels);
    }

    MATH::FloatRectangle AnimatedSprite::GetWorldBoundingBox() const
    {
        MATH::FloatRectangle boundingBox = Sprite.GetWorldBoundingBox();
        return boundingBox;
    }

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

    void AnimatedSprite::UseAnimationSequence(const std::string& animation_name)
    {
        CurrentAnimationSequenceName = animation_name;
    }

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

    std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence()
    {
        return AnimationSequences[CurrentAnimationSequenceName];
    }
}
