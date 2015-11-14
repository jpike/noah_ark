#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
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
        /// Constructs a camera to manipulate the view of the provided render target.
        /// @param[in]  render_target - The render target whose view to control
        ///     via the camera.
        /// @throws std::invalid_argument - Thrown if the provided render target is null.
        /// @todo   Rethink passing render target here.  Maybe it would be better to
        /// have the camera more decoupled and just set it in the main loop?
        explicit Camera(const std::shared_ptr<sf::RenderTarget>& render_target);

        // CAMERA PARAMETER RETRIEVAL.
        /// Gets the rectangle defining the view of the camera, in world coordinates.
        /// @return The viewing rectangle of the camera.
        MATH::FloatRectangle GetViewBounds() const;

        // CAMERA POSITIONING.
        /// Sets the center world position upon which the camera should focus.
        /// @param[in]  center_x - The center world x position upon which
        ///     the camera should focus.
        /// @param[in]  center_y - The center world y position upon which
        ///     the camera should focus.
        void SetCenter(const float center_x, const float center_y);
        /// Sets the center world position upon which the camera should focus.
        /// @param[in]  center_position - The center world position upon which
        ///     the camera should focus.
        void SetCenter(const MATH::Vector2f& center_position);

        // SCROLLING.
        /// Starts the camera scrolling between the two positions.
        /// @param[in]  start_position - The starting world position of the scrolling.
        /// @param[in]  end_position - The ending world position of the scrolling.
        void StartScrolling(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position);
        /// Updates the scrolling of the camera based on the provided time.
        /// Assumed that scrolling of the camera has started.
        /// @param[in]  elapsed_time_in_seconds - The time by which to update
        ///     the scrolling.
        void Scroll(const float elapsed_time_in_seconds);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The render target whose view is controlled by this camera.
        std::shared_ptr<sf::RenderTarget> RenderTarget;
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
