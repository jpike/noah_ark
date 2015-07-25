#include "Graphics/AnimationSequence.h"

namespace GRAPHICS
{
    AnimationSequence::AnimationSequence(
        const std::string& name,
        const bool is_looping,
        const sf::Time& total_duration,
        const std::vector<MATH::IntRectangle>& frames) :
    AnimationName(name),
    Loops(is_looping),
    TotalDuration(total_duration),
    Frames(frames),
    CurrentlyPlaying(false),
    CurrentFrameIndex(0),
    ElapsedTimeForCurrentFrame(sf::Time::Zero)
    {}

    AnimationSequence::~AnimationSequence()
    {}

    std::string AnimationSequence::GetName() const
    {
        return AnimationName;
    }

    bool AnimationSequence::IsLooping() const
    {
        return Loops;
    }

    sf::Time AnimationSequence::GetDuration() const
    {
        return TotalDuration;
    }

    unsigned int AnimationSequence::GetFrameCount() const
    {
        unsigned int frame_count = Frames.size();
        return frame_count;
    }

    unsigned int AnimationSequence::GetCurrentFrameIndex() const
    {
        return CurrentFrameIndex;
    }

    std::vector<MATH::IntRectangle> AnimationSequence::GetFrames() const
    {
        return Frames;
    }

    MATH::IntRectangle AnimationSequence::GetFrame(const unsigned int frame_index) const
    {
        // at() is used over [] since at() will perform bounds checking
        // and throw an exception if the index is out-of-bounds.
        MATH::IntRectangle frame = Frames.at(frame_index);
        return frame;
    }

    MATH::IntRectangle AnimationSequence::GetCurrentFrame() const
    {
        MATH::IntRectangle current_frame = GetFrame(CurrentFrameIndex);
        return current_frame;
    }

    void AnimationSequence::Start()
    {
        CurrentlyPlaying = true;
        CurrentFrameIndex = 0;
        ElapsedTimeForCurrentFrame = sf::Time::Zero;
    }

    bool AnimationSequence::IsPlaying() const
    {
        return CurrentlyPlaying;
    }

    void AnimationSequence::Progress(const sf::Time elapsed_time)
    {
        // ADD THE ELAPSED TIME FOR THE CURRENT FRAME.
        ElapsedTimeForCurrentFrame += elapsed_time;

        // CHECK IF TIME FOR THE CURRENT FRAME HAS BEEN ELAPSED.
        sf::Time duration_per_frame = GetDurationPerFrame();
        bool full_time_for_current_frame_elapsed = (ElapsedTimeForCurrentFrame >= duration_per_frame);
        if (!full_time_for_current_frame_elapsed)
        {
            // Nothing more needs to be done since the animation should remain
            // on the current frame.
            return;
        }

        // MOVE TO THE NEXT FRAME.
        ElapsedTimeForCurrentFrame = sf::Time::Zero;
        ++CurrentFrameIndex;

        // CHECK IF THE ANIMATION SEQUENCE HAS COMPLETED.
        unsigned int total_frame_count = GetFrameCount();
        bool animation_sequence_has_completed_one_full_iteration = (CurrentFrameIndex >= total_frame_count);
        if (animation_sequence_has_completed_one_full_iteration)
        {
            // CHECK IF THE ANIMATION SHOULD LOOP.
            if (Loops)
            {
                // RESTART THE ANIMATION FROM THE BEGINNING.
                Start();
            }
            else
            {
                // STOP AND RESET THE ANIMATION.
                Reset();
            }
        }
    }

    void AnimationSequence::Reset()
    {
        CurrentlyPlaying = false;
        CurrentFrameIndex = 0;
        ElapsedTimeForCurrentFrame = sf::Time::Zero;
    }

    sf::Time AnimationSequence::GetDurationPerFrame() const
    {
        sf::Int64 frame_count = static_cast<sf::Int64>(GetFrameCount());
        sf::Time duration_per_frame = TotalDuration / frame_count;
        return duration_per_frame;
    }
}