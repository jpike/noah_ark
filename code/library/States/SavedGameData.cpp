#include <cassert>
#include <fstream>
#include <sstream>
#include "ErrorHandling/Asserts.h"
#include "ErrorHandling/ErrorMessageBox.h"
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
        default_saved_game_data.Player->SetWorldPosition(OBJECTS::Noah::DEFAULT_START_WORLD_POSITION);

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
                OBJECTS::AnimalSpecies::Value animal_species;
                OBJECTS::AnimalGender::Value animal_gender;
                saved_game_data_file.read(reinterpret_cast<char*>(&animal_species), sizeof(animal_species));
                saved_game_data_file.read(reinterpret_cast<char*>(&animal_gender), sizeof(animal_gender));
                OBJECTS::AnimalType animal_type(animal_species, animal_gender);
                
                RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(animal_type.Species);

                std::shared_ptr<GRAPHICS::AnimatedSprite> animal_sprite = RESOURCES::AnimalGraphics::GetSprite(animal_type);
                ASSERT_THEN_IF(animal_sprite)
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

            // READ IN FOOD COUNTS ON THE ARK.
            saved_game_data_file.read(reinterpret_cast<char*>(saved_game_data->FoodCountsOnArk.data()), food_counts_total_size_in_bytes);

            // READ IN THE FLOOD DAY/TIME INFORMATION.
            float flood_elapsed_time_in_seconds = 0.0f;
            saved_game_data_file.read(reinterpret_cast<char*>(&flood_elapsed_time_in_seconds), sizeof(flood_elapsed_time_in_seconds));
            saved_game_data->FloodElapsedGameplayTime = sf::seconds(flood_elapsed_time_in_seconds);

            // PROVIDE VISIBILITY INTO ANY ERRORS.
            bool error_reading_data = saved_game_data_file.bad() || saved_game_data_file.fail();
            if (error_reading_data)
            {
                ERROR_HANDLING::ErrorMessageBox::Display("Error reading saved game data file.");
            }

            // RETURN THE LOADED SAVED GAME DATA.
            return saved_game_data;
        }
        catch (const std::exception& exception)
        {
            // INDICATE THAT THE DATA COULD NOT BE SUCCESSFULLY READ.
            ERROR_HANDLING::ErrorMessageBox::Display(exception.what());
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

        // WRITE OUT FOOD COUNTS ON THE ARK.
        saved_game_data_file.write(reinterpret_cast<const char*>(FoodCountsOnArk.data()), food_counts_total_size_in_bytes);

        // WRITE OUT THE FLOOD DAY/TIME INFORMATION.
        float flood_elapsed_time_in_seconds = FloodElapsedGameplayTime.asSeconds();
        saved_game_data_file.write(reinterpret_cast<const char*>(&flood_elapsed_time_in_seconds), sizeof(flood_elapsed_time_in_seconds));

        // PROVIDE VISIBILITY INTO ANY ERRORS.
        bool error_writing_data = saved_game_data_file.bad() || saved_game_data_file.fail();
        if (error_writing_data)
        {
            ERROR_HANDLING::ErrorMessageBox::Display("Error writing saved game data file.");
        }
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
            constexpr unsigned int CLEAN_ANIMAL_COUNT_PER_GENDER = 7;
            expected_animal_count = CLEAN_ANIMAL_COUNT_PER_GENDER;
        }
        else
        {
            // 1 pair of each unclean animal are required (1 male + 1 female per pair).
            // See Genesis 7:2.
            constexpr unsigned int UNCLEAN_ANIMAL_COUNT_PER_GENDER = 1;
            expected_animal_count = UNCLEAN_ANIMAL_COUNT_PER_GENDER;
        }

        // DETERMINE IF THE APPROPRIATE NUMBER OF ANIMALS HAVE BEEN COLLECTED.
        const INVENTORY::AnimalCollectionStatistics& gender_collection_statistics = CollectedAnimalsBySpeciesThenGender[animal_type.Species][animal_type.Gender];
        unsigned int actual_animal_count = gender_collection_statistics.CollectedTotalCount();
        bool animal_type_fully_collected = (actual_animal_count >= expected_animal_count);
        return animal_type_fully_collected;
    }

    /// Determines if all animals have been fully collected.
    /// @return True if all animals have been fully collected; false otherwise.
    bool SavedGameData::AnimalsFullyCollected() const
    {
        // CHECK IF EACH ANIMAL SPECIES HAS BEEN COLLECTED.
        for (unsigned int animal_species_id = 0; animal_species_id < OBJECTS::AnimalSpecies::COUNT; ++animal_species_id)
        {
            // CHECK IF MALES OF THE CURRENT SPECIES HAVE BEEN FULLY COLLECTED.
            OBJECTS::AnimalType male_animal_type(static_cast<OBJECTS::AnimalSpecies::Value>(animal_species_id), OBJECTS::AnimalGender::MALE);
            bool males_of_species_fully_collected = AnimalTypeFullyCollected(male_animal_type);
            if (!males_of_species_fully_collected)
            {
                // Return early to avoid wasting time for future animal types.
                return false;
            }

            // CHECK IF FEMALES OF THE CURRENT SPECIES HAVE BEEN FULLY COLLECTED.
            OBJECTS::AnimalType female_animal_type(static_cast<OBJECTS::AnimalSpecies::Value>(animal_species_id), OBJECTS::AnimalGender::FEMALE);
            bool females_of_species_fully_collected = AnimalTypeFullyCollected(female_animal_type);
            if (!females_of_species_fully_collected)
            {
                // Return early to avoid wasting time for future animal types.
                return false;
            }
        }

        // INDICATE THAT ALL ANIMALS HAVE BEEN FULLY COLLECTED.
        // If we didn't return early above, then all animals must have been fully collected.
        return true;
    }

    /// Determines if all foods have been fully collected.
    /// @return True if all foods have been fully collected; false otherwise.
    bool SavedGameData::FoodsFullyCollected() const
    {
        // CHECK IF EACH TYPE OF FOOD HAS BEEN COLLECTED.
        for (unsigned int food_id = OBJECTS::Food::NONE + 1; food_id < OBJECTS::Food::COUNT; ++food_id)
        {
            // As long as a good has been collected at least once, that should be sufficient.
            unsigned int current_food_type_collected_count = Player->Inventory.FoodCounts[food_id];
            bool current_food_type_collected = (current_food_type_collected_count > 0);
            if (!current_food_type_collected)
            {
                // Return early to avoid wasting time for future food types.
                return false;
            }
        }

        // INDICATE THAT ALL FOODS HAVE BEEN FULLY COLLECTED.
        // If we didn't return early above, then all types of foods must have been fully collected.
        return true;
    }

    /// Determines if all family members have been fully collected.
    /// @return True if all family members have been fully collected; false otherwise.
    bool SavedGameData::FamilyMembersFullyCollected() const
    {
        // CHECK IF EACH FAMILY MEMBER HAS BEEN GATHERED.
        for (unsigned int family_member_id = 0; family_member_id < OBJECTS::FamilyMember::COUNT; ++family_member_id)
        {
            bool current_family_member_collected = FamilyMembersGathered[family_member_id];
            if (!current_family_member_collected)
            {
                // Return early to avoid wasting fime for future family members.
                return false;
            }
        }

        // INDICATE THAT ALL FAMILY MEMBERS HAVE BEEN FULLY COLLECTED.
        // If we didn't return early above, then all family members must have been fully collected.
        return true;
    }

    /// Determines if the ark has been fully built.
    /// @return True if the ark has been fully built; false otherwise.
    bool SavedGameData::ArkFullyBuilt() const
    {
        // CHECK IF ALL ARK PIECES HAVE BEEN BUILT.
        // The total ark piece count is hardcoded here for simplicity since it isn't expected to change.
        constexpr std::size_t TOTAL_ARK_PIECE_COUNT = 1326;
        std::size_t actual_built_ark_piece_count = BuiltArkPieces.size();
        bool ark_fully_built = (actual_built_ark_piece_count >= TOTAL_ARK_PIECE_COUNT);
        return ark_fully_built;
    }

    /// Determines if all items required prior to entering into the ark for the flood have been collected.
    /// @return True if all items required from the flood have been collected; false otherwise.
    bool SavedGameData::CollectedAllItemsRequiredBeforeFlood() const
    {
        bool animals_fully_collected = AnimalsFullyCollected();
        bool food_fully_collected = FoodsFullyCollected();
        bool family_members_fully_collected = FamilyMembersFullyCollected();
        bool ark_fully_built = ArkFullyBuilt();
        bool all_required_items_collected = (animals_fully_collected && food_fully_collected && family_members_fully_collected && ark_fully_built);
        return all_required_items_collected;
    }
}
