#include "Graphics/Camera.h"

using namespace GRAPHICS;

Camera::Camera(const float leftViewPosition, const float topViewPosition) :
    m_topLeftViewPositionInPixels(leftViewPosition, topViewPosition)
{}

Camera::~Camera()
{}

float Camera::GetLeftViewPosition() const
{
    return m_topLeftViewPositionInPixels.X;
}

float Camera::GetTopViewPosition() const
{
    return m_topLeftViewPositionInPixels.Y;
}

void Camera::SetTopLeftViewPosition(const MATH::Vector2f& topLeftViewPosition)
{
    m_topLeftViewPositionInPixels = topLeftViewPosition;
}

void Camera::MoveUp(const float distanceInPixels)
{
    m_topLeftViewPositionInPixels.Y -= distanceInPixels;
}

void Camera::MoveDown(const float distanceInPixels)
{
    m_topLeftViewPositionInPixels.Y += distanceInPixels;
}

void Camera::MoveLeft(const float distanceInPixels)
{
    m_topLeftViewPositionInPixels.X -= distanceInPixels;
}

void Camera::MoveRight(const float distanceInPixels)
{
    m_topLeftViewPositionInPixels.X += distanceInPixels;
}