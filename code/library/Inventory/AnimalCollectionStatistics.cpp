#include "Inventory/AnimalCollectionStatistics.h"

namespace INVENTORY
{
    /// Checks if these statistics indicate if the animal(s) tracked by these statistics
    /// have been collected or not.
    /// @return True if an animal has been collected according to these statistics; false otherwise.
    bool AnimalCollectionStatistics::Collected() const
    {
        bool animal_collected = (
            MaleFollowingPlayerCount > 0 ||
            FemaleFollowingPlayerCount > 0 ||
            MaleInArkCount > 0 ||
            FemaleInArkCount > 0);
        return animal_collected;
    }

    /// Checks if these statistics indicate if male animal(s) tracked by these statistics
    /// have been collected or not.
    /// @return True if male animals have been collected according to these statistics; false otherwise.
    bool AnimalCollectionStatistics::MaleCollected() const
    {
        bool male_animal_collected = (
            MaleFollowingPlayerCount > 0 ||
            MaleInArkCount > 0);
        return male_animal_collected;
    }

    /// Checks if these statistics indicate if female animal(s) tracked by these statistics
    /// have been collected or not.
    /// @return True if female animals have been collected according to these statistics; false otherwise.
    bool AnimalCollectionStatistics::FemaleCollected() const
    {
        bool female_animal_collected = (
            FemaleFollowingPlayerCount > 0 ||
            FemaleInArkCount > 0);
        return female_animal_collected;
    }
}
