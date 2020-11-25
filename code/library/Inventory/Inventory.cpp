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
        FoodCounts[food.Type] += OBJECTS::Food::COUNT_PER_INSTANCE;
    }

    /// Adds an animal to the inventory.
    /// @param[in]  animal - The animal to add.
    void Inventory::AddAnimal(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal)
    {
        // ADD THE ANIMAL TO THE GROUP.
        FollowingAnimals.Add(animal);
    }
}
