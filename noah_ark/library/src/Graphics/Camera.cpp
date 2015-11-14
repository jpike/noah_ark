#include <stdexcept>
#include "Graphics/Camera.h"

namespace GRAPHICS
{
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

    void Camera::SetCenter(const float center_x, const float center_y)
    {
        // MODIFY THE RENDER TARGET'S VIEW.
        auto view = RenderTarget->getView();
        view.setCenter(center_x, center_y);
        RenderTarget->setView(view);
    }

    void Camera::SetCenter(const MATH::Vector2f& center_position)
    {
        SetCenter(center_position.X, center_position.Y);
    }

    void Camera::StartScrolling(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position)
    {
        // SAVE THE POSITIONS OF THE SCROLLING.
        ScrollStartPosition = start_position;
        ScrollEndPosition = end_position;

        // START THE SCROLLING ANEW.
        CurrentTotalScrollTimeInSeconds = 0.0f;
        IsScrolling = true;
    }

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
