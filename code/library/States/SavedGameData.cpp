#include <fstream>
#include <sstream>
#include "States/IntroSequence.h"
#include "States/SavedGameData.h"

namespace STATES
{
    const std::string SavedGameData::DEFAULT_FILEPATH = "SavedGames/saved_game.dat";

    /// Creates an instance of the default saved game data
    /// for a newly started game.
    /// @return The default saved game data.
    SavedGameData SavedGameData::DefaultSavedGameData()
    {
        // The exact values are set based on experimentation
        // for what seems best for a new game.
        SavedGameData default_saved_game_data;
        default_saved_game_data.Filepath = DEFAULT_FILEPATH;

        // INITIALIZE THE DEFAULT PLAYER POSITION.
        default_saved_game_data.Player->SetWorldPosition(MATH::Vector2f(256.0f, 192.0f));

        // INITIALIZE THE DEFAULT FOUND BIBLE VERSES.
        // Verses from the intro sequence are found by default.
        const auto& intro_bible_verses = IntroSequence::IntroBibleVerses();
        default_saved_game_data.Player->Inventory.BibleVerses = std::set<BIBLE::BibleVerse>(
            intro_bible_verses.cbegin(),
            intro_bible_verses.cend());

        // INITIALIZE THE BIBLE VERSES LEFT TO FIND.
        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            default_saved_game_data.Player->Inventory.BibleVerses.cbegin(),
            default_saved_game_data.Player->Inventory.BibleVerses.cend(),
            std::inserter(default_saved_game_data.BibleVersesLeftToFind, default_saved_game_data.BibleVersesLeftToFind.begin()));

