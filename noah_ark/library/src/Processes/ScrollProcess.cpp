#include "Processes/ScrollProcess.h"

namespace PROCESSES
{
    ScrollProcess::ScrollProcess(
        const ScrollProcess::ScrollDirection direction,
        const MATH::Vector2f& startPoint,
        const MATH::Vector2f& endPoint,
        const float maximumScrollTimeInSeconds) :
        m_direction(direction),
        m_startPoint(startPoint),
        m_endPoint(endPoint),
        m_currentPoint(startPoint),
        m_maximumScrollTimeInSeconds(maximumScrollTimeInSeconds),
        m_totalScrollTimeSoFarInSeconds(0.0f)
    {}

    ScrollProcess::~ScrollProcess()
    {}

    MATH::Vector2f ScrollProcess::Scroll(const float elapsedTimeInSeconds)
    {
        // UPDATE THE TIME SPENT SCROLLING SO FAR.
        m_totalScrollTimeSoFarInSeconds += elapsedTimeInSeconds;
        bool maximumScrollTimeExceeded = (m_totalScrollTimeSoFarInSeconds >= m_maximumScrollTimeInSeconds);
        if (maximumScrollTimeExceeded)
        {
            // Force the scroll time and current point to the specified end to
            // ensure that too much scrolling doesn't occur.
            m_totalScrollTimeSoFarInSeconds = m_maximumScrollTimeInSeconds;
            m_currentPoint = m_endPoint;
            return m_currentPoint;
        }

        // UPDATE THE CURRENT POINT SCROLLED TO.
        // Basic linear interpolation is used to determine the point scrolled to.
        float scrollPercentageSoFar = m_totalScrollTimeSoFarInSeconds / m_maximumScrollTimeInSeconds;

        m_currentPoint.X = m_startPoint.X + ((m_endPoint.X - m_startPoint.X) * scrollPercentageSoFar);
        m_currentPoint.Y = m_startPoint.Y + ((m_endPoint.Y - m_startPoint.Y) * scrollPercentageSoFar);

        return m_currentPoint;
    }

    bool ScrollProcess::IsComplete() const
    {
        bool scrollingComplete = (m_totalScrollTimeSoFarInSeconds >= m_maximumScrollTimeInSeconds);
        return scrollingComplete;
    }

    ScrollProcess::ScrollDirection ScrollProcess::GetDirection() const
    {
        return m_direction;
    }

    MATH::Vector2f ScrollProcess::GetStartPoint() const
    {
        return m_startPoint;
    }

    MATH::Vector2f ScrollProcess::GetEndPoint() const
    {
        return m_endPoint;
    }
}
