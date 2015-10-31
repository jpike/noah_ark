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
        /// Constructor.
        /// @param[in]  name - The name to identify the animation sequence.
        /// @param[in]  is_looping - Whether the animation sequence loops or not.
        /// @param[in]  total_duration - The total duration of the animation sequence.
        /// @param[in]  frames - The frames in the sequence.
        explicit AnimationSequence(
            const std::string& name,
            const bool is_looping,
            const sf::Time& total_duration,
            const std::vector<MATH::IntRectangle>& frames);

        // FRAME RETREIVAL.
        /// Gets the total number of frames in the animation sequence.
        /// @return The total number of frames.
        unsigned int GetFrameCount() const;
        /// Gets the frame at the specified index.
        /// @param[in]  frame_index - The 0-based index of the frame to retrieve.
        /// @return The rectangular frame at the specified index.
        /// @throws std::out_of_range - Thrown if the index is out of bounds.
        MATH::IntRectangle GetFrame(const unsigned int frame_index) const;
        /// Gets the current frame within the playing of the animation sequence.
        /// @return The current frame.
        MATH::IntRectangle GetCurrentFrame() const;

        // ANIMATING.
        /// Starts playing the animation sequence from the beginning.
        void Start();
        /// Determines if the animation sequence is currently playing.
        /// @return True if the animation sequence is playing; false otherwise.
        bool IsPlaying() const;
        /// Progresses the animation the specified amount of time.
        /// @param[in]  elapsed_time - The amount of time to progress the animation.
        void Progress(const sf::Time elapsed_time);
        /// Stops the animation sequence from playing, resetting it back
        /// to the first frame.
        void Reset();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        std::string AnimationName;    ///< The name of this animation.
        bool Loops;   ///< Whether or not this animation loops.
        sf::Time TotalDuration;   ///< The total duration of the animation sequence.
        std::vector<MATH::IntRectangle> Frames;    ///< The underlying Thor animation resource.

    private:
        // HELPER METHODS.
        /// Gets the time duration for a single frame of the animation sequence.
        /// @return The duration per frame.
        sf::Time GetDurationPerFrame() const;

        // MEMBER VARIABLES.
        bool CurrentlyPlaying; ///< True if the animation sequence is playing; false otherwise.
        unsigned int CurrentFrameIndex; ///< The index of the current frame being played within the sequence.
        sf::Time ElapsedTimeForCurrentFrame; ///< The amount of time elapsed for current frame being played.
    };
}