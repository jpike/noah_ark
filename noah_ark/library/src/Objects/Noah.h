#pragma once

#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Graphics/AnimatedSprite.h"
#include "Math/Vector2.h"
#include "Objects/Axe.h"

/// Holds codes for specific objects in the game.
namespace OBJECTS
{
    /// An inventory of items that have been collected by Noah.
    /// @todo   I'm not sure yet what namespace this should go in.
    class Inventory
    {
    public:
        // WOOD.
        /// Adds a single item of wood to the inventory.
        void AddWood()
        {
            ++WoodCount;
        }

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The number of wooden logs in the inventory.
        unsigned int WoodCount = 0;
    };

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

        // AXE SWINGING.
        /// Causes Noah to begin swinging his axe, if he has one and isn't already swinging it.
        /// @return The event describing the axe swing, if an axe swing is started.
        std::shared_ptr<EVENTS::AxeSwingEvent> SwingAxe() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The direction Noah is currently facing.
        CORE::Direction FacingDirection = CORE::Direction::INVALID;
        /// The sprite used for rendering Noah.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The axe that can be swung by Noah.  It is stored as a shared pointer
        /// to allow passing it around as part of an axe swing event.
        std::shared_ptr<OBJECTS::Axe> Axe = std::make_shared<OBJECTS::Axe>();
        /// The inventory of items held by Noah.
        /// @todo   Should the axe go in here?
        OBJECTS::Inventory Inventory = OBJECTS::Inventory();
    };
}