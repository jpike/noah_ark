#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Math/RandomNumberGenerator.h"
#include "States/GameState.h"

namespace STATES
{
    /// The cutscene that plays once the ark is complete.
    /// It shows the flood waters rising and serves to transition the gameplay
    /// from the overworld area to being confined within the ark.
    class FloodCutscene
    {
    public:
        // PUBLIC METHODS.
        GameState Update(const sf::Time& elapsed_time);
        sf::Sprite Render(GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The total time that the credits screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;

    private:
        // HELPER METHODS.
        bool Completed() const;

        // PRIVATE MEMBER VARIABLES.
        /// A random number generator.
        MATH::RandomNumberGenerator RandomNumberGenerator = {};
    };
}
