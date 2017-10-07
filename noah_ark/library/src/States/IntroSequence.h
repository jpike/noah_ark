#pragma once

#include <memory>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Bible/BibleVerse.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Renderer.h"

/// Holds code related to different states in the game.
/// A "state" is a distinct set of functionality or part of game that can be experienced.
/// Different states are useful as different parts of the game program need to
/// behave very differently.
namespace STATES
{
    /// The introductory sequence for the game that plays prior to the
    /// title screen and entering into the main gameplay.
    /// It serves to introduce the basic background story of the game.
    class IntroSequence
    {
    public:
        // STATIC METHODS.
        static const std::vector<BIBLE::BibleVerse>& IntroBibleVerses();

        // OTHER METHODS.
        bool Completed() const;
        void Update(const sf::Time& elapsed_time);
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // STATIC CONSTANTS.
        /// The max time per frame of the intro sequence.
        static const sf::Time MAX_TIME_PER_FRAME;

        // MEMBER VARIABLES.
        /// The index of the current Bible verse being displayed.
        unsigned int CurrentFrameIndex = 0;
        /// The time elapsed for the currently displayed frame of the intro sequence.
        sf::Time ElapsedTimeForCurrentFrame = sf::Time::Zero;
    };
}
