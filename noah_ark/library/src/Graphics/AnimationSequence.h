#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An sequence of graphical frames that form an animation.
    ///         It is not intended to be automatically rendered by
    ///         the graphics system but instead much be attached
    ///         to another graphics component that is rendered.
    ///////////////////////////////////////////////////////////
    class AnimationSequence
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  name - The name to identify the animation sequence.
        /// @param[in]  isLooping - Whether the animation sequence loops or not.
        /// @param[in]  totalDuration - The total duration of the animation sequence.
        /// @param[in]  frames - The frames in the sequence.
        explicit AnimationSequence(
            const std::string& name,
            const bool isLooping,
            const sf::Time& totalDuration,
            const std::vector<sf::IntRect>& frames);
        
        /// @brief  Destructor.
        ~AnimationSequence();

        /// @brief  Gets the name assigned to this animation sequence.
        /// @return The name of this animation sequence.
        std::string GetName() const;

        /// @brief  Gets whether or not this animation sequence loops.
        /// @return Whether or not this this animation loops.
        bool IsLooping() const;

        /// @brief  Gets the total duration of the animation sequence.
        /// @return The total duration of the animation.
        sf::Time GetDuration() const;

        /// @brief  Gets the frame information for the animation sequences.
        /// @return The frame animation information.
        std::vector<sf::IntRect> GetFrames() const;

        /// Gets the frame at the specified index.
        /// @param[in]  frame_index - The 0-based index of the frame to retrieve.
        /// @return The rectangular frame at the specified index.
        /// @throws std::out_of_range - Thrown if the index is out of bounds.
        sf::IntRect GetFrame(const unsigned int frame_index) const;

        /// Gets the total number of frames in the animation sequence.
        /// @return The total number of frames.
        unsigned int GetFrameCount() const;

        /// Gets the current frame within the playing of the animation sequence.
        /// @return The current frame.
        sf::IntRect GetCurrentFrame() const;

        /// Gets the 0-based index for the current frame of the animation sequence.
        /// @return The current frame index.
        unsigned int GetCurrentFrameIndex() const;

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

    private:
        AnimationSequence(const AnimationSequence&);    ///< Private to disallow copying.
        AnimationSequence& operator=(const AnimationSequence&);    ///< Private to disallow copying.

        /// Gets the time duration for a single frame of the animation sequence.
        /// @return The duration per frame.
        sf::Time GetDurationPerFrame() const;

        std::string AnimationName;    ///< The name of this animation.
        bool Loops;   ///< Whether or not this animation loops.
        sf::Time TotalDuration;   ///< The total duration of the animation sequence.
        std::vector<sf::IntRect> Frames;    ///< The underlying Thor animation resource.

        bool CurrentlyPlaying; ///< True if the animation sequence is playing; false otherwise.
        unsigned int CurrentFrameIndex; ///< The index of the current frame being played within the sequence.
        sf::Time ElapsedTimeForCurrentFrame; ///< The amount of time elapsed for current frame being played.
    };
}