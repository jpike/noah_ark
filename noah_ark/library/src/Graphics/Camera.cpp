#include <stdexcept>
#include "Graphics/Camera.h"

namespace GRAPHICS
{
    /// Constructs a camera to manipulate the view of the provided render target.
    /// @param[in]  render_target - The render target whose view to control
    ///     via the camera.
    /// @throws std::invalid_argument - Thrown if the provided render target is null.
    /// @todo   Rethink passing render target here.  Maybe it would be better to
    /// have the camera more decoupled and just set it in the main loop?
    Camera::Camera(const std::shared_ptr<sf::RenderTarget>& render_target) :
    RenderTarget(render_target),
    IsScrolling(false),
    ScrollStartPosition(),
    ScrollEndPosition(),
    CurrentTotalScrollTimeInSeconds(0.0f)
    {
        // MAKE SURE A VALID RENDER TARGET WAS PROVIDED.
        bool render_target_exists = (nullptr != RenderTarget);
        if (!render_target_exists)
        {
            // A valid render target is required for construction.
            throw std::invalid_argument("Null render target cannot be provided to Camera constructor.");
        }
    }

    /// Gets the rectangle defining the view of the camera, in world coordinates.
    /// @return The viewing rectangle of the camera.
    MATH::FloatRectangle Camera::GetViewBounds() const
    {
        // GET THE RENDER TARGET'S VIEW PARAMETERS.
        auto view = RenderTarget->getView();
        sf::Vector2f view_center = view.getCenter();
        sf::Vector2f view_size = view.getSize();

        // RETURN THE VIEW'S BOUNDS.
        MATH::FloatRectangle camera_bounds = MATH::FloatRectangle::FromCenterAndDimensions(
            view_center.x,
            view_center.y,
            view_size.x,
            view_size.y);
        return camera_bounds;
    }

    /// Sets the center world position upon which the camera should focus.
    /// @param[in]  center_x - The center world x position upon which
    ///     the camera should focus.
    /// @param[in]  center_y - The center world y position upon which
    ///     the camera should focus.
    void Camera::SetCenter(const float center_x, const float center_y)
    {
        // MODIFY THE RENDER TARGET'S VIEW.
        auto view = RenderTarget->getView();
        view.setCenter(center_x, center_y);
        RenderTarget->setView(view);
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
    /// @param[in]  elapsed_time_in_seconds - The time by which to update
    ///     the scrolling.
    void Camera::Scroll(const float elapsed_time_in_seconds)
    {
        // CHECK IF SCROLLING IS FINISHED.
        const float MAX_SCROLL_TIME_IN_SECONDS = 3.0f;
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
