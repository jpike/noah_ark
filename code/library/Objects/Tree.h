#pragma once

#include <optional>
#include <vector>
#include <SFML/System.hpp>
#include "Audio/SoundEffect.h"
#include "Graphics/AnimatedSprite.h"
#include "Math/Rectangle.h"
#include "Objects/Food.h"

namespace OBJECTS
{
    /// A tree that can be placed in the overworld.
    /// Trees will provide a source of wood and potentially fruit.
    class Tree
    {
    public:
        // STATIC CONSTANTS.
        /// The initial number of hit points a tree has.
        /// Lower in debug builds to speed up playtesting.
#if _DEBUG
        static const unsigned int INITIAL_HIT_POINTS = 1;
#else
        static const unsigned int INITIAL_HIT_POINTS = 3;
#endif

        // POSITIONING/BOUNDARIES.
        MATH::FloatRectangle GetWorldBoundingBox() const;
        MATH::Vector2f GetTrunkCenterWorldPosition() const;

        // HITTING.
        void TakeHit();
        void StartShaking();

        // UPDATING.
        void Update(const sf::Time& elapsed_time);

        // OTHER BASIC ACCESSORS.
        bool IsShaking() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the tree.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The sound effect to play when the tree is shaking.
        std::shared_ptr<AUDIO::SoundEffect> TreeShakeSound = nullptr;
        /// The number of hit points the tree currently has.
        unsigned int HitPoints = INITIAL_HIT_POINTS;
        /// Any food that currently exists on the tree.
        std::optional<Food> Food = std::nullopt;
    };
}
