#pragma once

#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace MATH
{
    ///////////////////////////////////////////////////////////
    /// @brief  A 2D mathematical rectangle with floating-point
    ///         data types.
    ///
    ///         This class is intended to be light-weight.
    ///         It does not perform any error checking on
    ///         the values provided to it (like making sure
    ///         a width is not negative).  That responsibility
    ///         lies with users of this class.
    ///////////////////////////////////////////////////////////
    class FloatRectangle
    {
    public:
        /// Creates a rectangle from the top-left coordinates and dimensions.
        /// @param[in]  left_position - The left position of the rectangle.
        /// @param[in]  top_position - The top position of the rectangle.
        /// @param[in]  width - The width of the rectangle.
        /// @param[in]  height - The height of the rectangle.
        /// @return A rectangle based on the provided values.
        static FloatRectangle FromTopLeftAndDimensions(
            const float left_position,
            const float top_position,
            const float width,
            const float height);
        /// Constructor.  Default parameter values allow for easy creation of an
        /// invalid rectangle (no width/height).
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        /// @param[in]  widthInPixels - The width of the rectangle.
        /// @param[in]  heightInPixels - The height of the rectangle.
        explicit FloatRectangle(
            const float centerWorldXPositionInPixels = 0.0f,
            const float centerWorldYPositionInPixels = 0.0f,
            const float widthInPixels = 0.0f,
            const float heightInPixels = 0.0f);
        /// @brief      Constructor accepting an HGE rectangle.
        /// @param[in]  sfmlRectangle - The SFML rectangle to use for the float rectangle.
        explicit FloatRectangle(const sf::FloatRect& sfmlRectangle);
        /// @brief  Destructor.
        ~FloatRectangle();

        /// @brief  Gets the center X position of the rectangle, in world pixel coordinates.
        /// @return The center X position of the rectangle.
        float GetCenterXPosition() const;
        /// @brief  Gets the center Y position of the rectangle, in world pixel coordinates.
        /// @return The center Y position of the rectangle.
        float GetCenterYPosition() const;
        /// Gets the center position of the rectangle, in world pixel coordinates.
        /// @return The center position of the rectangl.
        MATH::Vector2f GetCenterPosition() const;

        /// @brief  Gets the left X position of the rectangle, in world pixel coordinates.
        /// @return The left X position of the rectangle.
        float GetLeftXPosition() const;
        /// @brief  Gets the right X position of the rectangle, in world pixel coordinates.
        /// @return The right X position of the rectangle.
        float GetRightXPosition() const;

        /// @brief  Gets the top Y position of the rectangle, in world pixel coordinates.
        /// @return The top Y position of the rectangle.
        float GetTopYPosition() const;
        /// @brief  Gets the bottom Y position of the rectangle, in world pixel coordinates.
        /// @return The bottom Y position of the rectangle.
        float GetBottomYPosition() const;
        
        /// @brief  Sets the center world position (in pixels) of the rectangle.
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        void SetCenterPosition(const float centerWorldXPositionInPixels, const float centerWorldYPositionInPixels);

        /// @brief  Gets the width of the rectangle, in pixels.
        /// @return The width of the rectangle.
        float GetWidth() const;
        /// @brief  Gets the height of the rectangle, in pixels.
        /// @return The height of the rectangle.
        float GetHeight() const;

        /// @brief      Determines if the specified point is contained in this rectangle.
        /// @param[in]  worldXPositionInPixels - The world X position to check for containmnet in this rectangle.
        /// @param[in]  worldYPositionInPixels - The world Y position to check for containmnet in this rectangle.
        /// @return     True if the specified point is within this rectangle; false otherwise.
        bool Contains(const float worldXPositionInPixels, const float worldYPositionInPixels) const;

        /// Checks if the other rectangle intersects any portion of this rectangle.
        /// @param[in]  other_rectangle - The rectangle to test against this rectangle for intersection.
        /// @return True if the rectangles intersect; false otherwise.
        bool Intersects(const MATH::FloatRectangle& other_rectangle) const;

    private:

        /// @brief      Recalculates the rectangle based on the provided information.
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        /// @param[in]  widthInPixels - The width of the rectangle.
        /// @param[in]  heightInPixels - The height of the rectangle.
        /// @return     The underlying SFML rectangle based on the provided parameters.
        sf::FloatRect RecalculateRectangle(
            const float centerWorldXPositionInPixels,
            const float centerWorldYPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);

        sf::FloatRect m_rectangle;    ///< The underlying SFML rectangle.
    };
}