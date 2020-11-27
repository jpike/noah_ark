#include "Inventory/AnimalCollectionStatistics.h"

namespace INVENTORY
{
    /// Gets the total number of animals collected according to these statistics.
    /// @return The total collected animal count stored in these statistics.
    unsigned int AnimalCollectionStatistics::CollectedTotalCount() const
    {
        unsigned int collected_total_count = FollowingPlayerCount + InArkCount;
        return collected_total_count;
    }

    /// Checks if these statistics indicate if the animal(s) tracked by these statistics
    /// have been collected or not.
    /// @return True if an animal has been collected according to these statistics; false otherwise.
    bool AnimalCollectionStatistics::Collected() const
    {
        unsigned int collected_total_count = CollectedTotalCount();
        bool animal_collected = (collected_total_count > 0);
        return animal_collected;
    }
}
