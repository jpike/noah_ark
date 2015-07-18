#include <cmath>
#include "Math/FloatRectangle.h"

namespace MATH
{
    FloatRectangle FloatRectangle::FromTopLeftAndDimensions(
        const float left_position,
        const float top_position,
        const float width,
        const float height)
    {
        MATH::FloatRectangle rectangle;

        rectangle.m_rectangle.left = left_position;
        rectangle.m_rectangle.top = top_position;
        rectangle.m_rectangle.width = width;
        rectangle.m_rectangle.height = height;

        return rectangle;
    }

    FloatRectangle::FloatRectangle(
        const float centerWorldXPositionInPixels,
        const float centerWorldYPositionInPixels,
        const float widthInPixels,
        const float heightInPixels) :
        m_rectangle()
    {
        // CREATE THE HGE RECTANGLE FROM THE PROVIDED PARAMETERS.
        m_rectangle = RecalculateRectangle(
            centerWorldXPositionInPixels,
            centerWorldYPositionInPixels,
            widthInPixels,
            heightInPixels);
    }

    FloatRectangle::FloatRectangle(const sf::FloatRect& sfmlRectangle) :
        m_rectangle(sfmlRectangle)
    {}

    FloatRectangle::~FloatRectangle()
    {}

    float FloatRectangle::GetCenterXPosition() const
    {
        // Calculate the center using the midpoint formula.
        const float leftXPosition = GetLeftXPosition();
        const float rightXPosition = GetRightXPosition();
        const float horizontalMidpoint = (leftXPosition + rightXPosition) / 2.0f;
        return horizontalMidpoint;
    }

    float FloatRectangle::GetCenterYPosition() const
    {
        // Calculate the center using the midpoint formula.
        const float topYPosition = GetTopYPosition();
        const float bottomYPosition = GetBottomYPosition();
        const float verticalMidpoint = (topYPosition + bottomYPosition) / 2.0f;
        return verticalMidpoint;
    }

    MATH::Vector2f FloatRectangle::GetCenterPosition() const
    {
        float centerXPosition = GetCenterXPosition();
        float centerYPosition = GetCenterYPosition();
        MATH::Vector2f centerPosition(centerXPosition, centerYPosition);
        return centerPosition;
    }

    float FloatRectangle::GetLeftXPosition() const
    {
        return m_rectangle.left;
    }

    float FloatRectangle::GetRightXPosition() const
    {
        const float rightXPosition = m_rectangle.left + m_rectangle.width;
        return rightXPosition;
    }

    float FloatRectangle::GetTopYPosition() const
    {
        return m_rectangle.top;
    }

    float FloatRectangle::GetBottomYPosition() const
    {
        const float bottomYPosition = m_rectangle.top + m_rectangle.height;
        return bottomYPosition;
    }

    void FloatRectangle::SetCenterPosition(const float centerWorldXPositionInPixels, const float centerWorldYPositionInPixels)
    {
        // RE-CREATE THE HGE RECTANGLE FROM THE PROVIDED PARAMETERS.
        m_rectangle = RecalculateRectangle(
            centerWorldXPositionInPixels,
            centerWorldYPositionInPixels,
            GetWidth(),
            GetHeight());
    }

    float FloatRectangle::GetWidth() const
    {
        return m_rectangle.width;
    }

    float FloatRectangle::GetHeight() const
    {
        return m_rectangle.height;
    }

    bool FloatRectangle::Contains(const float worldXPositionInPixels, const float worldYPositionInPixels) const
    {
        return m_rectangle.contains(worldXPositionInPixels, worldYPositionInPixels);
    }

    bool FloatRectangle::Intersects(const MATH::FloatRectangle& other_rectangle) const
    {
        return m_rectangle.intersects(other_rectangle.m_rectangle);
    }

    sf::FloatRect FloatRectangle::RecalculateRectangle(
        const float centerWorldXPositionInPixels,
        const float centerWorldYPositionInPixels,
        const float widthInPixels,
        const float heightInPixels)
    {
        // CALCULATE THE LEFT COORDINATE.
        const float halfWidth = widthInPixels / 2.0f;
        const float leftXPosition = centerWorldXPositionInPixels - halfWidth;

        // CALCULATE THE TOP COORDINATE.
        const float halfHeight = heightInPixels / 2.0f;
        // Y decreases going up on the screen.
        const float topYPosition = centerWorldYPositionInPixels - halfHeight;

        // CONVERT THE RECTANGLE TO SFML FORMAT.
        sf::FloatRect rectangle = sf::FloatRect(leftXPosition, topYPosition, widthInPixels, heightInPixels);
        return rectangle;
    }
}
