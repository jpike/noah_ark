#pragma once

#include <string>
#include <Thor/Animation.hpp>

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An sequence of graphical frames that form an animation.
    ///         It is not intended to be automatically rendered by
    ///         the graphics system but instead much be attached
    ///         to another graphics component that is rendered.
    ///
    ///         It requires a Thor animation resource.
    ///////////////////////////////////////////////////////////
    class AnimationSequence
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  name - The name to identify the animation sequence.
        /// @param[in]  isLooping - Whether the animation sequence loops or not.
        /// @param[in]  totalDuration - The total duration of the animation sequence.
        /// @param[in]  frameAnimation - The Thor animation resource to use.
        explicit AnimationSequence(
            const std::string& name,
            const bool isLooping,
            const sf::Time& totalDuration,
            const thor::FrameAnimation& frameAnimation);
        
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
        thor::FrameAnimation GetFrames() const;

    private:
        AnimationSequence(const AnimationSequence& animationToCopy);    ///< Private to disallow copying.
        AnimationSequence& operator=(const AnimationSequence& rhsAnimation);    ///< Private to disallow copying.

        std::string m_animationName;    ///< The name of this animation.
        bool m_isLooping;   ///< Whether or not this animation loops.
        sf::Time m_totalDuration;   ///< The total duration of the animation sequence.
        thor::FrameAnimation m_frameAnimation;    ///< The underlying Thor animation resource.
    };
}