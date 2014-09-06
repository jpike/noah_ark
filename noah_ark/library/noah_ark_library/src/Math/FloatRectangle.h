#pragma once

#include <hgerect.h>

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
        /// Constructor.
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        /// @param[in]  widthInPixels - The width of the rectangle.
        /// @param[in]  heightInPixels - The height of the rectangle.
        explicit FloatRectangle(
            const float centerWorldXPositionInPixels,
            const float centerWorldYPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);
        /// Destructor.
        ~FloatRectangle();

        /// Gets the center X position of the rectangle, in world pixel coordinates.
        /// @return The center X position of the rectangle.
        float GetCenterXPosition() const;
        /// Gets the center Y position of the rectangle, in world pixel coordinates.
        /// @return The center Y position of the rectangle.
        float GetCenterYPosition() const;

        /// Gets the width of the rectangle, in pixels.
        /// @return The width of the rectangle.
        float GetWidth() const;
        /// Gets the height of the rectangle, in pixels.
        /// @return The height of the rectangle.
        float GetHeight() const;

    private:

        hgeRect m_rectangle;    ///< The underlying HGE rectangle.
    };
}