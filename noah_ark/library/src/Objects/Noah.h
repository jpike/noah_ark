#pragma once

#include <string>
#include "Events/AxeSwingEvent.h"
#include "Graphics/AnimatedSprite.h"
#include "Objects/Axe.h"
#include "Physics/Collision/BoxCollider.h"

/// @brief  Holds codes for specific objects in the game.
namespace OBJECTS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Represents the character of Noah, typically controlled
    ///         by the player in the game.
    ///////////////////////////////////////////////////////////
    class Noah
    {
    public:
        // STATIC CONSTANTS.
        static const std::string WALK_FRONT_ANIMATION_NAME; ///< The name of the animation for Noah walking facing toward the player (down).
        static const std::string WALK_BACK_ANIMATION_NAME;  ///< The name of the animation for Noah walking facing away from the player (up).
        static const std::string WALK_LEFT_ANIMATION_NAME;  ///< The name of the animation for Noah walking left.
        static const std::string WALK_RIGHT_ANIMATION_NAME; ///< The name of the animation for Noah walking right.

        static const float MOVE_SPEED_IN_PIXELS_PER_SECOND; ///< How fast Noah moves in pixels per second.

        /// @brief  Constructor.
        explicit Noah();
        /// @brief  Destructor.
        ~Noah();

        /// Updates Noah for a frame based on the elapsed amount of time.
        /// @param[in]  elapsed_time_in_seconds - The amount of time to update by.
        void Update(const float elapsed_time_in_seconds);

        /// @brief      Sets the animated sprite used for rendering Noah on screen.
        /// @param[in]  sprite - The sprite to set.
        void SetSprite(const std::shared_ptr<GRAPHICS::AnimatedSprite>& sprite);

        /// @todo   Temporary?
        std::shared_ptr<GRAPHICS::AnimatedSprite> GetSprite() const
        {
            return Sprite;
        }

        /// Sets the axe that can be used by Noah.
        /// @param[in]  axe - The axe to set.
        void SetAxe(const std::shared_ptr<Axe>& axe);

        /// @brief      Sets the box collider used for detecting collisions with Noah.
        /// @param[in]  collider - The collider to set.
        void SetCollider(const std::shared_ptr<PHYSICS::COLLISION::BoxCollider>& collider);

        /// @brief      Gets the world position of Noah.
        /// @return     The world position of Noah, in pixels.
        MATH::Vector2f GetWorldPosition() const;

        /// @brief      Sets the world position of Noah.
        /// @param[in]  xPositionInPixels - The x-coordinate of the Noah in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the Noah in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief      Gets the bounding box of Noah in the world.
        /// @return     The bounding box of Noah.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        /// @brief      Moves Noah up based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveUp(const float elapsedTimeInSeconds);
        /// @brief      Moves Noah down based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveDown(const float elapsedTimeInSeconds);
        /// @brief      Moves Noah left based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveLeft(const float elapsedTimeInSeconds);
        /// @brief      Moves Noah right based on the specified amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        void MoveRight(const float elapsedTimeInSeconds);

        /// @brief  Have Noah stop performing whatever action he may be currently doing
        ///         and enter an idle state (typically standing still).
        void Idle();

        /// Causes Noah to begin swinging his axe, if he has one and isn't already swinging it.
        /// @return The event describing the axe swing, if an axe swing is started.
        std::shared_ptr<EVENTS::AxeSwingEvent> SwingAxe() const;

    private:
        Noah(const Noah&);   ///< Private to disallow copying.
        Noah& operator=(const Noah&);   ///< Private to disallow copying.

        /// @brief      Gets the distance Noah should move for the elapsed time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        /// @return     The distance to move (in pixels) for the given time.
        float GetMoveDistance(const float elapsedTimeInSeconds) const;

        CORE::Direction FacingDirection;    ///< The direction Noah is currently facing.
        std::shared_ptr<GRAPHICS::AnimatedSprite> Sprite; ///< The sprite used for rendering Noah.
        std::shared_ptr<PHYSICS::COLLISION::BoxCollider> Collider; ///< The collider used to colliding with Noah.
        std::shared_ptr<OBJECTS::Axe> Axe;  ///< The axe that can be swung by Noah.
    };
}