#pragma once

#include <array>
#include <cstddef>
#include <set>
#include "Bible/BibleVerse.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Math/RandomNumberGenerator.h"

namespace STATES
{
    /// A mini-game that can be played during the flood to let players
    /// try and order verses correctly.
    class BibleVerseMiniGame
    {
    public:
        // PUBLIC STATIC CONSTANTS.
        /// The minimum number of verses required for the mini-game.
        static constexpr std::size_t MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME = 3;

        // PUBLIC METHODS.
        void PopulateVersesFrom(const std::set<BIBLE::BibleVerse>* available_verses, MATH::RandomNumberGenerator& random_number_generator);
        void Update(HARDWARE::GamingHardware& gaming_hardware);
        void Render(GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the mini-game is open; false if not.
        bool IsOpen = false;

    private:
        // PRIVATE STATIC CONSTANTS.
        /// The verse number indicating an order has not been set for a verse.
        static constexpr std::size_t UNORDERED_VERSE_NUMBER = 0;

        // PRIVATE ENUMS.
        /// Different substates the mini-game can be in.
        enum class Substate
        {
            /// The user is currently selecting verses.
            SELECTING_VERSES = 0,
            /// The user is currently viewing results after selecting verses.
            VIEWING_RESULTS
        };

        // PRIVATE HELPER METHODS.
        static std::array<BIBLE::BibleVerse, MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME> GetRandomVerses(
            const std::set<BIBLE::BibleVerse>& available_verses,
            MATH::RandomNumberGenerator& random_number_generator);

        // PRIVATE MEMBER VARIABLES.
        /// The full set of verses the mini-game can select from.
        const std::set<BIBLE::BibleVerse>* AvailableVerses = nullptr;
        /// The verses currently being used for the mini-game.
        std::array<BIBLE::BibleVerse, MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME> CurrentVerses = {};
        /// The orders for the current verses (0 = unordered).
        /// Array indices correspond to the order of the current verses.
        std::array<std::size_t, MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME> CurrentVerseOrders = {};
        /// The index of the verse that is currently selected.
        std::size_t CurrentlySelectedVerseIndex = 0;
        /// The next order number that can be chosen (1 is the first order number that can be chosen).
        std::size_t NextOrderNumberThatCanBeEntered = 1;
        /// The current substate of the mini-game.
        Substate CurrentSubstate = Substate::SELECTING_VERSES;
        /// The results message from the current round of the game.
        std::string ResultsMessage = "";
    };
}
