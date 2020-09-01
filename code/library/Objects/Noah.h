#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "Bible/BibleVerses.h"
#include "Gameplay/AxeSwingEvent.h"
#include "Gameplay/Direction.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Texture.h"
#include "Inventory/Inventory.h"
#include "Math/Vector2.h"
#include "Objects/Axe.h"
#include "States/SavedGameData.h"

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

        // CONSTRUCTION.
        explicit Noah(
            const STATES::SavedGameData& saved_game_data,
            const std::shared_ptr<GRAPHICS::Texture>& noah_texture,
            const std::shared_ptr<OBJECTS::Axe>& axe);

        // POSITIONING.
        MATH::Vector2f GetWorldPosition() const;
        void SetWorldPosition(const MATH::Vector2f& world_position);

        // BOUNDING BOX.
        MATH::FloatRectangle GetWorldBoundingBox() const;
        
        // MOVEMENT.
        void BeginWalking(const GAMEPLAY::Direction direction, const std::string& walking_animation_name);

        // AXE SWINGING.
        std::shared_ptr<GAMEPLAY::AxeSwingEvent> SwingAxe() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The direction Noah is currently facing.
        GAMEPLAY::Direction FacingDirection = GAMEPLAY::Direction::INVALID;
        /// The sprite used for rendering Noah.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The inventory of items held by Noah.
        std::shared_ptr<INVENTORY::Inventory> Inventory = nullptr;
    };
}