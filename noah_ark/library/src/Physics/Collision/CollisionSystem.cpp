#include <algorithm>
#include "Physics/Collision/CollisionSystem.h"

using namespace PHYSICS::COLLISION;

CollisionSystem::CollisionSystem() :
    m_collisionComponents(),
    m_overworldMap()
{}

CollisionSystem::~CollisionSystem()
{}

void CollisionSystem::SetOverworldMap(const std::shared_ptr<MAPS::OverworldMap>& overworldMap)
{
    m_overworldMap = overworldMap;
}

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
        /*MATH::Vector2f movementVector = requestedMovement->ToVector();
        currentCollisionComponent->Move(movementVector);*/
        SimulateMovement(*requestedMovement, *currentCollisionComponent);
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

void CollisionSystem::SimulateMovement(const Movement& movement, ICollisionComponent& collisionComponent) const
{
    // SIMULATE MOVEMENT BASED ON THE PARTICULAR DIRECTION.
    switch (movement.Direction)
    {
    case CORE::Direction::UP:
        SimulateUpMovement(movement, collisionComponent);
        break;
    case CORE::Direction::DOWN:
        SimulateDownMovement(movement, collisionComponent);
        break;
    case CORE::Direction::LEFT:
        SimulateLeftMovement(movement, collisionComponent);
        break;
    case CORE::Direction::RIGHT:
        SimulateRightMovement(movement, collisionComponent);
        break;
    case CORE::Direction::INVALID:
        // Intentionally fall through.
    default:
        // No movement can be simulated for an invalid direction.
        return;
    }
}

void CollisionSystem::SimulateUpMovement(const Movement& movement, ICollisionComponent& collisionComponent) const
{
    // VERIFY THAT THE MOVEMENT IS FOR THE 'UP' DIRECTION.
    bool movementForUpDirection = (CORE::Direction::UP == movement.Direction);
    if (!movementForUpDirection)
    {
        // An invalid movement was provided to this method since the direction was not up.
        return;
    }

    // GRADUALLY MOVE THE COLLISION COMPONENT UPWARD UNTIL WE COLLIDE WITH SOMETHING.
    float distanceLeftToMove = movement.DistanceInPixels;
    //auto moreUpMovementToSimulate = [&] ()->bool { (); };
    while (distanceLeftToMove > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE TOP OF THE COLLISION COMPONENT IS ON.
        MATH::FloatRectangle collisionBox = collisionComponent.GetBoundingBox();
        float collisionBoxTopYPosition = collisionBox.GetTopYPosition();
        // Adjust the top position to be a little higher to avoid an infinite loop.
        collisionBoxTopYPosition -= 0.0f;

        // Get the tile for the top-left corner.
        float collisionBoxLeftXPosition = collisionBox.GetLeftXPosition();
        // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        float collisionBoxWidth = collisionBox.GetWidth();
        float horizontalCornerTilesAdjustmentAmount = collisionBoxWidth / 4.0f;
        collisionBoxLeftXPosition += horizontalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> topLeftTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxLeftXPosition, collisionBoxTopYPosition);

        // Get the tile right above the collision box's center.
        float collisionBoxCenterXPosition = collisionBox.GetCenterXPosition();
        std::shared_ptr<MAPS::Tile> topCenterTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxCenterXPosition, collisionBoxTopYPosition);

        // Get the tile for the top-right corner.
        float collisionBoxRightXPosition = collisionBox.GetRightXPosition();
        collisionBoxRightXPosition -= horizontalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> topRightTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxRightXPosition, collisionBoxTopYPosition);

        // MAKE SURE THE TOP TILES EXIST.
        bool topTilesExist = (nullptr != topLeftTile) && (nullptr != topCenterTile) && (nullptr != topRightTile);
        if (!topTilesExist)
        {
            // There aren't any tiles above the collision component to move to.
            return;
        }

        // CHECK IF THE TOP TILES ARE WALKABLE.
        bool topTilesWalkable = (topLeftTile->IsWalkable()) && (topCenterTile->IsWalkable()) && (topRightTile->IsWalkable());
        if (!topTilesWalkable)
        {
            // At least one of the top tiles isn't walkable, so it is blocking any further upward movement.
            return;
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float topTileTopYPosition = topCenterTile->GetTopYPosition();
        float distanceFromTopOfCollisionBoxToTopOfTile = fabs(collisionBoxTopYPosition - topTileTopYPosition);
        bool remainingMovementConfinedToCurrentTile = (distanceLeftToMove <= distanceFromTopOfCollisionBoxToTopOfTile);
        if (remainingMovementConfinedToCurrentTile)
        {
            // Move the collision component the total movement amount since it is confined
            // to a walkable tile.
            // Upward movement is in the negative Y direction.
            collisionComponent.Move(MATH::Vector2f(0.0f, -1.0f * distanceLeftToMove));

            // The entire movement has completed, so return.
            return;
        }
        else
        {
            // Ensure that we move up at least 1 pixel to prevent an infinite loop.
            float distanceToMoveForCurrentTile = std::max(distanceFromTopOfCollisionBoxToTopOfTile, 1.0f);

            // Move the collision component to the top of the current top tile.
            // Upward movement is in the negative Y direction.
            collisionComponent.Move(MATH::Vector2f(0.0f, -1.0f * distanceToMoveForCurrentTile));

            // Track the distance we just moved.
            distanceLeftToMove -= distanceToMoveForCurrentTile;

            // At this point, the loop will continue another iteration.
            // New top tiles should be detected for the new position of the collision component,
            // allowing movement to be simulated for the next set of top tiles.
        }
    }
}

