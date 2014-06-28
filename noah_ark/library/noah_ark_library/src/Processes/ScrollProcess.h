#pragma once

#include "Math/Vector2.h"

/// @brief  Holds code related to processes that update over time.
namespace PROCESSES
{
    ///////////////////////////////////////////////////////////
    /// @brief  A process that controls scrolling from one point to
    ///         another over time.
    ///////////////////////////////////////////////////////////
    class ScrollProcess
    {
    public:
        /// @brief  An enumerated type to associate a direction with the scroll process.
        enum class ScrollDirection
        {
            UNSPECIFIED,    ///< Scrolling in an unspecified direction, or in a direction that can't be represented with other values of this enumeration.
            UP, ///< Scrolling up.
            DOWN,   ///< Scrolling down.
            LEFT,   ///< Scrolling left.
            RIGHT   ///< Scrolling right.
        };

        /// @brief      Constructor.
        /// @param[in]  direction - The general direction of scrolling.
        /// @param[in]  startPoint - The starting point of the scrolling.
        /// @param[in]  endPoint - The ending point where scrolling should stop.
        /// @param[in]  maximumScrollTimeInSeconds - The maximum amount of time (in seconds) that it
        ///             should take to scroll from the start point to the end point.
        ScrollProcess(
            const ScrollDirection direction,
            const MATH::Vector2f& startPoint, 
            const MATH::Vector2f& endPoint,
            const float maximumScrollTimeInSeconds);

        /// @brief  Destructor.
        ~ScrollProcess();

        /// @brief      Continues scrolling from the current point towards the end point.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time to scroll for.
        /// @return     The current point after scrolling for the provided time.
        MATH::Vector2f Scroll(const float elapsedTimeInSeconds);

        /// @brief  Checks whether scrolling is complete (has reached the end point) or not.
        /// @return True if scrolling is complete; false otherwise.
        bool IsComplete() const;

        /// @brief  Gets the direction of scrolling.
        /// @return The direction.
        ScrollDirection GetDirection() const;
        
        /// @brief  Gets the starting point of scrolling.
        /// @return The starting point.
        MATH::Vector2f GetStartPoint() const;

        /// @brief  Gets the ending point of scrolling.
        /// @return The ending point.
        MATH::Vector2f GetEndPoint() const;

    private:

        ScrollDirection m_direction;    ///< The general direction of the scrolling.
        
        MATH::Vector2f m_startPoint;    ///< The point where scrolling started.
        MATH::Vector2f m_endPoint;  ///< The point where scrolling should end.
        MATH::Vector2f m_currentPoint;  ///< The current point that we have scrolled to between the start and end points.

        float m_maximumScrollTimeInSeconds;  ///< The maximum amount of time (in seconds) that it should take to scroll from the start point to the end point.
        float m_totalScrollTimeSoFarInSeconds;   ///< The total amount of time spent scrolling, in seconds.
    };
}