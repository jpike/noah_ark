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
    void Inventory::AddAnimal(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal)
    {
        // ADD THE ANIMAL TO THE GROUP.
        FollowingAnimals.Add(animal);
    }
}