void CollisionSystem::SimulateDownMovement(const Movement& movement, ICollisionComponent& collisionComponent) const
{
    // VERIFY THAT THE MOVEMENT IS FOR THE 'DOWN' DIRECTION.
    bool movementForDownDirection = (CORE::Direction::DOWN == movement.Direction);
    if (!movementForDownDirection)
    {
        // An invalid movement was provided to this method since the direction was not down.
        return;
    }

    // GRADUALLY MOVE THE COLLISION COMPONENT DOWNWARD UNTIL WE COLLIDE WITH SOMETHING.
    float distanceLeftToMove = movement.DistanceInPixels;
    while (distanceLeftToMove > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE BOTTOM OF THE COLLISION COMPONENT IS ON.
        MATH::FloatRectangle collisionBox = collisionComponent.GetBoundingBox();
        float collisionBoxBottomYPosition = collisionBox.GetBottomYPosition();

        // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collisionBoxWidth = collisionBox.GetWidth();
        float horizontalCornerTilesAdjustmentAmount = collisionBoxWidth / 4.0f;

        // Get the tile for the bottom-left corner.
        float collisionBoxLeftXPosition = collisionBox.GetLeftXPosition();
        collisionBoxLeftXPosition += horizontalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> bottomLeftTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxLeftXPosition, collisionBoxBottomYPosition);

        // Get the tile right below the collision box's center.
        float collisionBoxCenterXPosition = collisionBox.GetCenterXPosition();
        std::shared_ptr<MAPS::Tile> bottomCenterTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxCenterXPosition, collisionBoxBottomYPosition);

        // Get the tile for the bottom-right corner.
        float collisionBoxRightXPosition = collisionBox.GetRightXPosition();
        collisionBoxRightXPosition -= horizontalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> bottomRightTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxRightXPosition, collisionBoxBottomYPosition);

        // MAKE SURE THE BOTTOM TILES EXIST.
        bool bottomTilesExist = (nullptr != bottomLeftTile) && (nullptr != bottomCenterTile) && (nullptr != bottomRightTile);
        if (!bottomTilesExist)
        {
            // There aren't any tiles below the collision component to move to.
            return;
        }

        // CHECK IF THE BOTTOM TILES ARE WALKABLE.
        bool bottomTilesWalkable = (bottomLeftTile->IsWalkable()) && (bottomCenterTile->IsWalkable()) && (bottomRightTile->IsWalkable());
        if (!bottomTilesWalkable)
        {
            // At least one of the bottom tiles isn't walkable, so it is blocking any further downward movement.
            return;
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float bottomTileBottomYPosition = bottomCenterTile->GetBottomYPosition();
        float distanceFromBottomOfCollisionBoxToBottomOfTile = fabs(bottomTileBottomYPosition - collisionBoxBottomYPosition);
        bool remainingMovementConfinedToCurrentTile = (distanceLeftToMove <= distanceFromBottomOfCollisionBoxToBottomOfTile);
        if (remainingMovementConfinedToCurrentTile)
        {
            // Move the collision component the total movement amount since it is confined
            // to a walkable tile.
            // Downward movement is in the positive Y direction.
            collisionComponent.Move(MATH::Vector2f(0.0f, distanceLeftToMove));

            // The entire movement has completed, so return.
            return;
        }
        else
        {
            // Ensure that we move down at least 1 pixel to prevent an infinite loop.
            float distanceToMoveForCurrentTile = std::max(distanceFromBottomOfCollisionBoxToBottomOfTile, 1.0f);

            // Move the collision component to the bottom of the current bottom tile.
            // Downward movement is in the positive Y direction.
            collisionComponent.Move(MATH::Vector2f(0.0f, distanceToMoveForCurrentTile));

            // Track the distance we just moved.
            distanceLeftToMove -= distanceToMoveForCurrentTile;

            // At this point, the loop will continue another iteration.
            // New bottom tiles should be detected for the new position of the collision component,
            // allowing movement to be simulated for the next set of bottom tiles.
        }
    }
}

