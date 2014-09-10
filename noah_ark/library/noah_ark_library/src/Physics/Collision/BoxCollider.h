#pragma once

#include "Math/Vector2.h"
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
        virtual MATH::FloatRectangle GetBoundingBox() const;

        /// @brief  Gets the center position of the box.
        /// @return The center position of the bounding box.
        MATH::Vector2f GetCenterPosition() const;

        /// @brief      Sets the center position of the box.
        /// @param[in]  centerXWorldPositionInPixels - The center X world position of the box (in pixels).
        /// @param[in]  centerYWorldPositionInPixels - The center Y world position of the box (in pixels).
        void SetCenterPosition(const float centerXWorldPositionInPixels, const float centerYPositionInPixels);

        /// @copydoc    ICollisionComponent::RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels)
        virtual void RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels);

    private:

        MATH::FloatRectangle m_boundingBox; ///< The bounding box (in world coordinates) for the collider.
        Movement m_requestedMovement;   ///< Any pending movement requested for the collider.
    };
}
}