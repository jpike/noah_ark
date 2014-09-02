#include "Physics/Collision/BoxCollider.h"

using namespace PHYSICS::COLLISION;

BoxCollider::BoxCollider(
    const float centerXWorldPositionInPixels,
    const float centerYWorldPositionInPixels,
    const float widthInPixels,
    const float heightInPixels) :
    m_boundingBox()
{
    // CALCULATE THE BOUNDING CORNERS OF THE BOX.
    const float halfWidthInPixels = widthInPixels / 2.0f;
    const float leftXWorldPositionInPixels = centerXWorldPositionInPixels - halfWidthInPixels;
    const float rightXWorldPositionInPixels = centerXWorldPositionInPixels + halfWidthInPixels;

    const float halfHeightInPixels = heightInPixels / 2.0f;
    // Y positions decrease going up on the screen.
    const float topYWorldPositionInPixels = centerYWorldPositionInPixels - halfHeightInPixels;
    const float bottomYWorldPositionInPixels = centerYWorldPositionInPixels + halfHeightInPixels;

    // CREATE THE BOUNDING BOX BASED ON THE CALCULATED CORNERS.
    m_boundingBox = hgeRect(
        leftXWorldPositionInPixels, 
        topYWorldPositionInPixels, 
        rightXWorldPositionInPixels,
        bottomYWorldPositionInPixels);
}

BoxCollider::~BoxCollider()
{}

hgeRect BoxCollider::GetBoundingBox() const
{
    return m_boundingBox;
}