void CollisionSystem::SimulateLeftMovement(const Movement& movement, ICollisionComponent& collisionComponent) const
{
    // VERIFY THAT THE MOVEMENT IS FOR THE 'LEFT' DIRECTION.
    bool movementForLeftDirection = (CORE::Direction::LEFT == movement.Direction);
    if (!movementForLeftDirection)
    {
        // An invalid movement was provided to this method since the direction was not left.
        return;
    }

    // GRADUALLY MOVE THE COLLISION COMPONENT LEFTWARD UNTIL WE COLLIDE WITH SOMETHING.
    float distanceLeftToMove = movement.DistanceInPixels;
    while (distanceLeftToMove > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE LEFT OF THE COLLISION COMPONENT IS ON.
        MATH::FloatRectangle collisionBox = collisionComponent.GetBoundingBox();
        float collisionBoxLeftXPosition = collisionBox.GetLeftXPosition();

        // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collisionBoxHeight = collisionBox.GetHeight();
        float verticalCornerTilesAdjustmentAmount = collisionBoxHeight / 4.0f;

        // Get the tile for the top-left corner.
        float collisionBoxTopYPosition = collisionBox.GetTopYPosition();
        collisionBoxTopYPosition += verticalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> topLeftTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxLeftXPosition, collisionBoxTopYPosition);

        // Get the tile to the direct left of the collision box's center.
        float collisionBoxCenterYPosition = collisionBox.GetCenterYPosition();
        std::shared_ptr<MAPS::Tile> centerLeftTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxLeftXPosition, collisionBoxCenterYPosition);

        // Get the tile for the bottom-left corner.
        float collisionBoxBottomYPosition = collisionBox.GetBottomYPosition();
        collisionBoxBottomYPosition -= verticalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> bottomLeftTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxLeftXPosition, collisionBoxBottomYPosition);

        // MAKE SURE THE LEFT TILES EXIST.
        bool leftTilesExist = (nullptr != topLeftTile) && (nullptr != centerLeftTile) && (nullptr != bottomLeftTile);
        if (!leftTilesExist)
        {
            // There aren't any tiles left of the collision component to move to.
            return;
        }

        // CHECK IF THE LEFT TILES ARE WALKABLE.
        bool leftTilesWalkable = (topLeftTile->IsWalkable()) && (centerLeftTile->IsWalkable()) && (bottomLeftTile->IsWalkable());
        if (!leftTilesWalkable)
        {
            // At least one of the left tiles isn't walkable, so it is blocking any further leftward movement.
            return;
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float leftTileLeftXPosition = centerLeftTile->GetLeftXPosition();
        float distanceFromLeftOfCollisionBoxToLeftOfTile = fabs(collisionBoxLeftXPosition - leftTileLeftXPosition);
        bool remainingMovementConfinedToCurrentTile = (distanceLeftToMove <= distanceFromLeftOfCollisionBoxToLeftOfTile);
        if (remainingMovementConfinedToCurrentTile)
        {
            // Move the collision component the total movement amount since it is confined
            // to a walkable tile.
            collisionComponent.Move(MATH::Vector2f(-1.0f * distanceLeftToMove, 0.0f));

            // The entire movement has completed, so return.
            return;
        }
        else
        {
            // Ensure that we move left at least 1 pixel to prevent an infinite loop.
            float distanceToMoveForCurrentTile = std::max(distanceFromLeftOfCollisionBoxToLeftOfTile, 1.0f);

            // Move the collision component to the left of the current left tile.
            collisionComponent.Move(MATH::Vector2f(-1.0f * distanceToMoveForCurrentTile, 0.0f));

            // Track the distance we just moved.
            distanceLeftToMove -= distanceToMoveForCurrentTile;

            // At this point, the loop will continue another iteration.
            // New left tiles should be detected for the new position of the collision component,
            // allowing movement to be simulated for the next set of left tiles.
        }
    }
}

