#include <fstream>
#include "States/IntroSequence.h"
#include "States/SavedGameData.h"

namespace STATES
{
    const std::string SavedGameData::DEFAULT_FILENAME = "saved_game.dat";

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

            // A single space exists after the verse number before the verse text,
            // so that needs to be read in order to avoid having an extra space
            // at the beginning of the verse text.
            char space_before_verse_text;
            saved_game_data_file.get(space_before_verse_text);

            // The text may contain spaces, so the remainder of the entire
            // line needs to be read in order to get the full verse text.
            std::string text;
            std::getline(saved_game_data_file, text);

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

    /// Attempts to write saved game data to the specified file.
    /// @param[in]  filepath - The path to the file to write the saved game data.
    void SavedGameData::Write(const std::string& filepath) const
    {
        // OPEN THE FILE.
        std::ofstream saved_game_data_file(filepath);
        bool file_opened = saved_game_data_file.is_open();
        if (!file_opened)
        {
            // The saved game data cannot be written.
            return;
        }

        // WRITE THE PLAYER'S POSITION.
        const char SEPARATOR_BETWEEN_RELATED_DATA = ' ';
        saved_game_data_file
            << PlayerWorldPosition.X
            << SEPARATOR_BETWEEN_RELATED_DATA
            << PlayerWorldPosition.Y
            << std::endl;

        // WRITE PLAYER'S WOOD COUNT.
        saved_game_data_file << WoodCount << std::endl;

        // WRITE THE FOUND BIBLE VERSES.
        for (const auto& current_verse : FoundBibleVerses)
        {
            // WRITE THE CURRENT VERSE.
            saved_game_data_file
                << static_cast<unsigned int>(current_verse.Book)
                << SEPARATOR_BETWEEN_RELATED_DATA
                << current_verse.Chapter
                << SEPARATOR_BETWEEN_RELATED_DATA
                << current_verse.Verse
                << SEPARATOR_BETWEEN_RELATED_DATA
                << current_verse.Text
                << std::endl;
        }
    }
}
