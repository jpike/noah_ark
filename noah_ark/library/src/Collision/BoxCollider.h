#pragma once

#include "Collision/ICollisionComponent.h"
#include "Math/ITransformable.h"
#include "Math/Vector2.h"

namespace COLLISION
{
    /// A 2D box to define the collidable region of
    /// game objects.
    class BoxCollider : public ICollisionComponent, public MATH::ITransformable
    {
    public:
        /// Constructor.
        /// @param[in]  centerXWorldPositionInPixels - The center X world position of the box (in pixels).
        /// @param[in]  centerYWorldPositionInPixels - The center Y world position of the box (in pixels).
        /// @param[in]  widthInPixels - The width of the box (in pixels).
        /// @param[in]  heightInPixels - The height of the box (in pixels).
        explicit BoxCollider(
            const float centerXWorldPositionInPixels,
            const float centerYWorldPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);
        /// Destructor.
        virtual ~BoxCollider();

        /// @copydoc    ICollisionComponent::GetBoundingBox() const
        virtual MATH::FloatRectangle GetBoundingBox() const;

        /// @copydoc    ITransformable::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent);

        /// Gets the center position of the box.
        /// @return The center position of the bounding box.
        MATH::Vector2f GetCenterPosition() const;

        /// Sets the center position of the box.
        /// @param[in]  centerXWorldPositionInPixels - The center X world position of the box (in pixels).
        /// @param[in]  centerYWorldPositionInPixels - The center Y world position of the box (in pixels).
        void SetCenterPosition(const float centerXWorldPositionInPixels, const float centerYWorldPositionInPixels);

        /// @copydoc    ICollisionComponent::RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels)
        virtual void RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels);

        /// @copydoc    ICollisionComponent::GetRequestedMovement()
        virtual std::unique_ptr<Movement> GetRequestedMovement();

        /// @copydoc    ICollisionComponent::Move(const MATH::Vector2f& movementVector)
        virtual void Move(const MATH::Vector2f& movementVector);

    private:

        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels; ///< The world position component of the collider.  Used for synchronizing with graphics.
        MATH::FloatRectangle m_boundingBox; ///< The bounding box (in world coordinates) for the collider.
        std::unique_ptr<Movement> m_requestedMovement;   ///< Any pending movement requested for the collider.
    };
}