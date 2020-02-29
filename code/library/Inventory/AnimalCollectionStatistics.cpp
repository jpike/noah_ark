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
}
