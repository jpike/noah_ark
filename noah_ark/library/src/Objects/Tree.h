#pragma once

#include <optional>
#include <vector>
#include <SFML/System.hpp>
#include "Audio/SoundEffect.h"
#include "Graphics/Sprite.h"
#include "Math/Rectangle.h"
#include "Objects/Food.h"

namespace OBJECTS
{
    /// Defines a sub-rotation that is used to partially shake a tree.
    /// Multiple sub-rotations may be stringed together to form a complete shake.
    struct TreeShakeSubRotation
    {
        /// The signed speed in which the tree should rotate during
        /// this sub-rotation of the shaking.  Should move the tree's
        /// rotation angle toward the destination rotation angle.
        /// Each time the tree's shaking is updated, the tree's rotation
        /// angle will be added to by this velocity.
        float RotationVelocityInDegreesPerSecond = 0.0f;
        /// The final destination rotation angle in degrees for this
        /// sub-rotation of the tree's shake.  May be positive or
        /// negative.  When the rotation angle of the tree reaches
        /// or exceeds this angle, the sub-rotation is defined as
        /// to have stopped.
        float DestinationRotationAngleInDegrees = 0.0f;
    };

    /// A tree that can be placed in the overworld.
    /// Trees will provide a source of wood and potentially fruit.
    class Tree
    {
    public:
        // STATIC CONSTANTS.
        /// The initial number of hit points a tree has.
        static const unsigned int INITIAL_HIT_POINTS = 3;

        // POSITIONING/BOUNDARIES.
        MATH::FloatRectangle GetWorldBoundingBox() const;
        MATH::Vector2f GetTrunkCenterWorldPosition() const;

        // HITTING.
        void TakeHit();
        void StartShaking();

        // UPDATING.
        void Update(const sf::Time& elapsed_time);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the tree.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
        /// The sound effect to play when the tree is shaking.
        std::shared_ptr<AUDIO::SoundEffect> TreeShakeSound = nullptr;
        /// The number of hit points the tree currently has.
        unsigned int HitPoints = INITIAL_HIT_POINTS;
        /// Any food that currently exists on the tree.
        std::optional<Food> Food = std::nullopt;
        /// True if the tree is currently shaking; false otherwise.
        bool Shaking = false;

    private:
        /// The current rotation angle for the tree's sprite, if the tree is currently shaking.
        float CurrentShakeRotationInDegrees = 0.0f;
        /// The index of the current sub-rotation for shaking the tree, if the tree is currently
        /// shaking.
        unsigned int CurrentShakeSubRotationIndex = 0;
        /// The sequence of sub-rotations to use for shaking the tree, if the tree should currently
        /// be shaking.
        std::vector<TreeShakeSubRotation> ShakeSubRotations = std::vector<TreeShakeSubRotation>();
    };
}
