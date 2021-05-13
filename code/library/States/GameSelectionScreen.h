#pragma once

#include <filesystem>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Memory/Pointers.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The screen for selecting the game to play
    /// (either a saved game file or a new game).
    class GameSelectionScreen
    {
    public:
        /// The max length of the filename for a saved game.
        /// The length is largely arbitrary but currently set to not result in
        /// the filename wrapping onto multiple lines.  One thought was to enforce
        /// an old "8.3" file naming convention, but that was discarded for now
        /// since it could be confusing due to all of the extra width on screen.
        static constexpr std::size_t MAX_SAVED_GAME_FILENAME_LENGTH_IN_CHARACTERS = 30;

        /// The possible substates of the game.
        enum class SubState
        {
            /// The state where all saved game options are being listed.
            LISTING_GAMES,
            /// The state where the player is entering a new saved game name.
            ENTERING_NEW_GAME
        };

        /// The path to the saved games folder.
        static const std::filesystem::path SAVED_GAMES_FOLDER_PATH;

        void Load();
        void LoadSavedGames();

        GameState Update(const HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;

        /// The total time that the title screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;
        /// The current substate.
        SubState CurrentSubState = SubState::LISTING_GAMES;
        /// The current text being entered by the user for a new saved game filename.
        std::string CurrentNewGameFilenameText = "";
        /// The index of the saved game that is currently selected.
        std::size_t SelectedGameIndex = 0;
        /// The saved games available for the player to select.
        std::vector<MEMORY::NonNullSharedPointer<SavedGameData>> SavedGames = {};
    };
}
