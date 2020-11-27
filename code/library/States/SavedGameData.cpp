#include <cassert>
#include <fstream>
#include <sstream>
#include "Resources/AnimalGraphics.h"
#include "Resources/AnimalSounds.h"
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

            // READ IN OUT THE COLLECTED BIBLE VERSES.
            std::size_t collected_bible_verse_count = 0;
            saved_game_data_file.read(reinterpret_cast<char*>(&collected_bible_verse_count), sizeof(collected_bible_verse_count));
            for (std::size_t bible_verse_index = 0; bible_verse_index < collected_bible_verse_count; ++bible_verse_index)
            {
                // READ IN THE CURRENT BIBLE VERSE.
                BIBLE::BibleVerse bible_verse;
                saved_game_data_file.read(reinterpret_cast<char*>(&bible_verse.Book), sizeof(bible_verse.Book));
                saved_game_data_file.read(reinterpret_cast<char*>(&bible_verse.Chapter), sizeof(bible_verse.Chapter));
                saved_game_data_file.read(reinterpret_cast<char*>(&bible_verse.Verse), sizeof(bible_verse.Verse));

                // The length of the verse text needs to be read first to know how big the verse text is.
                std::size_t verse_text_length_in_bytes = 0;
                saved_game_data_file.read(reinterpret_cast<char*>(&verse_text_length_in_bytes), sizeof(verse_text_length_in_bytes));
                bible_verse.Text.resize(verse_text_length_in_bytes);
                saved_game_data_file.read(bible_verse.Text.data(), verse_text_length_in_bytes);

                saved_game_data->Player->Inventory.BibleVerses.insert(bible_verse);
            }

            // READ IN THE FOOD COUNTS.
            std::size_t food_counts_total_size_in_bytes = sizeof(unsigned int) * static_cast<std::size_t>(OBJECTS::Food::TypeId::COUNT);
            saved_game_data_file.read(reinterpret_cast<char*>(saved_game_data->Player->Inventory.FoodCounts.data()), food_counts_total_size_in_bytes);

            // READ IN THE FOLLOWING ANIMAL INFORMATION.
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X), sizeof(saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X));
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y), sizeof(saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y));

            std::size_t following_animal_count = 0;
            saved_game_data_file.read(reinterpret_cast<char*>(&following_animal_count), sizeof(following_animal_count));
            saved_game_data->Player->Inventory.FollowingAnimals.Animals.reserve(following_animal_count);

            for (std::size_t animal_index = 0; animal_index < following_animal_count; ++animal_index)
            {
                OBJECTS::AnimalType animal_type;
                saved_game_data_file.read(reinterpret_cast<char*>(&animal_type.Species), sizeof(animal_type.Species));
                saved_game_data_file.read(reinterpret_cast<char*>(&animal_type.Gender), sizeof(animal_type.Gender));
                
                RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(animal_type.Species);

                std::shared_ptr<GRAPHICS::AnimatedSprite> animal_sprite = RESOURCES::AnimalGraphics::GetSprite(animal_type);
                assert(animal_sprite);
                if (animal_sprite)
                {
                    auto animal = MEMORY::NonNullSharedPointer<OBJECTS::Animal>(std::make_shared<OBJECTS::Animal>(
                        animal_type,
                        *animal_sprite,
                        animal_sound_id));
                    saved_game_data->Player->Inventory.FollowingAnimals.Add(animal);
                }
            }

            // READ IN THE BUILT ARK PIECES.
            std::size_t built_ark_piece_count = 0;
            saved_game_data_file.read(reinterpret_cast<char*>(&built_ark_piece_count), sizeof(built_ark_piece_count));
            saved_game_data->BuiltArkPieces.reserve(built_ark_piece_count);
            for (std::size_t ark_piece_index = 0; ark_piece_index < built_ark_piece_count; ++ark_piece_index)
            {
                unsigned int ark_piece_id = 0;
                saved_game_data_file.read(reinterpret_cast<char*>(&ark_piece_id), sizeof(ark_piece_id));

                OBJECTS::ArkPiece ark_piece(ark_piece_id);
                saved_game_data_file.read(reinterpret_cast<char*>(&ark_piece.Sprite.WorldPosition.X), sizeof(ark_piece.Sprite.WorldPosition.X));
                saved_game_data_file.read(reinterpret_cast<char*>(&ark_piece.Sprite.WorldPosition.Y), sizeof(ark_piece.Sprite.WorldPosition.Y));
                saved_game_data_file.read(reinterpret_cast<char*>(&ark_piece.Built), sizeof(ark_piece.Built));
                saved_game_data_file.read(reinterpret_cast<char*>(&ark_piece.IsExternalDoorway), sizeof(ark_piece.IsExternalDoorway));

                saved_game_data->BuiltArkPieces.emplace_back(ark_piece);
            }

            // READ IN THE COLLECTED ANIMAL STATISTICS.
            std::size_t animal_statistics_total_count = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT) * static_cast<std::size_t>(OBJECTS::AnimalGender::COUNT);
            std::size_t collected_animal_statistics_total_size_in_bytes = sizeof(INVENTORY::AnimalCollectionStatistics) * animal_statistics_total_count;
            saved_game_data_file.read(reinterpret_cast<char*>(saved_game_data->CollectedAnimalsBySpeciesThenGender.data()), collected_animal_statistics_total_size_in_bytes);

            // INITIALIZE THE BIBLE VERSES LEFT TO FIND.
            std::set_difference(
                BIBLE::BIBLE_VERSES.cbegin(),
                BIBLE::BIBLE_VERSES.cend(),
                saved_game_data->Player->Inventory.BibleVerses.cbegin(),
                saved_game_data->Player->Inventory.BibleVerses.cend(),
                std::inserter(saved_game_data->BibleVersesLeftToFind, saved_game_data->BibleVersesLeftToFind.begin()));

            // READ IN FAMILY COLLECTION STATISTICS.
            std::size_t family_member_statistics_total_size_in_bytes = sizeof(bool) * saved_game_data->FamilyMembersGathered.size();
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->FamilyMembersGathered), family_member_statistics_total_size_in_bytes);

            // READ IN THE FLOOD DAY COUNT.
            saved_game_data_file.read(reinterpret_cast<char*>(&saved_game_data->FloodDayCount), sizeof(saved_game_data->FloodDayCount));

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

        // WRITE THE PLAYER'S WOOD COUNT.
        saved_game_data_file.write(reinterpret_cast<const char*>(&Player->Inventory.WoodCount), sizeof(Player->Inventory.WoodCount));

        // WRITE OUT THE COLLECTED BIBLE VERSES.
        std::size_t collected_bible_verse_count = Player->Inventory.BibleVerses.size();
        saved_game_data_file.write(reinterpret_cast<const char*>(&collected_bible_verse_count), sizeof(collected_bible_verse_count));
        for (const auto& bible_verse : Player->Inventory.BibleVerses)
        {
            // WRITE OUT THE CURRENT BIBLE VERSE.
            saved_game_data_file.write(reinterpret_cast<const char*>(&bible_verse.Book), sizeof(bible_verse.Book));
            saved_game_data_file.write(reinterpret_cast<const char*>(&bible_verse.Chapter), sizeof(bible_verse.Chapter));
            saved_game_data_file.write(reinterpret_cast<const char*>(&bible_verse.Verse), sizeof(bible_verse.Verse));

            // The length of the verse text needs to be written out to know where the verse text ends.
            std::size_t verse_text_length_in_bytes = bible_verse.Text.length();
            saved_game_data_file.write(reinterpret_cast<const char*>(&verse_text_length_in_bytes), sizeof(verse_text_length_in_bytes));
            saved_game_data_file.write(bible_verse.Text.data(), verse_text_length_in_bytes);
        }

        // WRITE OUT THE FOOD COUNTS.
        std::size_t food_counts_total_size_in_bytes = sizeof(unsigned int) * static_cast<std::size_t>(OBJECTS::Food::TypeId::COUNT);
        saved_game_data_file.write(reinterpret_cast<const char*>(Player->Inventory.FoodCounts.data()), food_counts_total_size_in_bytes);

        // WRITE OUT THE FOLLOWING ANIMAL INFORMATION.
        saved_game_data_file.write(reinterpret_cast<const char*>(&Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X), sizeof(Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X));
        saved_game_data_file.write(reinterpret_cast<const char*>(&Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y), sizeof(Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y));

        std::size_t following_animal_count = Player->Inventory.FollowingAnimals.Animals.size();
        saved_game_data_file.write(reinterpret_cast<const char*>(&following_animal_count), sizeof(following_animal_count));
        for (const auto& animal : Player->Inventory.FollowingAnimals.Animals)
        {
            saved_game_data_file.write(reinterpret_cast<const char*>(&animal->Type.Species), sizeof(animal->Type.Species));
            saved_game_data_file.write(reinterpret_cast<const char*>(&animal->Type.Gender), sizeof(animal->Type.Gender));
        }

        // WRITE OUT THE BUILT ARK PIECES.
        std::size_t built_ark_piece_count = BuiltArkPieces.size();
        saved_game_data_file.write(reinterpret_cast<const char*>(&built_ark_piece_count), sizeof(built_ark_piece_count));
        for (const auto& ark_piece : BuiltArkPieces)
        {
            saved_game_data_file.write(reinterpret_cast<const char*>(&ark_piece.Id), sizeof(ark_piece.Id));
            saved_game_data_file.write(reinterpret_cast<const char*>(&ark_piece.Sprite.WorldPosition.X), sizeof(ark_piece.Sprite.WorldPosition.X));
            saved_game_data_file.write(reinterpret_cast<const char*>(&ark_piece.Sprite.WorldPosition.Y), sizeof(ark_piece.Sprite.WorldPosition.Y));
            saved_game_data_file.write(reinterpret_cast<const char*>(&ark_piece.Built), sizeof(ark_piece.Built));
            saved_game_data_file.write(reinterpret_cast<const char*>(&ark_piece.IsExternalDoorway), sizeof(ark_piece.IsExternalDoorway));
        }

        // WRITE OUT THE COLLECTED ANIMAL STATISTICS.
        std::size_t animal_statistics_total_count = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT) * static_cast<std::size_t>(OBJECTS::AnimalGender::COUNT);
        std::size_t collected_animal_statistics_total_size_in_bytes = sizeof(INVENTORY::AnimalCollectionStatistics) * animal_statistics_total_count;
        saved_game_data_file.write(reinterpret_cast<const char*>(CollectedAnimalsBySpeciesThenGender.data()), collected_animal_statistics_total_size_in_bytes);

        // WRITE OUT FAMILY COLLECTION STATISTICS.
        std::size_t family_member_statistics_total_size_in_bytes = sizeof(bool) * FamilyMembersGathered.size();
        saved_game_data_file.write(reinterpret_cast<const char*>(&FamilyMembersGathered), family_member_statistics_total_size_in_bytes);

        // WRITE OUT THE FLOOD DAY COUNT.
        saved_game_data_file.write(reinterpret_cast<const char*>(&FloodDayCount), sizeof(FloodDayCount));

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
