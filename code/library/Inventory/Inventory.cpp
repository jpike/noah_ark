#include "Inventory/Inventory.h"

namespace INVENTORY
{
    /// Adds wood to the inventory.
    /// @param[in]  wood_count - The amount of wood to add.
    void Inventory::AddWood(const unsigned int wood_count)
    {
        WoodCount += wood_count;
    }

    /// Adds food to the inventory.
    /// @param[in]  food - The food to add.
    void Inventory::AddFood(const OBJECTS::Food& food)
    {
        CollectedFoodCounts[food.Type] += OBJECTS::Food::COUNT_PER_INSTANCE;
    }

    /// Gets the amount of food in the inventory of the given type.
    /// @param[in]  food_type - The type of food to get the collected count for.
    /// @return The amount of food of the specified type in the inventory.
    unsigned int Inventory::GetCollectedFoodCount(const OBJECTS::FoodType food_type) const
    {
        // CHECK IF THE FOOD TYPE HAS BEEN COLLECTED.
        auto collected_food_type_and_count = CollectedFoodCounts.find(food_type);
        bool food_type_collected = (CollectedFoodCounts.cend() != collected_food_type_and_count);
        if (food_type_collected)
        {
            // RETURN THE COLLECTED COUNT.
            return collected_food_type_and_count->second;
        }
        else
        {
            // INDICATE THAT THE FOOD HASN'T BEEN COLLECTED.
            return 0;
        }
    }

    /// Adds an animal to the inventory.
    /// @param[in]  animal - The animal to add.
    void Inventory::AddAnimal(const std::shared_ptr<OBJECTS::Animal>& animal)
    {
        // MAKE SURE THE ANIMAL EXISTS.
        bool animal_exists = (nullptr != animal);
        if (!animal_exists)
        {
            return;
        }

        // ADD THE ANIMAL TO THE GROUP.
        FollowingAnimals.Add(animal);
    }

    /// Gets the number of collected animals, per gender, for a given species.
    /// @param[in]  species - The species for which to get collected animal counts.
    /// @param[out] species_male_animal_collected_count - The count of collected male
    ///     animals of the species.
    /// @param[out] species_female_animal_collected_count - The count of collected female
    ///     animals of the species.
    void Inventory::GetAnimalCollectedCount(
        const OBJECTS::AnimalSpecies species,
        unsigned int& species_male_animal_collected_count,
        unsigned int& species_female_animal_collected_count) const
    {
        // INDICATE THAT COLLECTED COUNTS HAVEN'T BEEN DETERMINED YET.
        species_male_animal_collected_count = 0;
        species_female_animal_collected_count = 0;

        // GET THE COLLECTION COUNT FOR MALE ANIMALS OF THE SPECIES.
        species;
#if TODO
        OBJECTS::AnimalType male_animal_type(species, OBJECTS::AnimalGender::MALE);
        const auto male_animal_collected_count = CollectedAnimalCounts.find(male_animal_type);
        bool male_animals_collected = (CollectedAnimalCounts.cend() != male_animal_collected_count);
        if (male_animals_collected)
        {
            species_male_animal_collected_count = male_animal_collected_count->second;
        }

        // GET THE COLLECTION COUNT FOR FEMALE ANIMALS OF THE SPECIES.
        OBJECTS::AnimalType female_animal_type(species, OBJECTS::AnimalGender::FEMALE);
        const auto female_animal_collected_count = CollectedAnimalCounts.find(female_animal_type);
        bool female_animals_collected = (CollectedAnimalCounts.cend() != female_animal_collected_count);
        if (female_animals_collected)
        {
            species_female_animal_collected_count = female_animal_collected_count->second;
        }
#endif
    }

    /// Gets the collection statistics for the specified animal species.
    /// @param[in]  species - The animal species for which to get statistics.
    /// @return The collection statistics for the animal species.
    AnimalCollectionStatistics Inventory::GetAnimalCollectionStatistics(const OBJECTS::AnimalSpecies species) const
    {
        AnimalCollectionStatistics collection_statistics;

        GetAnimalCollectedCount(species, collection_statistics.MaleFollowingPlayerCount, collection_statistics.FemaleFollowingPlayerCount);

        /// @todo   Get ark counts...does that even make sense to be the "inventory"?

        return collection_statistics;
    }
}