void CollisionSystem::SimulateRightMovement(const Movement& movement, ICollisionComponent& collisionComponent) const
{
    // VERIFY THAT THE MOVEMENT IS FOR THE 'RIGHT' DIRECTION.
    bool movementForRightDirection = (CORE::Direction::RIGHT == movement.Direction);
    if (!movementForRightDirection)
    {
        // An invalid movement was provided to this method since the direction was not right.
        return;
    }

    // GRADUALLY MOVE THE COLLISION COMPONENT RIGHTWARD UNTIL WE COLLIDE WITH SOMETHING.
    float distanceLeftToMove = movement.DistanceInPixels;
    while (distanceLeftToMove > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE RIGHT OF THE COLLISION COMPONENT IS ON.
        MATH::FloatRectangle collisionBox = collisionComponent.GetBoundingBox();
        float collisionBoxRightXPosition = collisionBox.GetRightXPosition();

        // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collisionBoxHeight = collisionBox.GetHeight();
        float verticalCornerTilesAdjustmentAmount = collisionBoxHeight / 4.0f;

        // Get the tile for the top-right corner.
        float collisionBoxTopYPosition = collisionBox.GetTopYPosition();
        collisionBoxTopYPosition += verticalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> topRightTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxRightXPosition, collisionBoxTopYPosition);

        // Get the tile to the direct right of the collision box's center.
        float collisionBoxCenterYPosition = collisionBox.GetCenterYPosition();
        std::shared_ptr<MAPS::Tile> centerRightTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxRightXPosition, collisionBoxCenterYPosition);

        // Get the tile for the bottom-right corner.
        float collisionBoxBottomYPosition = collisionBox.GetBottomYPosition();
        collisionBoxBottomYPosition -= verticalCornerTilesAdjustmentAmount;
        std::shared_ptr<MAPS::Tile> bottomRightTile = m_overworldMap->GetTileAtWorldPosition(collisionBoxRightXPosition, collisionBoxBottomYPosition);

        // MAKE SURE THE RIGHT TILES EXIST.
        bool rightTilesExist = (nullptr != topRightTile) && (nullptr != centerRightTile) && (nullptr != bottomRightTile);
        if (!rightTilesExist)
        {
            // There aren't any tiles right of the collision component to move to.
            return;
        }

        // CHECK IF THE RIGHT TILES ARE WALKABLE.
        bool rightTilesWalkable = (topRightTile->IsWalkable()) && (centerRightTile->IsWalkable()) && (bottomRightTile->IsWalkable());
        if (!rightTilesWalkable)
        {
            // At least one of the right tiles isn't walkable, so it is blocking any further rightward movement.
            return;
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float rightTileRightXPosition = centerRightTile->GetRightXPosition();
        float distanceFromRightOfCollisionBoxToRightOfTile = fabs(rightTileRightXPosition - collisionBoxRightXPosition);
        bool remainingMovementConfinedToCurrentTile = (distanceLeftToMove <= distanceFromRightOfCollisionBoxToRightOfTile);
        if (remainingMovementConfinedToCurrentTile)
        {
            // Move the collision component the total movement amount since it is confined
            // to a walkable tile.
            collisionComponent.Move(MATH::Vector2f(distanceLeftToMove, 0.0f));

            // The entire movement has completed, so return.
            return;
        }
        else
        {
            // Ensure that we move right at least 1 pixel to prevent an infinite loop.
            float distanceToMoveForCurrentTile = std::max(distanceFromRightOfCollisionBoxToRightOfTile, 1.0f);

            // Move the collision component to the right of the current right tile.
            collisionComponent.Move(MATH::Vector2f(distanceFromRightOfCollisionBoxToRightOfTile, 0.0f));

            // Track the distance we just moved.
            distanceLeftToMove -= distanceToMoveForCurrentTile;

            // At this point, the loop will continue another iteration.
            // New right tiles should be detected for the new position of the collision component,
            // allowing movement to be simulated for the next set of right tiles.
        }
    }
}