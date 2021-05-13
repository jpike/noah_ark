#pragma once

#include <SFML/System.hpp>
#include "Gameplay/FloodSpecialDayAction.h"
#include "Graphics/AnimatedSprite.h"
#include "Math/Vector2.h"
#include "Objects/Animal.h"
#include "Resources/AssetId.h"

namespace OBJECTS
{
    /// A bird sent out from the ark near the end of the flood.
    /// This class exists separately from the generic animal class in order to encapsulate
    /// special logic for this part of the game.  It is generic for a "bird" to re-use
    /// nearly identical code for the raven and the dove.
    class BirdSentFromArk
    {
    public:
        // CONSTRUCTION.
        explicit BirdSentFromArk(const GAMEPLAY::FloodSpecialDayAction action_that_sent_out_bird, const AnimalSpecies::Value species);
        
        // OTHER METHODS.
        void Update(const sf::Time& elapsed_time);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the bird is moving out of the ark versus returning.
        bool MovingOut = true;
        /// The action that sent the bird out.
        GAMEPLAY::FloodSpecialDayAction ActionThatSentOutBird = GAMEPLAY::FloodSpecialDayAction::NONE;
        /// The type of bird (raven or dove).  Arbitrarily set to one of the bird types.
        AnimalSpecies::Value Species = AnimalSpecies::RAVEN;
        /// The sprite for the bird.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The sound for the bird.
        RESOURCES::AssetId SoundId = static_cast<RESOURCES::AssetId>(0);
        /// The movement direction for the bird.
        MATH::Vector2f MoveDirection = MATH::Vector2f();
    };
}
