#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Bible/BibleVerse.h"
#include "Graphics/Renderer.h"
#include "States/GameState.h"

namespace STATES
{
    /// The game state for playing a short intro sequence when the player
    /// decides to start a new game (save file) from the beginning.
    class NewGameIntroSequence
    {
    public:
        // METHODS.
        void ResetToBeginning();
        GameState Update(const sf::Time& elapsed_time, AUDIO::Speakers& speakers);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;

    private:
        // STATIC CONSTANTS.
        /// The max time per frame of the intro sequence.
        static const sf::Time MAX_TIME_PER_FRAME;

        // STATIC METHODS.
        static const std::vector<BIBLE::BibleVerse>& BibleVerses();

        // OTHER HELPER METHODS.
        bool Completed() const;

        // MEMBER VARIABLES.
        /// The index of the current Bible verse being displayed.
        unsigned int CurrentFrameIndex = 0;
        /// The time elapsed for the currently displayed frame of the intro sequence.
        sf::Time ElapsedTimeForCurrentFrame = sf::Time::Zero;
    };
}
