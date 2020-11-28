#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "Bible/BibleVerses.h"
#include "Gameplay/AxeSwingEvent.h"
#include "Gameplay/Direction.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Screen.h"
#include "Graphics/Texture.h"
#include "Inventory/Inventory.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"
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
        /// The name of the animation for walking walking front/down.
        static const std::string WALK_FRONT_ANIMATION_NAME;
        /// The name of the animation for walking walking back/up.
        static const std::string WALK_BACK_ANIMATION_NAME;
        /// The name of the animation for walking walking left.
        static const std::string WALK_LEFT_ANIMATION_NAME;
        /// The name of the animation for walking walking right.
        static const std::string WALK_RIGHT_ANIMATION_NAME;
        /// The default starting world position for Noah.
        /// The center of the screen has been designed to be a good default start position.
        static inline const MATH::Vector2f DEFAULT_START_WORLD_POSITION = MATH::Vector2f(
            static_cast<float>(GRAPHICS::Screen::DEFAULT_WIDTH_IN_PIXELS) / 2.0f,
            static_cast<float>(GRAPHICS::Screen::DEFAULT_HEIGHT_IN_PIXELS) / 2.0f);

        // CONSTRUCTION.
        explicit Noah();

        // POSITIONING/BOUNDARIES.
        MATH::Vector2f GetWorldPosition() const;
        void SetWorldPosition(const MATH::Vector2f& world_position);
        MATH::FloatRectangle GetWorldBoundingBox() const;
        
        // ACTIONS.
        void BeginWalking(const GAMEPLAY::Direction direction, const std::string& walking_animation_name);
        std::shared_ptr<GAMEPLAY::AxeSwingEvent> SwingAxe() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The direction Noah is currently facing.
        GAMEPLAY::Direction FacingDirection = GAMEPLAY::Direction::INVALID;
        /// The sprite used for rendering Noah.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The inventory.
        INVENTORY::Inventory Inventory = {};
    };
}