#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace MATH
{
    /// A 2D mathematical rectangle.  It is templated to
    /// support rectangles with different data types
    /// for coordinates.
    /// 
    /// This class is intended to be light-weight.
    /// It does not perform any error checking on
    /// the values provided to it (like making sure
    /// a width is not negative).  That responsibility
    /// lies with users of this class.
    ///
    /// @tparam CoordinateType - The data type to use
    ///     for the coordinates/dimensions of the rectangle.
    ///     This type will determine the accuracy of calculations
    ///     based on this rectangle - all operations will
    ///     be done using this type.
    template <typename CoordinateType>
    class Rectangle
    {
    public:
        // CONSTRUCTION/DESTRUCTION.
        static Rectangle FromLeftTopAndDimensions(
            const CoordinateType left_position,
            const CoordinateType top_position,
            const CoordinateType width,
            const CoordinateType height);
        static Rectangle FromCenterAndDimensions(
            const CoordinateType center_x_position,
            const CoordinateType center_y_position,
            const CoordinateType width,
            const CoordinateType height);
        explicit Rectangle() = default;
        explicit Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle);

        // OPERATORS.
        bool operator== (const Rectangle& rhs_rectangle) const;

        // POSITIONING.
        CoordinateType CenterX() const;
        CoordinateType CenterY() const;
        MATH::Vector2<CoordinateType> Center() const;
        MATH::Vector2<CoordinateType> LeftXBottomY() const;
        void SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position);

        // MOVEMENT.
        void Move(const MATH::Vector2<CoordinateType>& movement);

        // DIMENSIONS.
        CoordinateType Width() const;
        CoordinateType Height() const;

        // COLLISION TESTING.
        bool Contains(const CoordinateType x_position, const CoordinateType y_position) const;
        bool Intersects(const Rectangle& other_rectangle) const;

        // CONVERSION.
        template <typename DestinationCoordinateType>
        sf::Rect<DestinationCoordinateType> ToSfmlRectangle() const;

        // MEMBER VARIABLES.
        // The left, top (x, y) coordinates of the rectangle.
        MATH::Vector2<CoordinateType> LeftTop = MATH::Vector2<CoordinateType>();
        // The right, bottom (x, y) coordinates of the rectangle.
        MATH::Vector2<CoordinateType> RightBottom = MATH::Vector2<CoordinateType>();
    };

    // DEFINE COMMON RECTANGLE TYPES.
    /// A rectangle with integer coordinates.
    typedef Rectangle<int> IntRectangle;
    /// A rectangle with floating-point coordinates.
    typedef Rectangle<float> FloatRectangle;

    // CLASS IMPLEMENTATION.

    /// Creates a rectangle from the left-top coordinates and dimensions.
    /// @param[in]  left_position - The left position of the rectangle.
    /// @param[in]  top_position - The top position of the rectangle.
    /// @param[in]  width - The width of the rectangle.
    /// @param[in]  height - The height of the rectangle.
    /// @return A rectangle based on the provided values.
    template <typename CoordinateType>
    Rectangle<CoordinateType> Rectangle<CoordinateType>::FromLeftTopAndDimensions(
        const CoordinateType left_position,
        const CoordinateType top_position,
        const CoordinateType width,
        const CoordinateType height)
    {
        Rectangle<CoordinateType> rectangle;

        rectangle.LeftTop.X = left_position;
        rectangle.LeftTop.Y = top_position;
        rectangle.RightBottom.X = left_position + width;
        rectangle.RightBottom.Y = top_position + height;

        return rectangle;
    }

    /// Creates a rectangle from center coordinates and dimensions.
    /// @param[in]  center_x_position - The center X position of the rectangle.
    /// @param[in]  center_y_position - The center Y position of the rectangle.
    /// @param[in]  width - The width of the rectangle.
    /// @param[in]  height - The height of the rectangle.
    template <typename CoordinateType>
    Rectangle<CoordinateType> Rectangle<CoordinateType>::FromCenterAndDimensions(
        const CoordinateType center_x_position,
        const CoordinateType center_y_position,
        const CoordinateType width,
        const CoordinateType height)
    {
        // CALCULATE THE LEFT COORDINATE.
        CoordinateType half_width = width / static_cast<CoordinateType>(2);
        CoordinateType left_x_position = center_x_position - half_width;

        // CALCULATE THE TOP COORDINATE.
        CoordinateType half_height = height / static_cast<CoordinateType>(2);
        // Y decreases going up on the screen.
        CoordinateType top_y_position = center_y_position - half_height;

        // CREATE THE RECTANGLE.
        Rectangle<CoordinateType> rectangle = FromLeftTopAndDimensions(left_x_position, top_y_position, width, height);
        return rectangle;
    }

    /// Constructor accepting an SFML rectangle.
    /// @param[in]  sfml_rectangle - The SFML rectangle to use for this rectangle.
    template <typename CoordinateType>
    Rectangle<CoordinateType>::Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle) :
        LeftTop(sfml_rectangle.left, sfml_rectangle.top),
        RightBottom(sfml_rectangle.left + sfml_rectangle.width, sfml_rectangle.top + sfml_rectangle.height)
    {}

    /// Equality operator.
    /// @param[in]  rhs_rectangle - The rectangle on the right-hand side of the operator.
    /// @return     True if the rectangles are equal; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::operator== (const Rectangle<CoordinateType>& rhs_rectangle) const
    {
        bool equal = (
            LeftTop == rhs_rectangle.LeftTop &&
            RightBottom == rhs_rectangle.RightBottom);
        return equal;
    }

    /// Gets the center X position of the rectangle.
    /// @return The center X position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::CenterX() const
    {
        // Calculate the center using the midpoint formula.
        CoordinateType horizontal_midpoint = (LeftTop.X + RightBottom.X) / static_cast<CoordinateType>(2);
        return horizontal_midpoint;
    }

    /// Gets the center Y position of the rectangle.
    /// @return The center Y position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::CenterY() const
    {
        // Calculate the center using the midpoint formula.
        CoordinateType vertical_midpoint = (LeftTop.Y + RightBottom.Y) / static_cast<CoordinateType>(2);
        return vertical_midpoint;
    }

    /// Gets the center position of the rectangle.
    /// @return The center position of the rectangle.
    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::Center() const
    {
        CoordinateType center_x_position = CenterX();
        CoordinateType center_y_position = CenterY();
        MATH::Vector2<CoordinateType> center_position(center_x_position, center_y_position);
        return center_position;
    }

    /// Gets the left X and bottom Y corner position of the rectangle.
    /// @return The left-bottom corner of the rectangle.
    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::LeftXBottomY() const
    {
        MATH::Vector2<CoordinateType> left_bottom_corner(LeftTop.X, RightBottom.Y);
        return left_bottom_corner;
    }

    /// Sets the center position of the rectangle.
    /// @param[in]  center_x_position - The center X position of the rectangle.
    /// @param[in]  center_y_position - The center Y position of the rectangle.
    template <typename CoordinateType>
    void Rectangle<CoordinateType>::SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position)
    {
        *this = FromCenterAndDimensions(center_x_position, center_y_position, Width(), Height());
    }

    /// Moves the rectangle based on the specified vector.
    /// @param[in]  movement - The vector to move the rectangle by.
    template <typename CoordinateType>
    void Rectangle<CoordinateType>::Move(const MATH::Vector2<CoordinateType>& movement)
    {
        LeftTop += movement;
        RightBottom += movement;
    }

    /// Gets the width of the rectangle.
    /// @return The width of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::Width() const
    {
        CoordinateType width = RightBottom.X - LeftTop.X;
        return width;
    }

    /// Gets the height of the rectangle.
    /// @return The height of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::Height() const
    {
        CoordinateType height = RightBottom.Y - LeftTop.Y;
        return height;
    }

    /// Determines if the specified point is contained in this rectangle.
    /// @param[in]  x_position - The X position to check for containmnet in this rectangle.
    /// @param[in]  y_position - The Y position to check for containmnet in this rectangle.
    /// @return     True if the specified point is within this rectangle; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Contains(const CoordinateType x_position, const CoordinateType y_position) const
    {
        sf::Rect<CoordinateType> sfml_rectangle(LeftTop.X, LeftTop.Y, Width(), Height());
        return sfml_rectangle.contains(x_position, y_position);
    }

    /// Checks if the other rectangle intersects any portion of this rectangle.
    /// @param[in]  other_rectangle - The rectangle to test against this rectangle for intersection.
    /// @return True if the rectangles intersect; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Intersects(const Rectangle<CoordinateType>& other_rectangle) const
    {
        sf::Rect<CoordinateType> sfml_rectangle(LeftTop.X, LeftTop.Y, Width(), Height());
        return sfml_rectangle.intersects(other_rectangle.ToSfmlRectangle<CoordinateType>());
    }

    /// Converts the rectangle to an SFML rectangle with the specified coordinate type.
    /// Direct casting is used for conversion.
    /// @tparam DestinationCoordinateType - The type of the coordinates in the returned rectangle.
    /// @return An SFML rectangle with the specified coordinate type.
    template <typename CoordinateType>
    template <typename DestinationCoordinateType>
    sf::Rect<DestinationCoordinateType> Rectangle<CoordinateType>::ToSfmlRectangle() const
    {
        sf::Rect<DestinationCoordinateType> converted_rectangle;
        converted_rectangle.left = static_cast<DestinationCoordinateType>(LeftTop.X);
        converted_rectangle.top = static_cast<DestinationCoordinateType>(LeftTop.Y);
        converted_rectangle.width = static_cast<DestinationCoordinateType>(Width());
        converted_rectangle.height = static_cast<DestinationCoordinateType>(Height());
        return converted_rectangle;
    }
}