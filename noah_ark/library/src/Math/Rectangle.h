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
        /// Creates a rectangle from the top-left coordinates and dimensions.
        /// @param[in]  left_position - The left position of the rectangle.
        /// @param[in]  top_position - The top position of the rectangle.
        /// @param[in]  width - The width of the rectangle.
        /// @param[in]  height - The height of the rectangle.
        /// @return A rectangle based on the provided values.
        static Rectangle FromTopLeftAndDimensions(
            const CoordinateType left_position,
            const CoordinateType top_position,
            const CoordinateType width,
            const CoordinateType height);
        /// Creates a rectangle from center coordinates and dimensions.
        /// @param[in]  center_x_position - The center X position of the rectangle.
        /// @param[in]  center_y_position - The center Y position of the rectangle.
        /// @param[in]  width - The width of the rectangle.
        /// @param[in]  height - The height of the rectangle.
        static Rectangle FromCenterAndDimensions(
            const CoordinateType center_x_position,
            const CoordinateType center_y_position,
            const CoordinateType width,
            const CoordinateType height);
        /// Default constructor to create an invalid rectangle
        /// with 0 for all values.
        explicit Rectangle();
        /// Constructor accepting an SFML rectangle.
        /// @param[in]  sfml_rectangle - The SFML rectangle to use for this rectangle.
        explicit Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle);
        /// Destructor.
        ~Rectangle();

        // OPERATORS.
        /// Equality operator.
        /// @param[in]  rhs_rectangle - The rectangle on the right-hand side of the operator.
        /// @return     True if the rectangles are equal; false otherwise.
        bool operator== (const Rectangle& rhs_rectangle) const;

        // POSITIONING.
        /// @brief  Gets the center X position of the rectangle.
        /// @return The center X position of the rectangle.
        CoordinateType GetCenterXPosition() const;
        /// @brief  Gets the center Y position of the rectangle.
        /// @return The center Y position of the rectangle.
        CoordinateType GetCenterYPosition() const;
        /// Gets the center position of the rectangle.
        /// @return The center position of the rectangle.
        MATH::Vector2<CoordinateType> GetCenterPosition() const;
        /// Gets the left X position of the rectangle.
        /// @return The left X position of the rectangle.
        CoordinateType GetLeftXPosition() const;
        /// Gets the right X position of the rectangle.
        /// @return The right X position of the rectangle.
        CoordinateType GetRightXPosition() const;
        /// Gets the top Y position of the rectangle.
        /// @return The top Y position of the rectangle.
        CoordinateType GetTopYPosition() const;
        /// Gets the bottom Y position of the rectangle.
        /// @return The bottom Y position of the rectangle.
        CoordinateType GetBottomYPosition() const;
        /// Sets the center position of the rectangle.
        /// @param[in]  center_x_position - The center X position of the rectangle.
        /// @param[in]  center_y_position - The center Y position of the rectangle.
        void SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position);

        // MOVEMENT.
        /// Moves the rectangle based on the specified vector.
        /// @param[in]  movement - The vector to move the rectangle by.
        void Move(const MATH::Vector2<CoordinateType>& movement);

        // DIMENSIONS.
        /// Gets the width of the rectangle.
        /// @return The width of the rectangle.
        CoordinateType GetWidth() const;
        /// Gets the height of the rectangle.
        /// @return The height of the rectangle.
        CoordinateType GetHeight() const;

        // COLLISION TESTING.
        /// Determines if the specified point is contained in this rectangle.
        /// @param[in]  x_position - The X position to check for containmnet in this rectangle.
        /// @param[in]  y_position - The Y position to check for containmnet in this rectangle.
        /// @return     True if the specified point is within this rectangle; false otherwise.
        bool Contains(const CoordinateType x_position, const CoordinateType y_position) const;
        /// Checks if the other rectangle intersects any portion of this rectangle.
        /// @param[in]  other_rectangle - The rectangle to test against this rectangle for intersection.
        /// @return True if the rectangles intersect; false otherwise.
        bool Intersects(const Rectangle& other_rectangle) const;

    private:
        // HELPER METHODS.
        /// Recalculates the rectangle based on the provided information.
        /// @param[in]  center_x_position - The center X position of the rectangle.
        /// @param[in]  center_y_position - The center Y position of the rectangle.
        /// @param[in]  width - The width of the rectangle.
        /// @param[in]  height - The height of the rectangle.
        /// @return     The underlying SFML rectangle based on the provided parameters.
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

    template <typename CoordinateType>
    Rectangle<CoordinateType> Rectangle<CoordinateType>::FromTopLeftAndDimensions(
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

    template <typename CoordinateType>
    Rectangle<CoordinateType>::Rectangle() :
    SfmlRectangle()
    {}

    template <typename CoordinateType>
    Rectangle<CoordinateType>::Rectangle(const sf::Rect<CoordinateType>& sfml_rectangle) :
    SfmlRectangle(sfml_rectangle)
    {}

    template <typename CoordinateType>
    Rectangle<CoordinateType>::~Rectangle()
    {}

    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::operator== (const Rectangle<CoordinateType>& rhs_rectangle) const
    {
        bool equal = (SfmlRectangle == rhs_rectangle.SfmlRectangle);
        return equal;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetCenterXPosition() const
    {
        // Calculate the center using the midpoint formula.
        const CoordinateType left_x_position = GetLeftXPosition();
        const CoordinateType right_x_position = GetRightXPosition();
        const CoordinateType horizontal_midpoint = (left_x_position + right_x_position) / static_cast<CoordinateType>(2);
        return horizontal_midpoint;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetCenterYPosition() const
    {
        // Calculate the center using the midpoint formula.
        const CoordinateType top_y_position = GetTopYPosition();
        const CoordinateType bottom_y_position = GetBottomYPosition();
        const CoordinateType vertical_midpoint = (top_y_position + bottom_y_position) / static_cast<CoordinateType>(2);
        return vertical_midpoint;
    }

    template <typename CoordinateType>
    MATH::Vector2<CoordinateType> Rectangle<CoordinateType>::GetCenterPosition() const
    {
        CoordinateType center_x_position = GetCenterXPosition();
        CoordinateType center_y_position = GetCenterYPosition();
        MATH::Vector2<CoordinateType> center_position(center_x_position, center_y_position);
        return center_position;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetLeftXPosition() const
    {
        return SfmlRectangle.left;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetRightXPosition() const
    {
        const CoordinateType right_x_position = SfmlRectangle.left + SfmlRectangle.width;
        return right_x_position;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetTopYPosition() const
    {
        return SfmlRectangle.top;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetBottomYPosition() const
    {
        const CoordinateType bottom_y_position = SfmlRectangle.top + SfmlRectangle.height;
        return bottom_y_position;
    }

    template <typename CoordinateType>
    void Rectangle<CoordinateType>::SetCenterPosition(const CoordinateType center_x_position, const CoordinateType center_y_position)
    {
        // RE-CREATE THE HGE RECTANGLE FROM THE PROVIDED PARAMETERS.
        SfmlRectangle = RecalculateRectangle(
            center_x_position,
            center_y_position,
            GetWidth(),
            GetHeight());
    }

    template <typename CoordinateType>
    void Rectangle<CoordinateType>::Move(const MATH::Vector2<CoordinateType>& movement)
    {
        SfmlRectangle.left += movement.X;
        SfmlRectangle.top += movement.Y;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetWidth() const
    {
        return SfmlRectangle.width;
    }

    template <typename CoordinateType>
    CoordinateType Rectangle<CoordinateType>::GetHeight() const
    {
        return SfmlRectangle.height;
    }

    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Contains(const CoordinateType x_position, const CoordinateType y_position) const
    {
        return SfmlRectangle.contains(x_position, y_position);
    }

    template <typename CoordinateType>
    bool Rectangle<CoordinateType>::Intersects(const Rectangle<CoordinateType>& other_rectangle) const
    {
        return SfmlRectangle.intersects(other_rectangle.SfmlRectangle);
    }

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
}