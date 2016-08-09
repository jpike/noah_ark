#pragma once

#include <string>
#include <vector>
#include <SFML/System.hpp>
#include "Math/Rectangle.h"

namespace GRAPHICS
{
    /// An sequence of graphical frames that form an animation.
    /// It is not intended to be automatically rendered by
    /// the graphics system but instead much be attached
    /// to another graphics component that is rendered.
    class AnimationSequence
    {
    public:
        // CONSTRUCTION.
        explicit AnimationSequence(
            const std::string& name,
            const bool is_looping,
            const sf::Time& total_duration,
            const std::vector<MATH::IntRectangle>& frames);

        // FRAME RETREIVAL.
        unsigned int GetFrameCount() const;
        MATH::IntRectangle GetFrame(const unsigned int frame_index) const;
        MATH::IntRectangle GetCurrentFrame() const;

        // ANIMATING.
        void Start();
        bool IsPlaying() const;
        void Progress(const sf::Time elapsed_time);
        void Reset();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        std::string AnimationName;    ///< The name of this animation.
        bool Loops;   ///< Whether or not this animation loops.
        sf::Time TotalDuration;   ///< The total duration of the animation sequence.
        std::vector<MATH::IntRectangle> Frames;    ///< The underlying Thor animation resource.

    private:
        // HELPER METHODS.
        sf::Time GetDurationPerFrame() const;

        // MEMBER VARIABLES.
        bool CurrentlyPlaying; ///< True if the animation sequence is playing; false otherwise.
        unsigned int CurrentFrameIndex; ///< The index of the current frame being played within the sequence.
        sf::Time ElapsedTimeForCurrentFrame; ///< The amount of time elapsed for current frame being played.
    };
}