#include "Collision/BoxCollider.h"

namespace COLLISION
{
    BoxCollider::BoxCollider(
        const float centerXWorldPositionInPixels,
        const float centerYWorldPositionInPixels,
        const float widthInPixels,
        const float heightInPixels) :
        m_worldPositionInPixels(new MATH::Vector2f(centerXWorldPositionInPixels, centerYWorldPositionInPixels)),
        m_boundingBox(MATH::FloatRectangle::FromCenterAndDimensions(
        centerXWorldPositionInPixels,
        centerYWorldPositionInPixels,
        widthInPixels,
        heightInPixels)),
        m_requestedMovement()
    {}

    BoxCollider::~BoxCollider()
    {}

    MATH::FloatRectangle BoxCollider::GetBoundingBox() const
    {
        return m_boundingBox;
    }

    void BoxCollider::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
    {
        m_worldPositionInPixels = positionComponent;

        // Synchronize the bounding box with the new position.
        m_boundingBox.SetCenterPosition(m_worldPositionInPixels->X, m_worldPositionInPixels->Y);
    }

    MATH::Vector2f BoxCollider::GetCenterPosition() const
    {
        return (*m_worldPositionInPixels);
    }

    void BoxCollider::SetCenterPosition(const float centerXWorldPositionInPixels, const float centerYWorldPositionInPixels)
    {
        m_worldPositionInPixels->X = centerXWorldPositionInPixels;
        m_worldPositionInPixels->Y = centerYWorldPositionInPixels;

        // Synchronize the bounding box with the new position.
        m_boundingBox.SetCenterPosition(centerXWorldPositionInPixels, centerYWorldPositionInPixels);
    }

    void BoxCollider::RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels)
    {
        // CREATE A NEW OBJECT FOR THE REQUESTED MOVEMENT.
        m_requestedMovement = std::unique_ptr<Movement>(new Movement());

        // SET THE PARAMETERS OF THE REQUESTED MOVEMENT.
        m_requestedMovement->Direction = moveDirection;
        m_requestedMovement->DistanceInPixels = moveDistanceInPixels;
    }

    std::unique_ptr<Movement> BoxCollider::GetRequestedMovement()
    {
        return std::move(m_requestedMovement);
    }

    void BoxCollider::Move(const MATH::Vector2f& movementVector)
    {
        // MOVE THE POSITION OF THE COLLIDER.
        float newXPosition = m_worldPositionInPixels->X + movementVector.X;
        float newYPosition = m_worldPositionInPixels->Y + movementVector.Y;

        // Ensure that the bounding box stays in sync with the separate world position.
        SetCenterPosition(newXPosition, newYPosition);
    }
}