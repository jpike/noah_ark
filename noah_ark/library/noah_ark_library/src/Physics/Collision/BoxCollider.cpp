#include "Physics/Collision/BoxCollider.h"

using namespace PHYSICS::COLLISION;

BoxCollider::BoxCollider(
    const float centerXWorldPositionInPixels,
    const float centerYWorldPositionInPixels,
    const float widthInPixels,
    const float heightInPixels) :
    m_boundingBox(
        centerXWorldPositionInPixels,
        centerYWorldPositionInPixels,
        widthInPixels,
        heightInPixels),
    m_requestedMovement()
{}

BoxCollider::~BoxCollider()
{}

MATH::FloatRectangle BoxCollider::GetBoundingBox() const
{
    return m_boundingBox;
}

MATH::Vector2f BoxCollider::GetCenterPosition() const
{
    // GET THE BOUNDING BOX.
    MATH::FloatRectangle boundingBox = GetBoundingBox();
    
    // GET THE CENTER OF THE BOUNDING BOX.
    MATH::Vector2f centerPosition(
        boundingBox.GetCenterXPosition(),
        boundingBox.GetCenterYPosition());

    return centerPosition;
}

void BoxCollider::SetCenterPosition(const float centerXWorldPositionInPixels, const float centerYPositionInPixels)
{
    m_boundingBox.SetCenterPosition(centerXWorldPositionInPixels, centerYPositionInPixels);
}

void BoxCollider::RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels)
{
    m_requestedMovement.Direction = moveDirection;
    m_requestedMovement.DistanceInPixels = moveDistanceInPixels;
}