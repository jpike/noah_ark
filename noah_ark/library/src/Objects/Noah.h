#pragma once

#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Graphics/AnimatedSprite.h"
#include "Math/Vector2.h"
#include "Objects/Axe.h"

/// Holds codes for specific objects in the game.
namespace OBJECTS
{
    /// Represents the character of Noah, typically controlled
    /// by the player in the game.
    class Noah
    {
    public:
        // STATIC CONSTANTS.
        /// How fast Noah can move in pixels per second.
        static const float MOVE_SPEED_IN_PIXELS_PER_SECOND;

        // POSITIONING.
        /// Gets the world position of Noah.
        /// @return The world position of Noah.
        MATH::Vector2f GetWorldPosition() const;
        /// Sets the world position of Noah.
        /// @param[in]  world_position - The new world position to set.
        void SetWorldPosition(const MATH::Vector2f& world_position);

        // BOUNDING BOX.
        /// Gets the bounding box of Noah in the world.
        /// @return The bounding box of Noah.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // MOVEMENT.
        /// Moves Noah up based on the specified amount of time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to move Noah.
        void MoveUp(const float elapsed_time_in_seconds);
        /// Moves Noah down based on the specified amount of time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to move Noah.
        void MoveDown(const float elapsed_time_in_seconds);
        /// Moves Noah left based on the specified amount of time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to move Noah.
        void MoveLeft(const float elapsed_time_in_seconds);
        /// Moves Noah right based on the specified amount of time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to move Noah.
        void MoveRight(const float elapsed_time_in_seconds);

        // AXE SWINGING.
        /// Causes Noah to begin swinging his axe, if he has one and isn't already swinging it.
        /// @return The event describing the axe swing, if an axe swing is started.
        std::shared_ptr<EVENTS::AxeSwingEvent> SwingAxe() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The direction Noah is currently facing.
        CORE::Direction FacingDirection = CORE::Direction::INVALID;
        /// The sprite used for rendering Noah.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The axe that can be swung by Noah.
        /// @todo   Don't initialize to null?
        std::shared_ptr<OBJECTS::Axe> Axe = nullptr;

    private:
        // MOVEMENT.
        /// Gets the distance Noah should move for the elapsed time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to move Noah.
        /// @return The distance to move (in world coordinates/pixels) for the given time.
        float GetMoveDistance(const float elapsed_time_in_seconds) const;
    };

    /*class Noah
    {
    public:
        // STATIC CONSTANTS.
        static const std::string WALK_FRONT_ANIMATION_NAME; ///< The name of the animation for Noah walking facing toward the player (down).
        static const std::string WALK_BACK_ANIMATION_NAME;  ///< The name of the animation for Noah walking facing away from the player (up).
        static const std::string WALK_LEFT_ANIMATION_NAME;  ///< The name of the animation for Noah walking left.
        static const std::string WALK_RIGHT_ANIMATION_NAME; ///< The name of the animation for Noah walking right.

        static const float MOVE_SPEED_IN_PIXELS_PER_SECOND; ///< How fast Noah moves in pixels per second.

        /// Constructor.
        explicit Noah();
        /// Destructor.
        ~Noah();

        /// Updates Noah for a frame based on the elapsed amount of time.
        /// @param[in]  elapsed_time_in_seconds - The amount of time to update by.
        void Update(const float elapsed_time_in_seconds);

        /// Sets the axe that can be used by Noah.
        /// @param[in]  axe - The axe to set.
        void SetAxe(const std::shared_ptr<Axe>& axe);

        /// Sets the box collider used for detecting collisions with Noah.
        /// @param[in]  collider - The collider to set.
        void SetCollider(const std::shared_ptr<COLLISION::BoxCollider>& collider);

        /// Gets the world position of Noah.
        /// @return The world position of Noah, in pixels.
        MATH::Vector2f GetWorldPosition() const;

        /// Sets the world position of Noah.
        /// @param[in]  xPositionInPixels - The x-coordinate of the Noah in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the Noah in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// Gets the bounding box of Noah in the world.
        /// @return The bounding box of Noah.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        /// Moves Noah up based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveUp(const float elapsedTimeInSeconds);
        /// Moves Noah down based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveDown(const float elapsedTimeInSeconds);
        /// Moves Noah left based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveLeft(const float elapsedTimeInSeconds);
        /// Moves Noah right based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveRight(const float elapsedTimeInSeconds);

        /// Have Noah stop performing whatever action he may be currently doing
        /// and enter an idle state (typically standing still).
        void Idle();

        /// Causes Noah to begin swinging his axe, if he has one and isn't already swinging it.
        /// @return The event describing the axe swing, if an axe swing is started.
        std::shared_ptr<EVENTS::AxeSwingEvent> SwingAxe() const;

        
        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        GRAPHICS::AnimatedSprite Sprite; ///< The sprite used for rendering Noah.

    private:
        Noah(const Noah&);   ///< Private to disallow copying.
        Noah& operator=(const Noah&);   ///< Private to disallow copying.

        /// Gets the distance Noah should move for the elapsed time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        /// @return The distance to move (in pixels) for the given time.
        float GetMoveDistance(const float elapsedTimeInSeconds) const;

        CORE::Direction FacingDirection;    ///< The direction Noah is currently facing.
        std::shared_ptr<COLLISION::BoxCollider> Collider; ///< The collider used to colliding with Noah.
        std::shared_ptr<OBJECTS::Axe> Axe;  ///< The axe that can be swung by Noah.
    };*/
}