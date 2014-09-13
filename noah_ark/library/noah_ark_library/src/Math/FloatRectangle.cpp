#include <cmath>
#include "Math/FloatRectangle.h"

using namespace MATH;


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

FloatRectangle::FloatRectangle(const hgeRect& hgeRectangle) :
    m_rectangle(hgeRectangle)
{}

FloatRectangle::~FloatRectangle()
{}

float FloatRectangle::GetCenterXPosition() const
{
    // Calculate the center using the midpoint formula.
    const float horizontalMidpoint = (m_rectangle.x1 + m_rectangle.x2) / 2.0f;
    return horizontalMidpoint;
}

float FloatRectangle::GetCenterYPosition() const
{
    // Calculate the center using the midpoint formula.
    const float verticalMidpoint = (m_rectangle.y1 + m_rectangle.y2) / 2.0f;
    return verticalMidpoint;
}

float FloatRectangle::GetLeftXPosition() const
{
    return m_rectangle.x1;
}

float FloatRectangle::GetRightXPosition() const
{
    return m_rectangle.x2;
}

float FloatRectangle::GetTopYPosition() const
{
    return m_rectangle.y1;
}

float FloatRectangle::GetBottomYPosition() const
{
    return m_rectangle.y2;
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
    const float width = fabs(m_rectangle.x2 - m_rectangle.x1);
    return width;
}

float FloatRectangle::GetHeight() const
{
    const float height = fabs(m_rectangle.y2 - m_rectangle.y1);
    return height;
}

hgeRect FloatRectangle::RecalculateRectangle(
    const float centerWorldXPositionInPixels,
    const float centerWorldYPositionInPixels,
    const float widthInPixels,
    const float heightInPixels)
{
    // CALCULATE THE LEFT COORDINATE.
    const float halfWidth = widthInPixels / 2.0f;
    const float leftXPosition = centerWorldXPositionInPixels - halfWidth;

    // CALCULATE THE RIGHT COORDINATE.
    const float rightXPosition = centerWorldXPositionInPixels + halfWidth;

    // CALCULATE THE TOP COORDINATE.
    const float halfHeight = heightInPixels / 2.0f;
    // Y decreases going up on the screen.
    const float topYPosition = centerWorldYPositionInPixels - halfHeight;

    // CALCULATE THE BOTTOM COORDINATE.
    const float bottomYPosition = centerWorldYPositionInPixels + halfHeight;

    // CONVERT THE RECTANGLE TO HGE FORMAT.
    hgeRect rectangle = hgeRect(leftXPosition, topYPosition, rightXPosition, bottomYPosition);
    return rectangle;
}