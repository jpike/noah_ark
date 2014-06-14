#pragma once

#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A 2D camera for controlling what portion of the game
    ///         world is rendered on screen.
    ///////////////////////////////////////////////////////////
    class Camera
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  leftViewPosition - The left X world position in the camera's view, in pixels.
        /// @param[in]  topViewPositoin - The top Y world position in the camera's view, in pixels.
        explicit Camera(const float leftViewPosition = 0.0f, const float topViewPosition = 0.0f);

        /// @brief  Destructor.
        ~Camera();

        /// @brief  Gets the left X position (in pixels) of the world that
        ///         should currently be in view of this camera.
        /// @return The left position of the world currently being viewed.
        float GetLeftViewPosition() const;
        /// @brief  Gets the top X position (in pixels) of the world that
        ///         should currently be in view of this camera.
        /// @return The top position of the world currently being viewed.
        float GetTopViewPosition() const;

        /// @brief      Sets the top-left position of the world that should currently
        ///             be viewed by this camera.
        /// @param[in]  topLeftViewPosition - The top-left position (in pixels) of the
        ///             world that should be viewed.
        void SetTopLeftViewPosition(const MATH::Vector2f& topLeftViewPosition);

        /// @brief      Moves the camera up by the specified amount.
        /// @param[in]  distanceInPixels - The distance to move the camera.
        void MoveUp(const float distanceInPixels);
        /// @brief      Moves the camera down by the specified amount.
        /// @param[in]  distanceInPixels - The distance to move the camera.
        void MoveDown(const float distanceInPixels);
        /// @brief      Moves the camera left by the specified amount.
        /// @param[in]  distanceInPixels - The distance to move the camera.
        void MoveLeft(const float distanceInPixels);
        /// @brief      Moves the camera right by the specified amount.
        /// @param[in]  distanceInPixels - The distance to move the camera.
        void MoveRight(const float distanceInPixels);

    private:

        MATH::Vector2f m_topLeftViewPositionInPixels;  ///< The top-left world position being rendered.
    };
}