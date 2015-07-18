#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

namespace GRAPHICS
{
    AnimatedSprite::AnimatedSprite(const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    Sprite(sprite),
    AnimationSequences(),
    CurrentAnimationSequenceName()
    {
        // REQUIRE THAT THE SPRITE ISN'T NULL.
        bool sprite_provided = (nullptr != sprite);
        if (!sprite_provided)
        {
            throw std::invalid_argument("Sprite provided for the animated sprite cannot be null.");
        }
    }

    AnimatedSprite::~AnimatedSprite()
    {}

    bool AnimatedSprite::IsVisible() const
    {
        bool is_visible = Sprite->IsVisible();
        return is_visible;
    }

    void AnimatedSprite::SetVisible(const bool is_visible)
    {
        Sprite->SetVisible(is_visible);
    }

    void AnimatedSprite::Render(sf::RenderTarget& render_target)
    {
        // APPLY ANY ANIMATIONS TO THE SPRITE.
        std::shared_ptr<AnimationSequence> current_animation = GetCurrentAnimationSequence();
        bool animation_exists = (nullptr != current_animation);
        if (animation_exists)
        {
            // Make sure the correct portion of the sprite's texture for the
            // current frame of animation gets displayed.
            sf::IntRect animation_rectangle = current_animation->GetCurrentFrame();
            Sprite->SetTextureRectangle(animation_rectangle);
        }

        // RENDER THE SPRITE.
        Sprite->Render(render_target);
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
    }

    void AnimatedSprite::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& position_component)
    {
        // MAKE SURE THE PROVIDED POSITION COMPONENT EXISTS.
        bool new_position_component_exists = (nullptr != position_component);
        if (!new_position_component_exists)
        {
            throw std::invalid_argument("Position component provided for the animated sprite cannot be null.");
        }

        // UPDATE THE SPRITE TO USE THE NEW POSITION COMPONENT.
        Sprite->SetPositionComponent(position_component);
    }

    MATH::Vector2f AnimatedSprite::GetWorldPosition() const
    {
        MATH::Vector2f world_position = Sprite->GetWorldPosition();
        return world_position;
    }

    void AnimatedSprite::SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels)
    {
        Sprite->SetWorldPosition(x_position_in_pixels, y_position_in_pixels);
    }

    MATH::FloatRectangle AnimatedSprite::GetWorldBoundingBox()
    {
        MATH::FloatRectangle boundingBox = Sprite->GetBoundingBox();
        return boundingBox;
    }

    void AnimatedSprite::MoveUp(const float distance_to_move_in_pixels)
    {
        Sprite->MoveUp(distance_to_move_in_pixels);
    }

    void AnimatedSprite::MoveDown(const float distance_to_move_in_pixels)
    {
        Sprite->MoveDown(distance_to_move_in_pixels);
    }

    void AnimatedSprite::MoveLeft(const float distance_to_move_in_pixels)
    {
        Sprite->MoveLeft(distance_to_move_in_pixels);
    }

    void AnimatedSprite::MoveRight(const float distance_to_move_in_pixels)
    {
        Sprite->MoveRight(distance_to_move_in_pixels);
    }

    void AnimatedSprite::SetRotation(const float degrees)
    {
        Sprite->SetRotation(degrees);
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

    bool AnimatedSprite::IsAnimating()
    {
        // CHECK IF A CURRENT ANIMATION SEQUENCE EXISTS.
        std::shared_ptr<AnimationSequence> current_animation_sequence = GetCurrentAnimationSequence();
        bool current_animation_exists = (nullptr != current_animation_sequence);
        if (current_animation_exists)
        {
            // Return if the animation is playing.
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
            bool current_animation_is_playing = current_animation_sequence->IsPlaying();
            if (current_animation_is_playing)
            {
                // RESET THE SPRITE TO THE FIRST FRAME OF THE ANIMATION.
                // This is necessary so that the sprite isn't left with a frame in the middle of the animation sequence.
                current_animation_sequence->Reset();
                sf::IntRect first_frame = current_animation_sequence->GetCurrentFrame();
                Sprite->SetTextureRectangle(first_frame);
            }
        }
    }

    std::shared_ptr<AnimationSequence> AnimatedSprite::GetCurrentAnimationSequence()
    {
        return AnimationSequences[CurrentAnimationSequenceName];
    }
}
