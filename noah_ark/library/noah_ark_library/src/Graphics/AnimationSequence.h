#pragma once

#include <hgeanim.h>
#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An sequence of graphical frames that form an animation.
    ///         It is not intended to be automatically rendered by
    ///         the graphics system but instead much be attached
    ///         to another graphics component that is rendered.
    ///
    ///         It requires an HGE animation resource, of which the
    ///         responsibility for managing this memory is
    ///         external to this class, but the resource should
    ///         not be deleted while the instance given this resource
    ///         is in use.
    ///////////////////////////////////////////////////////////
    class AnimationSequence
    {
    public:
        /// @brief      Constructor.  The sprite is visible by default.
        /// @param      pAnimation - The HGE animation resource to use.
        /// @throws     std::invalid_argument - If pAnimation is null.
        explicit AnimationSequence(hgeAnimation* const pAnimation);
        
        /// @brief  Destructor.
        ~AnimationSequence();

        /// @brief      Renders the current frame of the animation at the provided world position.
        /// @param[in]  worldXPositionInPixels - The world X position at which to render the animation.
        /// @param[in]  worldYPositionInPixels - The world Y position at which to render the animation.
        void Render(const float worldXPositionInPixels, const float worldYPositionInPixels);

        /// @brief      Updates the animation sequence for a single frame based on
        ///             the provided time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame
        ///             for wich to update the animation sequence.
        void Update(const float elapsedTimeInSeconds);

        /// @brief      Sets the Z-position for the graphics component.  This is used
        ///             to order how graphics are layered on top of each other.
        ///             Lower values are on the bottom.
        /// @param[in]  zPosition - The z-position to set for the graphics component.
        void SetZPosition(const float zPosition);

        /// @brief      Gets the bounding box of the animation sequence, given
        ///             the provided world coordinates.
        /// @param[in]  worldXPositionInPixels - The world X position that the animation sequence is rendered at.
        /// @param[in]  worldYPositionInPixels - The world Y position that the animation sequence is rendered at.
        /// @return     The bounding box of the animation sequence.
        hgeRect GetWorldBoundingBox(const float worldXPositionInPixels, const float worldYPositionInPixels);

        /// @brief  Begins playing the animation sequence, if it isn't already playing.
        void Play();

        /// @brief  Resets the animation to its first frame and stops it from playing.
        void Reset();

    private:
        AnimationSequence(const AnimationSequence& animationToCopy);    ///< Private to disallow copying.
        AnimationSequence& operator=(const AnimationSequence& rhsAnimation);    ///< Private to disallow copying.

        hgeAnimation* m_pAnimation;    ///< The underlying HGE animation resource.
    };
}