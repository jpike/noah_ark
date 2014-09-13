#include <algorithm>
#include "Physics/Collision/CollisionSystem.h"

using namespace PHYSICS::COLLISION;

CollisionSystem::CollisionSystem() :
    m_collisionComponents()
{}

CollisionSystem::~CollisionSystem()
{}

void CollisionSystem::SimulateMovement()
{
    // SIMULATE MOVEMENT FOR ALL COLLISION COMPONENTS.
    for (std::weak_ptr<ICollisionComponent>& collisionComponent : m_collisionComponents)
    {
        // ATTEMPT TO OBTAIN A USABLE COLLISION COMPONENT.
        std::shared_ptr<ICollisionComponent> currentCollisionComponent = collisionComponent.lock();
        bool currentCollisionComponentStillExists = (nullptr != currentCollisionComponent);
        if (!currentCollisionComponentStillExists)
        {
            // Move on to the next collision component.
            continue;
        }

        // SEE IF ANY MOVEMENT WAS REQUEST FOR THE CURRENT COLLISION COMPONENT.
        std::unique_ptr<Movement> requestedMovement = currentCollisionComponent->GetRequestedMovement();
        if (!requestedMovement)
        {
            // Move on to the next collision component.
            continue;
        }

        // SIMULATE MOVEMENT FOR THE COLLISION COMPONENT.
        /// @todo   Collision detection will need to be added here.
        MATH::Vector2f movementVector = requestedMovement->ToVector();
        currentCollisionComponent->Move(movementVector);
    }

    // REMOVE ANY COLLISION COMPONENTS THAT ARE NO LONGER NEEDED.
    RemoveUnusedCollisionComponents();
}

std::shared_ptr<BoxCollider> CollisionSystem::CreateBoxCollider(
    const float centerXWorldPositionInPixels,
    const float centerYWorldPositionInPixels,
    const float widthInPixels,
    const float heightInPixels)
{
    // VALIDATE THE BOX DIMENSIONS.
    // The dimensions are required to be positive to ensure that
    // the box has an actual area.
    bool validWidth = (widthInPixels > 0);
    bool validHeight = (heightInPixels > 0);
    bool validDimensions = (validWidth && validHeight);
    if (!validDimensions)
    {
        // No valid box collider could be created.
        return std::shared_ptr<BoxCollider>();
    }

    // CREATE THE BOX COLLIDER MANAGED BY THIS SYSTEM
    std::shared_ptr<BoxCollider> boxCollider = std::make_shared<BoxCollider>(
        centerXWorldPositionInPixels,
        centerYWorldPositionInPixels,
        widthInPixels,
        heightInPixels);

    m_collisionComponents.push_back(boxCollider);

    return boxCollider;
}

bool CollisionSystem::ManagesCollisionComponent(const std::shared_ptr<ICollisionComponent>& collisionComponent) const
{
    // SEARCH FOR THE COLLISION COMPONENT IN THE CONTAINER.
    // We have to loop over the components manually because there isn't an equality
    // operator directly for weak pointers (as stored in the container).
    auto collisionComponentIterator = m_collisionComponents.cend();
    for (collisionComponentIterator = m_collisionComponents.cbegin();
        collisionComponentIterator != m_collisionComponents.cend();
        ++collisionComponentIterator)
    {
        // Attempt to convert the current collision component to a shared_ptr for comparison.
        const std::shared_ptr<ICollisionComponent> currentCollisionComponent = collisionComponentIterator->lock();

        // Check if the current collision component matches the provided one.
        bool specifiedCollisionComponentFound = (currentCollisionComponent == collisionComponent);
        if (specifiedCollisionComponentFound)
        {
            // Break out of the loop since the specified collision component was found.
            break;
        }
    }

    // DETERMINE IF THE COLLISION COMPONENT WAS FOUND.
    bool collisionComponentFound = (m_collisionComponents.cend() != collisionComponentIterator);
    return collisionComponentFound;
}

void CollisionSystem::RemoveUnusedCollisionComponents()
{
    // Remove any collsion components that are no longer being used.
    m_collisionComponents.remove_if(
        [](std::weak_ptr<ICollisionComponent>& collisionComponent) { return collisionComponent.expired(); });
}