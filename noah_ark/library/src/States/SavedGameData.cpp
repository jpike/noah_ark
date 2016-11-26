#include <fstream>
#include "States/IntroSequence.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// Creates an instance of the default saved game data
    /// for a newly started game.
    /// @return The default saved game data.
    SavedGameData SavedGameData::DefaultSavedGameData()
    {
        // The exact values are set based on experimentation
        // for what seems best for a new game.
        SavedGameData default_saved_game_data;

        // INITIALIZE THE DEFAULT PLAYER POSITION.
        default_saved_game_data.PlayerWorldPosition.X = 256.0f;
        default_saved_game_data.PlayerWorldPosition.Y = 192.0f;

        // INITIALIZE THE DEFAULT FOUND BIBLE VERSES.
        // Verses from the intro sequence are found by default.
        default_saved_game_data.FoundBibleVerses = IntroSequence::IntroBibleVerses();

        return default_saved_game_data;
    }

    /// Attempts to load saved game data from the specified file.
    /// @param[in]  filepath - The path to the file with saved game data to load.
    /// @return The saved game data loaded from the file, if successful; null otherwise.
    std::unique_ptr<SavedGameData> SavedGameData::Load(const std::string& filepath)
    {
        // OPEN THE FILE.
        std::ifstream saved_game_data_file(filepath);
        bool file_opened = saved_game_data_file.is_open();
        if (!file_opened)
        {
            // The saved game data cannot be loaded.
            return false;
        }

        // CREATE THE SAVED GAME DATA TO BE POPULATED FROM THE FILE.
        auto saved_game_data = std::make_unique<SavedGameData>();

        // READ IN THE PLAYER'S POSITION.
        /// @todo   Test all of this logic.  It hasn't really been tested yet.
        saved_game_data_file >> saved_game_data->PlayerWorldPosition.X;
        saved_game_data_file >> saved_game_data->PlayerWorldPosition.Y;

        // READ IN THE PLAYER'S WOORD COUNT.
        saved_game_data_file >> saved_game_data->WoodCount;

        // READ IN THE FOUND BIBLE VERSES.
        while (!saved_game_data_file.eof())
        {
            // READ IN THE CURRENT BIBLE VERSE'S DATA.
            unsigned int book;
            saved_game_data_file >> book;
            unsigned int chapter;
            saved_game_data_file >> chapter;
            unsigned int verse;
            saved_game_data_file >> verse;
            std::string text;
            saved_game_data_file >> text;

            // MAKE SURE CURRENT VERSE DATA WAS PROPERLY READ.
            if (saved_game_data_file.eof())
            {
                break;
            }

            // CREATE THE VERSE.
            BIBLE::BibleVerse current_verse(
                static_cast<BIBLE::BibleBook>(book),
                chapter,
                verse,
                text);

            // ADD THE VERSE TO THE IN-MEMORY DATA.
            saved_game_data->FoundBibleVerses.push_back(current_verse);
        }

        // RETURN THE LOADED SAVED GAME DATA.
        return saved_game_data;
    }
}
