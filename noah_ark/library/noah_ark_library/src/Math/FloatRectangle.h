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
        /// @brief      Constructor.
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        /// @param[in]  widthInPixels - The width of the rectangle.
        /// @param[in]  heightInPixels - The height of the rectangle.
        explicit FloatRectangle(
            const float centerWorldXPositionInPixels,
            const float centerWorldYPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);
        /// @brief      Constructor accepting an HGE rectangle.
        /// @param[in]  hgeRectangle - The HGE rectangle to use for the float rectangle.
        explicit FloatRectangle(const hgeRect& hgeRectangle);
        /// @brief  Destructor.
        ~FloatRectangle();

        /// @brief  Gets the center X position of the rectangle, in world pixel coordinates.
        /// @return The center X position of the rectangle.
        float GetCenterXPosition() const;
        /// @brief  Gets the center Y position of the rectangle, in world pixel coordinates.
        /// @return The center Y position of the rectangle.
        float GetCenterYPosition() const;

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

    private:

        /// @brief      Recalculates the rectangle based on the provided information.
        /// @param[in]  centerWorldXPositionInPixels - The center X position of the rectangle.
        /// @param[in]  centerWorldYPositionInPixels - The center Y position of the rectangle.
        /// @param[in]  widthInPixels - The width of the rectangle.
        /// @param[in]  heightInPixels - The height of the rectangle.
        /// @return     The underlying HGE rectangle based on the provided parameters.
        hgeRect RecalculateRectangle(
            const float centerWorldXPositionInPixels,
            const float centerWorldYPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);

        hgeRect m_rectangle;    ///< The underlying HGE rectangle.
    };
}