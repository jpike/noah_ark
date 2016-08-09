#include "Graphics/AnimationSequence.h"

namespace GRAPHICS
{
    /// Constructor.
    /// @param[in]  name - The name to identify the animation sequence.
    /// @param[in]  is_looping - Whether the animation sequence loops or not.
    /// @param[in]  total_duration - The total duration of the animation sequence.
    /// @param[in]  frames - The frames in the sequence.
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

    /// Gets the total number of frames in the animation sequence.
    /// @return The total number of frames.
    unsigned int AnimationSequence::GetFrameCount() const
    {
        unsigned int frame_count = Frames.size();
        return frame_count;
    }

    /// Gets the frame at the specified index.
    /// @param[in]  frame_index - The 0-based index of the frame to retrieve.
    /// @return The rectangular frame at the specified index.
    /// @throws std::out_of_range - Thrown if the index is out of bounds.
    MATH::IntRectangle AnimationSequence::GetFrame(const unsigned int frame_index) const
    {
        // at() is used over [] since at() will perform bounds checking
        // and throw an exception if the index is out-of-bounds.
        MATH::IntRectangle frame = Frames.at(frame_index);
        return frame;
    }

    /// Gets the current frame within the playing of the animation sequence.
    /// @return The current frame.
    MATH::IntRectangle AnimationSequence::GetCurrentFrame() const
    {
        MATH::IntRectangle current_frame = GetFrame(CurrentFrameIndex);
        return current_frame;
    }

    /// Starts playing the animation sequence from the beginning.
    void AnimationSequence::Start()
    {
        CurrentlyPlaying = true;
        CurrentFrameIndex = 0;
        ElapsedTimeForCurrentFrame = sf::Time::Zero;
    }

    /// Determines if the animation sequence is currently playing.
    /// @return True if the animation sequence is playing; false otherwise.
    bool AnimationSequence::IsPlaying() const
    {
        return CurrentlyPlaying;
    }

    /// Progresses the animation the specified amount of time.
    /// @param[in]  elapsed_time - The amount of time to progress the animation.
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

    /// Stops the animation sequence from playing, resetting it back
    /// to the first frame.
    void AnimationSequence::Reset()
    {
        CurrentlyPlaying = false;
        CurrentFrameIndex = 0;
        ElapsedTimeForCurrentFrame = sf::Time::Zero;
    }

    /// Gets the time duration for a single frame of the animation sequence.
    /// @return The duration per frame.
    sf::Time AnimationSequence::GetDurationPerFrame() const
    {
        sf::Int64 frame_count = static_cast<sf::Int64>(GetFrameCount());
        sf::Time duration_per_frame = TotalDuration / frame_count;
        return duration_per_frame;
    }
}