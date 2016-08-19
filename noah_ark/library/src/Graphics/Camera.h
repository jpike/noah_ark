#pragma once

#include <memory>
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    /// A camera for defining the viewable area of the game
    /// that gets rendered.
    class Camera
    {
    public:
        // CONSTRUCTION.
        explicit Camera(const MATH::FloatRectangle& view_bounds);

        // CAMERA POSITIONING.
        void SetCenter(const float center_x, const float center_y);
        void SetCenter(const MATH::Vector2f& center_position);

        // SCROLLING.
        void StartScrolling(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position);
        void Scroll(const float elapsed_time_in_seconds);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The bounding rectangle (in world coordinates) of the camera's view.
        MATH::FloatRectangle ViewBounds;
        /// True if the camera is currently scrolling; false otherwise.
        bool IsScrolling;

    private:
        // MEMBER VARIABLES.
        /// The starting world position of scrolling, if the camera is currently scrolling.
        MATH::Vector2f ScrollStartPosition;
        /// The ending world position of scrolling, if the camera is currently scrolling.
        MATH::Vector2f ScrollEndPosition;
        /// The current total time spent during scrolling, if scrolling has started.
        float CurrentTotalScrollTimeInSeconds;
    };
}