        return default_saved_game_data;
    }

    /// Attempts to load saved game data from the specified file.
    /// @param[in]  filepath - The path to the file with saved game data to load.
    /// @return The saved game data loaded from the file, if successful; null otherwise.
    std::unique_ptr<SavedGameData> SavedGameData::Load(const std::filesystem::path& filepath)
    {
        try
        {
            // ENSURE APPROPRIATE DIRECTORIES EXIST.
            std::filesystem::path parent_directory_path = filepath.parent_path();
            std::filesystem::create_directories(parent_directory_path);

            // OPEN THE FILE.
            std::ifstream saved_game_data_file(filepath, std::ios::binary | std::ios::in);
            bool file_opened = saved_game_data_file.is_open();
            if (!file_opened)
            {
                // The saved game data cannot be loaded.
                return nullptr;
            }

            // CREATE THE SAVED GAME DATA TO BE POPULATED FROM THE FILE.
            auto saved_game_data = std::make_unique<SavedGameData>();
            saved_game_data->Filepath = filepath;

            // READ IN  THE CURRENT GAME STATE.
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->CurrentGameState), sizeof(saved_game_data->CurrentGameState));

            // READ IN  THE PLAYER'S POSITION.
            MATH::Vector2f player_world_position;
            saved_game_data_file.read(reinterpret_cast<char*>(&player_world_position.X), sizeof(player_world_position.X));
            saved_game_data_file.read(reinterpret_cast<char*>(&player_world_position.Y), sizeof(player_world_position.Y));
            saved_game_data->Player->SetWorldPosition(player_world_position);

            // READ IN PLAYER'S WOOD COUNT.
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->Player->Inventory.WoodCount), sizeof(saved_game_data->Player->Inventory.WoodCount));

            // READ IN THE FOOD COUNTS.
            std::size_t food_counts_total_size_in_bytes = sizeof(unsigned int) * static_cast<std::size_t>(OBJECTS::Food::TypeId::COUNT);
            saved_game_data_file.read(reinterpret_cast<char*>(saved_game_data->Player->Inventory.FoodCounts.data()), food_counts_total_size_in_bytes);

            /// @todo   Rest of inventory.

            /// @todo   Built ark pieces.

            // READ IN  THE COLLECTED ANIMAL STATISTICS.
            std::size_t animal_statistics_total_count = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT) * static_cast<std::size_t>(OBJECTS::AnimalGender::COUNT);
            std::size_t collected_animal_statistics_total_size_in_bytes = sizeof(INVENTORY::AnimalCollectionStatistics) * animal_statistics_total_count;
            saved_game_data_file.read(reinterpret_cast<char*>(saved_game_data->CollectedAnimalsBySpeciesThenGender.data()), collected_animal_statistics_total_size_in_bytes);

            /// @todo   Bible verses.

            // READ IN FAMILY COLLECTION STATISTICS.
            std::size_t family_member_statistics_total_size_in_bytes = sizeof(bool) * saved_game_data->FamilyMembersGathered.size();
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->FamilyMembersGathered), family_member_statistics_total_size_in_bytes);

            // READ IN THE FLOOD DAY COUNT.
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->FloodDayCount), saved_game_data->FloodDayCount);

            /// @todo   More error handling?

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
    void SavedGameData::Write(const std::filesystem::path& filepath) const
    {
        // ENSURE APPROPRIATE DIRECTORIES EXIST.
        std::filesystem::path parent_directory_path = std::filesystem::path(filepath).parent_path();
        std::filesystem::create_directories(parent_directory_path);

        // OPEN THE FILE.
        // See https://en.cppreference.com/w/cpp/io/basic_filebuf/open for flags.
        std::ofstream saved_game_data_file(filepath, std::ios::binary | std::ios::out);
        bool file_opened = saved_game_data_file.is_open();
        if (!file_opened)
        {
            // The saved game data cannot be written.
            return;
        }

        // WRITE THE CURRENT GAME STATE.
        saved_game_data_file.write(reinterpret_cast<const char*>(&CurrentGameState), sizeof(CurrentGameState));

        // WRITE THE PLAYER'S POSITION.
        MATH::Vector2f player_world_position = Player->GetWorldPosition();
        saved_game_data_file.write(reinterpret_cast<const char*>(&player_world_position.X), sizeof(player_world_position.X));
        saved_game_data_file.write(reinterpret_cast<const char*>(&player_world_position.Y), sizeof(player_world_position.Y));

        // WRITE PLAYER'S WOOD COUNT.
        saved_game_data_file.write(reinterpret_cast<const char*>(&Player->Inventory.WoodCount), sizeof(Player->Inventory.WoodCount));

        // WRITE OUT THE FOOD COUNTS.
        std::size_t food_counts_total_size_in_bytes = sizeof(unsigned int) * static_cast<std::size_t>(OBJECTS::Food::TypeId::COUNT);
        saved_game_data_file.write(reinterpret_cast<const char*>(Player->Inventory.FoodCounts.data()), food_counts_total_size_in_bytes);

        /// @todo   Rest of inventory.

        /// @todo   Built ark pieces.

        // WRITE OUT THE COLLECTED ANIMAL STATISTICS.
        std::size_t animal_statistics_total_count = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT) * static_cast<std::size_t>(OBJECTS::AnimalGender::COUNT);
        std::size_t collected_animal_statistics_total_size_in_bytes = sizeof(INVENTORY::AnimalCollectionStatistics) * animal_statistics_total_count;
        saved_game_data_file.write(reinterpret_cast<const char*>(CollectedAnimalsBySpeciesThenGender.data()), collected_animal_statistics_total_size_in_bytes);

        /// @todo   Bible verses.

        // WRITE OUT FAMILY COLLECTION STATISTICS.
        std::size_t family_member_statistics_total_size_in_bytes = sizeof(bool) * FamilyMembersGathered.size();
        saved_game_data_file.write(reinterpret_cast<const char*>(&FamilyMembersGathered), family_member_statistics_total_size_in_bytes);

        // WRITE OUT THE FLOOD DAY COUNT.
        saved_game_data_file.write(reinterpret_cast<const char*>(&FloodDayCount), FloodDayCount);

        /// @todo   More error handling?
    }

    /// Gets the total number of animals collected for the given species.
    /// @param[in]  species - The animal species for which to get statistics.
    /// @return The number of animals collected for the species.
    unsigned int SavedGameData::GetCollectedAnimalCount(const OBJECTS::AnimalSpecies::Value species) const
    {
        const INVENTORY::AnimalCollectionStatistics& male_collection_statistics = CollectedAnimalsBySpeciesThenGender[species][OBJECTS::AnimalGender::MALE];
        unsigned int male_collected_animal_count = male_collection_statistics.CollectedTotalCount();

        const INVENTORY::AnimalCollectionStatistics& female_collection_statistics = CollectedAnimalsBySpeciesThenGender[species][OBJECTS::AnimalGender::FEMALE];
        unsigned int female_collected_animal_count = female_collection_statistics.CollectedTotalCount();
        
        unsigned int collected_animal_count = male_collected_animal_count + female_collected_animal_count;
        return collected_animal_count;
    }

    /// Determines if a given animal species has been collected at all.
    /// @param[in]  species - The animal species to check.
    /// @return True if the animal species has been collected at all; false otherwise.
    bool SavedGameData::AnimalSpeciesCollectedAtAll(const OBJECTS::AnimalSpecies::Value species) const
    {
        unsigned int collected_animal_count = GetCollectedAnimalCount(species);
        bool animal_species_collected = (collected_animal_count > 0);
        return animal_species_collected;
    }

    /// Determines all animals of the specified type have been collected.
    /// @param[in]  animal_type - The type of animals to check.
    /// @return True if all animals of the specified type have been collected; false otherwise.
    bool SavedGameData::AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const
    {
        // DETERMINE HOW MANY ANIMALS ARE EXPECTED BASED ON IF THE ANIMAL IS CLEAN OR NOT.
        unsigned int expected_animal_count = 0;
        bool animal_type_is_clean = animal_type.Clean();
        if (animal_type_is_clean)
        {
            // 7 pairs of each clean animal are required (1 male + 1 female per pair).
            // See Genesis 7:2.
            const unsigned int CLEAN_ANIMAL_COUNT_PER_GENDER = 7;
            expected_animal_count = CLEAN_ANIMAL_COUNT_PER_GENDER;
        }
        else
        {
            // 1 pair of each unclean animal are required (1 male + 1 female per pair).
            // See Genesis 7:2.
            const unsigned int UNCLEAN_ANIMAL_COUNT_PER_GENDER = 1;
            expected_animal_count = UNCLEAN_ANIMAL_COUNT_PER_GENDER;
        }

        // DETERMINE IF THE APPROPRIATE NUMBER OF ANIMALS HAVE BEEN COLLECTED.
        unsigned int actual_animal_count = GetCollectedAnimalCount(animal_type.Species);
        bool animal_type_fully_collected = (actual_animal_count >= expected_animal_count);
        return animal_type_fully_collected;
    }
}
