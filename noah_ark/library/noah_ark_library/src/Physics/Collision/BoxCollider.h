#pragma once

#include "Physics/Collision/ICollisionComponent.h"

namespace PHYSICS
{
namespace COLLISION
{
    ///////////////////////////////////////////////////////////
    /// @brief  A 2D box to define the collidable region of
    ///         game objects.
    ///////////////////////////////////////////////////////////
    class BoxCollider : public ICollisionComponent
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  centerXWorldPositionInPixels - The center X world position of the box (in pixels).
        /// @param[in]  centerYWorldPositionInPixels - The center Y world position of the box (in pixels).
        /// @param[in]  widthInPixels - The width of the box (in pixels).
        /// @param[in]  heightInPixels - The height of the box (in pixels).
        explicit BoxCollider(
            const float centerXWorldPositionInPixels,
            const float centerYWorldPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);
        /// @brief  Destructor.
        virtual ~BoxCollider();

        /// @copydoc    ICollisionComponent::GetBoundingBox() const
        virtual hgeRect GetBoundingBox() const;

    private:

        hgeRect m_boundingBox; ///< The bounding box (in world coordinates) for the collider.
    };
}
}