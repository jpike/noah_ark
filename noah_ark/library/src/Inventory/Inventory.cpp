#include "Inventory/Inventory.h"

namespace INVENTORY
{
    /// Adds wood to the inventory.
    /// @param[in]  wood_count - The amount of wood to add.
    void Inventory::AddWood(const unsigned int wood_count)
    {
        WoodCount += wood_count;
    }

    /// Determines all animals of the specified type have been collected.
    /// @param[in]  animal_type - The type of animals to check.
    /// @return True if all animals of the specified type have been collected;
    ///     false otherwise.
    bool Inventory::AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const
    {
        // DETERMINE IF THE ANIMAL IS CLEAN OR NOT.
        bool animal_type_is_clean = animal_type.Clean();
        if (animal_type_is_clean)
        {
            // DETERMINE IF SUFFICIENT ANIMALS OF THE APPROPRIATE GENDER HAVE BEEN COLLECTED.
            // 7 pairs of each clean animal are required (1 male + 1 female per pair).
            // See Genesis 7:2.
            const unsigned int CLEAN_ANIMAL_COUNT_PER_GENDER = 7;
            switch (animal_type.Gender)
            {
                case OBJECTS::AnimalGender::MALE:
                {
                    // CHECK IF ANY MALE ANIMALS OF THE SPECIES HAVE BEEN COLLECTED.
                    const auto& collected_male_animals_of_species = CleanMaleAnimals.find(animal_type.Species);
                    bool male_animals_collected_for_species = (CleanMaleAnimals.cend() != collected_male_animals_of_species);
                    if (!male_animals_collected_for_species)
                    {
                        // If no male animals have been collected for the species,
                        // then they obviously can't have been fully collected.
                        return false;
                    }

                    // CHECK IF A SUFFICIENT NUMBER OF THE ANIMALS HAVE BEEN COLLECTED.
                    unsigned int collected_male_animal_count_of_species = collected_male_animals_of_species->second.size();
                    bool all_male_animals_of_species_collected = (CLEAN_ANIMAL_COUNT_PER_GENDER == collected_male_animal_count_of_species);
                    return all_male_animals_of_species_collected;
                }
                case OBJECTS::AnimalGender::FEMALE:
                {
                    // CHECK IF ANY FEMALE ANIMALS OF THE SPECIES HAVE BEEN COLLECTED.
                    const auto& collected_female_animals_of_species = CleanFemaleAnimals.find(animal_type.Species);
                    bool female_animals_collected_for_species = (CleanFemaleAnimals.cend() != collected_female_animals_of_species);
                    if (!female_animals_collected_for_species)
                    {
                        // If no female animals have been collected for the species,
                        // then they obviously can't have been fully collected.
                        return false;
                    }

                    // CHECK IF A SUFFICIENT NUMBER OF THE ANIMALS HAVE BEEN COLLECTED.
                    unsigned int collected_female_animal_count_of_species = collected_female_animals_of_species->second.size();
                    bool all_female_animals_of_species_collected = (CLEAN_ANIMAL_COUNT_PER_GENDER == collected_female_animal_count_of_species);
                    return all_female_animals_of_species_collected;
                }
                default:
                    return false;
            }
        }
        else
        {
            // DETERMINE IF SUFFICIENT ANIMALS OF THE APPROPRIATE GENDER HAVE BEEN COLLECTED.
            // 1 pair of each unclean animal are required (1 male + 1 female per pair).
            // See Genesis 7:2.
            const unsigned int UNCLEAN_ANIMAL_COUNT_PER_GENDER = 1;
            switch (animal_type.Gender)
            {
                case OBJECTS::AnimalGender::MALE:
                {
                    // CHECK IF ANY MALE ANIMALS OF THE SPECIES HAVE BEEN COLLECTED.
                    const auto& collected_male_animals_of_species = UncleanMaleAnimals.find(animal_type.Species);
                    bool male_animals_collected_for_species = (UncleanMaleAnimals.cend() != collected_male_animals_of_species);
                    if (!male_animals_collected_for_species)
                    {
                        // If no male animals have been collected for the species,
                        // then they obviously can't have been fully collected.
                        return false;
                    }

                    // CHECK IF A SUFFICIENT NUMBER OF THE ANIMALS HAVE BEEN COLLECTED.
                    unsigned int collected_male_animal_count_of_species = collected_male_animals_of_species->second.size();
                    bool all_male_animals_of_species_collected = (UNCLEAN_ANIMAL_COUNT_PER_GENDER == collected_male_animal_count_of_species);
                    return all_male_animals_of_species_collected;
                }
                case OBJECTS::AnimalGender::FEMALE:
                {
                    // CHECK IF ANY FEMALE ANIMALS OF THE SPECIES HAVE BEEN COLLECTED.
                    const auto& collected_female_animals_of_species = UncleanFemaleAnimals.find(animal_type.Species);
                    bool female_animals_collected_for_species = (UncleanFemaleAnimals.cend() != collected_female_animals_of_species);
                    if (!female_animals_collected_for_species)
                    {
                        // If no female animals have been collected for the species,
                        // then they obviously can't have been fully collected.
                        return false;
                    }

                    // CHECK IF A SUFFICIENT NUMBER OF THE ANIMALS HAVE BEEN COLLECTED.
                    unsigned int collected_female_animal_count_of_species = collected_female_animals_of_species->second.size();
                    bool all_female_animals_of_species_collected = (UNCLEAN_ANIMAL_COUNT_PER_GENDER == collected_female_animal_count_of_species);
                    return all_female_animals_of_species_collected;
                }
                default:
                    return false;
            }
        }
    }
}
