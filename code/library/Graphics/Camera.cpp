#include "Graphics/Camera.h"

namespace GRAPHICS
{
    /// Constructs a camera with the provided initial view.
    /// @param[in]  view_bounds - The bounding rectangle (in world coordinates) of the camera's view.
    Camera::Camera(const MATH::FloatRectangle& view_bounds) :
    ViewBounds(view_bounds),
    IsScrolling(false),
    ScrollStartPosition(),
    ScrollEndPosition(),
    CurrentTotalScrollTimeInSeconds(0.0f)
    {}

    /// Sets the center world position upon which the camera should focus.
    /// @param[in]  center_x - The center world x position upon which
    ///     the camera should focus.
    /// @param[in]  center_y - The center world y position upon which
    ///     the camera should focus.
    void Camera::SetCenter(const float center_x, const float center_y)
    {
        ViewBounds.SetCenterPosition(center_x, center_y);
    }

    /// Sets the center world position upon which the camera should focus.
    /// @param[in]  center_position - The center world position upon which
    ///     the camera should focus.
    void Camera::SetCenter(const MATH::Vector2f& center_position)
    {
        SetCenter(center_position.X, center_position.Y);
    }

    /// Starts the camera scrolling between the two positions.
    /// @param[in]  start_position - The starting world position of the scrolling.
    /// @param[in]  end_position - The ending world position of the scrolling.
    void Camera::StartScrolling(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position)
    {
        // SAVE THE POSITIONS OF THE SCROLLING.
        ScrollStartPosition = start_position;
        ScrollEndPosition = end_position;

        // START THE SCROLLING ANEW.
        CurrentTotalScrollTimeInSeconds = 0.0f;
        IsScrolling = true;
    }

    /// Updates the scrolling of the camera based on the provided time.
    /// Assumed that scrolling of the camera has started.
    /// @param[in]  elapsed_time - The time by which to update
    ///     the scrolling.
    void Camera::Scroll(const sf::Time& elapsed_time)
    {
        // CHECK IF SCROLLING IS FINISHED.
#if _DEBUG
        // A shorter time is used for debug builds to speed up debugging.
        // Currently seems a bit too fast for the final game, but that might change.
        constexpr float MAX_SCROLL_TIME_IN_SECONDS = 1.5f;
#else
        constexpr float MAX_SCROLL_TIME_IN_SECONDS = 2.0f;
#endif
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        CurrentTotalScrollTimeInSeconds += elapsed_time_in_seconds;
        bool scrolling_finished = (CurrentTotalScrollTimeInSeconds >= MAX_SCROLL_TIME_IN_SECONDS);
        if (scrolling_finished)
        {
            // STOP THE SCROLLING.
            CurrentTotalScrollTimeInSeconds = MAX_SCROLL_TIME_IN_SECONDS;
            IsScrolling = false;
        }

        // POSITION THE CAMERA BASED ON HOW LONG SCROLLING HAS BEEN GOING ON.
        float scroll_percentage_so_far = CurrentTotalScrollTimeInSeconds / MAX_SCROLL_TIME_IN_SECONDS;

        float current_camera_center_x = ScrollStartPosition.X + ((ScrollEndPosition.X - ScrollStartPosition.X) * scroll_percentage_so_far);
        float current_camera_center_y = ScrollStartPosition.Y + ((ScrollEndPosition.Y - ScrollStartPosition.Y) * scroll_percentage_so_far);
        SetCenter(current_camera_center_x, current_camera_center_y);
    }
}
