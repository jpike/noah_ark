#pragma once

#include <string>
#include "Graphics/AnimatedSprite.h"

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

        /// @brief      Sets the animated sprite used for rendering Noah on screen.
        /// @param[in]  sprite - The sprite to set.
        void SetSprite(const std::shared_ptr<GRAPHICS::AnimatedSprite>& sprite);

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

    private:
        Noah(const Noah& noahToCopy);   ///< Private to disallow copying.
        Noah& operator=(const Noah& rhsNoah);   ///< Private to disallow copying.

        /// @brief      Gets the distance Noah should move for the elapsed time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for which to move Noah.
        /// @return     The distance to move (in pixels) for the given time.
        float GetMoveDistance(const float elapsedTimeInSeconds) const;

        std::shared_ptr<GRAPHICS::AnimatedSprite> m_sprite; ///< The sprite used for rendering Noah.
    };
}