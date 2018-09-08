#include <fstream>
#include <sstream>
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
        try
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
            unsigned int expected_bible_verse_count;
            saved_game_data_file >> expected_bible_verse_count;
            while (saved_game_data->FoundBibleVerses.size() < expected_bible_verse_count)
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
                bool verse_data_valid = !saved_game_data_file.eof();
                if (!verse_data_valid)
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

            // READ IN THE BUILT ARK PIECE DATA.
            unsigned int expected_ark_data_count;
            saved_game_data_file >> expected_ark_data_count;

            // Before reading in the ark piece data, a newline needs to be consumed.
            std::string newline_before_ark_piece_data;
            std::getline(saved_game_data_file, newline_before_ark_piece_data);

            while (saved_game_data->BuildArkPieces.size() < expected_ark_data_count)
            {
                // READ THE NEXT LINE OF DATA.
                std::string current_line_of_data;
                std::getline(saved_game_data_file, current_line_of_data);

                // READ IN THE CURRENT LINE OF ARK PIECE DATA.
                std::stringstream current_ark_data_line(current_line_of_data);
                BuiltArkPieceTileMapData built_ark_piece_data;
                current_ark_data_line >> built_ark_piece_data.TileMapGridXPosition;
                current_ark_data_line >> built_ark_piece_data.TileMapGridYPosition;

                // READ IN ALL BUILT ARK PIECE INDICES.
                while (!current_ark_data_line.eof())
                {
                    // READ IN THE CURRENT ARK PIECE INDEX.
                    unsigned int ark_piece_index;
                    current_ark_data_line >> ark_piece_index;

                    // Make sure the ark piece index is valid.
                    bool ark_piece_index_valid = !current_ark_data_line.eof() && !current_ark_data_line.bad();
                    if (ark_piece_index_valid)
                    {
                        built_ark_piece_data.BuiltArkPieceIndices.push_back(ark_piece_index);
                    }
                    else
                    {
                        break;
                    }
                }

                // MAKE SURE THE READ ARK DATA IS VALID.
                bool ark_data_valid = !saved_game_data_file.eof();
                if (!ark_data_valid)
                {
                    break;
                }

                // ADD THE ARK DATA TO THE IN-MEMORY DATA.
                saved_game_data->BuildArkPieces.push_back(built_ark_piece_data);
            }

            // READ IN THE COLLECTED ANIMAL DATA.
            unsigned int expected_collected_animal_data_count;
            saved_game_data_file >> expected_collected_animal_data_count;

            while (saved_game_data->CollectedAnimals.size() < expected_collected_animal_data_count)
            {
                // READ IN THE CURRENT ANIMAL DATA.
                int species;
                saved_game_data_file >> species;
                int gender;
                saved_game_data_file >> gender;
                unsigned int collected_count;
                saved_game_data_file >> collected_count;

                // MAKE SURE CURRENT ANIMAL DATA WAS PROPERLY READ.
                bool animal_data_valid = !saved_game_data_file.eof();
                if (!animal_data_valid)
                {
                    break;
                }

                // CREATE THE ANIMAL TYPE.
                OBJECTS::AnimalType animal_type(
                    static_cast<OBJECTS::AnimalSpecies>(species),
                    static_cast<OBJECTS::AnimalGender>(gender));

                // ADD THE ANIMAL COUNT TO THE IN-MEMORY DATA.
                saved_game_data->CollectedAnimals[animal_type] = collected_count;
            }

            // READ IN THE COLLECTED FOOD DATA.
            unsigned int expected_collected_food_data_count;
            saved_game_data_file >> expected_collected_food_data_count;

            while (saved_game_data->CollectedFood.size() < expected_collected_food_data_count)
            {
                // READ IN THE CURRENT FOOD DATA.
                int food_id;
                saved_game_data_file >> food_id;
                unsigned int collected_count;
                saved_game_data_file >> collected_count;

                // MAKE SURE CURRENT FOOD DATA WAS PROPERLY READ.
                bool food_data_valid = !saved_game_data_file.eof();
                if (!food_data_valid)
                {
                    break;
                }

                // ADD THE FOOD COUNT TO THE IN-MEMORY DATA.
                OBJECTS::FoodType food_type = static_cast<OBJECTS::FoodType>(food_id);
                saved_game_data->CollectedFood[food_type] = collected_count;
            }

            // RETURN THE LOADED SAVED GAME DATA.
            return saved_game_data;
        }
        catch (const std::exception&)
        {
            // INDICATE THAT THE DATA COULD NOT BE SUCCESSFULLY READ.
            return nullptr;
        }
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
        // The count of verses is written first.
        saved_game_data_file << FoundBibleVerses.size() << std::endl;
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

        // WRITE THE BUILT ARK PIECES.
        // The count of ark piece data is written out first.
        saved_game_data_file << BuildArkPieces.size() << std::endl;
        for (const auto& built_ark_piece_tile_map_data : BuildArkPieces)
        {
            // WRITE THE TILE MAP INDICES.
            saved_game_data_file
                << built_ark_piece_tile_map_data.TileMapGridXPosition
                << SEPARATOR_BETWEEN_RELATED_DATA
                << built_ark_piece_tile_map_data.TileMapGridYPosition
                << SEPARATOR_BETWEEN_RELATED_DATA;

            // WRITE THE BUILT ARK PIECE INDICES.
            for (const auto built_ark_piece_index : built_ark_piece_tile_map_data.BuiltArkPieceIndices)
            {
                saved_game_data_file << built_ark_piece_index << SEPARATOR_BETWEEN_RELATED_DATA;
            }

            // WRITE A LINE SEPARATOR BEFORE THE NEXT SET OF DATA.
            saved_game_data_file << std::endl;
        }

        // WRITE OUT THE COLLECTED ANIMALS.
        // The count of collected animals is written out first.
        saved_game_data_file << CollectedAnimals.size() << std::endl;
        for (const auto& collected_animal_type_and_count : CollectedAnimals)
        {
            // WRITE OUT THE ANIMAL TYPE.
            saved_game_data_file
                << static_cast<int>(collected_animal_type_and_count.first.Species)
                << SEPARATOR_BETWEEN_RELATED_DATA
                << static_cast<int>(collected_animal_type_and_count.first.Gender)
                << SEPARATOR_BETWEEN_RELATED_DATA;

            // WRITE OUT THE COLLECTED COUNT.
            saved_game_data_file << collected_animal_type_and_count.second;

            // WRITE A LINE SEPARATOR BEFORE THE NEXT SET OF DATA.
            saved_game_data_file << std::endl;
        }

        // WRITE OUT THE COLLECTED FOOD.
        // The count of the collected food is written out first.
        saved_game_data_file << CollectedFood.size() << std::endl;
        for (const auto& collected_food_type_and_count : CollectedFood)
        {
            // WRITE OUT THE FOOD TYPE AND COLLECTED COUNT.
            saved_game_data_file
                << static_cast<int>(collected_food_type_and_count.first)
                << SEPARATOR_BETWEEN_RELATED_DATA
                << static_cast<int>(collected_food_type_and_count.second);

            // WRITE A LINE SEPARATOR BEFORE THE NEXT SET OF DATA.
            saved_game_data_file << std::endl;
        }
    }
}