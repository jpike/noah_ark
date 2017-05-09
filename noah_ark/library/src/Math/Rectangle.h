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
        explicit Rectangle();
        explicit Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle);
        ~Rectangle();

        // OPERATORS.
        bool operator== (const Rectangle& rhs_rectangle) const;

        // POSITIONING.
        CoordinateType GetCenterXPosition() const;
        CoordinateType GetCenterYPosition() const;
        MATH::Vector2<CoordinateType> GetCenterPosition() const;
        CoordinateType GetLeftXPosition() const;
        CoordinateType GetRightXPosition() const;
        CoordinateType GetTopYPosition() const;
        CoordinateType GetBottomYPosition() const;
        MATH::Vector2<CoordinateType> GetLeftXTopYPosition() const;
        MATH::Vector2<CoordinateType> GetLeftXBottomYPosition() const;
        void SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position);

        // MOVEMENT.
        void Move(const MATH::Vector2<CoordinateType>& movement);

        // DIMENSIONS.
        CoordinateType GetWidth() const;
        CoordinateType GetHeight() const;

        // COLLISION TESTING.
        bool Contains(const CoordinateType x_position, const CoordinateType y_position) const;
        bool Intersects(const Rectangle& other_rectangle) const;

        // CONVERSION.
        template <typename DestinationCoordinateType>
        sf::Rect<DestinationCoordinateType> ToSfmlRectangle() const;

    private:
        // HELPER METHODS.
        static sf::Rect<CoordinateType> RecalculateRectangle(
            const CoordinateType center_x_position,
            const CoordinateType center_y_position,
            const CoordinateType width,
            const CoordinateType height);

        // MEMBER VARIABLES.
        sf::Rect<CoordinateType> SfmlRectangle;    ///< The underlying SFML rectangle.
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

        rectangle.SfmlRectangle.left = left_position;
        rectangle.SfmlRectangle.top = top_position;
        rectangle.SfmlRectangle.width = width;
        rectangle.SfmlRectangle.height = height;

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
        sf::Rect<CoordinateType> sfml_rectangle = RecalculateRectangle(
            center_x_position,
            center_y_position,
            width,
            height);

        Rectangle<CoordinateType> rectangle(sfml_rectangle);
        return rectangle;
    }

    /// Default constructor to create an invalid rectangle
    /// with 0 for all values.
    template <typename CoordinateType>
    Rectangle<CoordinateType>::Rectangle() :
    SfmlRectangle()
    {}

    /// Constructor accepting an SFML rectangle.
    /// @param[in]  sfml_rectangle - The SFML rectangle to use for this rectangle.
    template <typename CoordinateType>
    Rectangle<CoordinateType>::Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle) :
    SfmlRectangle(sfml_rectangle)
    {}

    /// Destructor.
    template <typename CoordinateType>
    Rectangle<CoordinateType>::~Rectangle()
    {}

    /// Equality operator.
    /// @param[in]  rhs_rectangle - The rectangle on the right-hand side of the operator.
    /// @return     True if the rectangles are equal; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::operator== (const Rectangle<CoordinateType>& rhs_rectangle) const
    {
        bool equal = (SfmlRectangle == rhs_rectangle.SfmlRectangle);
        return equal;
    }

    /// Gets the center X position of the rectangle.
    /// @return The center X position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetCenterXPosition() const
    {
        // Calculate the center using the midpoint formula.
        const CoordinateType left_x_position = GetLeftXPosition();
        const CoordinateType right_x_position = GetRightXPosition();
        const CoordinateType horizontal_midpoint = (left_x_position + right_x_position) / static_cast<CoordinateType>(2);
        return horizontal_midpoint;
    }

    /// Gets the center Y position of the rectangle.
    /// @return The center Y position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetCenterYPosition() const
    {
        // Calculate the center using the midpoint formula.
        const CoordinateType top_y_position = GetTopYPosition();
        const CoordinateType bottom_y_position = GetBottomYPosition();
        const CoordinateType vertical_midpoint = (top_y_position + bottom_y_position) / static_cast<CoordinateType>(2);
        return vertical_midpoint;
    }

    /// Gets the center position of the rectangle.
    /// @return The center position of the rectangle.
    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::GetCenterPosition() const
    {
        CoordinateType center_x_position = GetCenterXPosition();
        CoordinateType center_y_position = GetCenterYPosition();
        MATH::Vector2<CoordinateType> center_position(center_x_position, center_y_position);
        return center_position;
    }

    /// Gets the left X position of the rectangle.
    /// @return The left X position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetLeftXPosition() const
    {
        return SfmlRectangle.left;
    }

    /// Gets the right X position of the rectangle.
    /// @return The right X position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetRightXPosition() const
    {
        const CoordinateType right_x_position = SfmlRectangle.left + SfmlRectangle.width;
        return right_x_position;
    }

    /// Gets the top Y position of the rectangle.
    /// @return The top Y position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetTopYPosition() const
    {
        return SfmlRectangle.top;
    }

    /// Gets the bottom Y position of the rectangle.
    /// @return The bottom Y position of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetBottomYPosition() const
    {
        const CoordinateType bottom_y_position = SfmlRectangle.top + SfmlRectangle.height;
        return bottom_y_position;
    }

    /// Gets the left X and top Y corner position of the rectangle.
    /// @return The left-top corner of the rectangle.
    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::GetLeftXTopYPosition() const
    {
        CoordinateType left_x_position = GetLeftXPosition();
        CoordinateType top_y_position = GetTopYPosition();
        MATH::Vector2<CoordinateType> left_top_corner(left_x_position, top_y_position);
        return left_top_corner;
    }

    /// Gets the left X and bottom Y corner position of the rectangle.
    /// @return The left-bottom corner of the rectangle.
    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::GetLeftXBottomYPosition() const
    {
        CoordinateType left_x_position = GetLeftXPosition();
        CoordinateType bottom_y_position = GetBottomYPosition();
        MATH::Vector2<CoordinateType> left_bottom_corner(left_x_position, bottom_y_position);
        return left_bottom_corner;
    }

    /// Sets the center position of the rectangle.
    /// @param[in]  center_x_position - The center X position of the rectangle.
    /// @param[in]  center_y_position - The center Y position of the rectangle.
    template <typename CoordinateType>
    void Rectangle<CoordinateType>::SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position)
    {
        // RE-CREATE THE SFML RECTANGLE FROM THE PROVIDED PARAMETERS.
        SfmlRectangle = RecalculateRectangle(
            center_x_position,
            center_y_position,
            GetWidth(),
            GetHeight());
    }

    /// Moves the rectangle based on the specified vector.
    /// @param[in]  movement - The vector to move the rectangle by.
    template <typename CoordinateType>
    void Rectangle<CoordinateType>::Move(const MATH::Vector2<CoordinateType>& movement)
    {
        SfmlRectangle.left += movement.X;
        SfmlRectangle.top += movement.Y;
    }

    /// Gets the width of the rectangle.
    /// @return The width of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetWidth() const
    {
        return SfmlRectangle.width;
    }

    /// Gets the height of the rectangle.
    /// @return The height of the rectangle.
    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetHeight() const
    {
        return SfmlRectangle.height;
    }

    /// Determines if the specified point is contained in this rectangle.
    /// @param[in]  x_position - The X position to check for containmnet in this rectangle.
    /// @param[in]  y_position - The Y position to check for containmnet in this rectangle.
    /// @return     True if the specified point is within this rectangle; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Contains(const CoordinateType x_position, const CoordinateType y_position) const
    {
        return SfmlRectangle.contains(x_position, y_position);
    }

    /// Checks if the other rectangle intersects any portion of this rectangle.
    /// @param[in]  other_rectangle - The rectangle to test against this rectangle for intersection.
    /// @return True if the rectangles intersect; false otherwise.
    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Intersects(const Rectangle<CoordinateType>& other_rectangle) const
    {
        return SfmlRectangle.intersects(other_rectangle.SfmlRectangle);
    }

    /// Recalculates the rectangle based on the provided information.
    /// @param[in]  center_x_position - The center X position of the rectangle.
    /// @param[in]  center_y_position - The center Y position of the rectangle.
    /// @param[in]  width - The width of the rectangle.
    /// @param[in]  height - The height of the rectangle.
    /// @return     The underlying SFML rectangle based on the provided parameters.
    template <typename CoordinateType>
    sf::Rect<CoordinateType> Rectangle<CoordinateType>::RecalculateRectangle(
        const CoordinateType center_x_position,
        const CoordinateType center_y_position,
        const CoordinateType width,
        const CoordinateType height)
    {
        // CALCULATE THE LEFT COORDINATE.
        const CoordinateType half_width = width / static_cast<CoordinateType>(2);
        const CoordinateType left_x_position = center_x_position - half_width;

        // CALCULATE THE TOP COORDINATE.
        const CoordinateType half_height = height / static_cast<CoordinateType>(2);
        // Y decreases going up on the screen.
        const CoordinateType top_y_position = center_y_position - half_height;

        // CONVERT THE RECTANGLE TO SFML FORMAT.
        sf::Rect<CoordinateType> rectangle = sf::Rect<CoordinateType>(left_x_position, top_y_position, width, height);
        return rectangle;
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
        converted_rectangle.left = static_cast<DestinationCoordinateType>(SfmlRectangle.left);
        converted_rectangle.top = static_cast<DestinationCoordinateType>(SfmlRectangle.top);
        converted_rectangle.width = static_cast<DestinationCoordinateType>(SfmlRectangle.width);
        converted_rectangle.height = static_cast<DestinationCoordinateType>(SfmlRectangle.height);
        return converted_rectangle;
    }
}