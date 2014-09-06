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
        heightInPixels)
    {}

BoxCollider::~BoxCollider()
{}

MATH::FloatRectangle BoxCollider::GetBoundingBox() const
{
    return m_boundingBox;
}
