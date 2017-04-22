#include "Inventory/Inventory.h"

namespace INVENTORY
{
    /// Adds wood to the inventory.
    /// @param[in]  wood_count - The amount of wood to add.
    void Inventory::AddWood(const unsigned int wood_count)
    {
        WoodCount += wood_count;
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

        // COUNT THE COLLECTED ANIMAL.
        ++CollectedAnimalCounts[animal->Type];
    }

    /// Determines all animals of the specified type have been collected.
    /// @param[in]  animal_type - The type of animals to check.
    /// @return True if all animals of the specified type have been collected;
    ///     false otherwise.
    bool Inventory::AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const
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

        // CHECK IF THE TYPE OF ANIMAL HAS BEEN COLLECTED AT ALL.
        const auto collected_animal_type_and_count = CollectedAnimalCounts.find(animal_type);
        bool animal_collected = (CollectedAnimalCounts.cend() != collected_animal_type_and_count);
        if (!animal_collected)
        {
            return false;
        }

        // DETERMINE IF THE APPROPRIATE NUMBER OF ANIMALS HAVE BEEN COLLECTED.
        unsigned int actual_animal_count = collected_animal_type_and_count->second;
        bool animal_type_fully_collected = (actual_animal_count >= expected_animal_count);
        return animal_type_fully_collected;
    }
}
