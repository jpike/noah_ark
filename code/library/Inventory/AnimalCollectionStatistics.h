#pragma once

#include <compare>

namespace INVENTORY
{
    /// Statistics about an animal that can be collected in the game.
    /// Statistics are for a single species + gender combination.
    class AnimalCollectionStatistics
    {
    public:
        // OPERATORS.
        auto operator<=>(const AnimalCollectionStatistics&) const = default;

        // METHODS.
        unsigned int CollectedTotalCount() const;
        bool Collected() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The number of animals (of the species + gender) currently following the player.
        unsigned int FollowingPlayerCount = 0;
        /// The number of animals (of the species + gender) collected and in the ark.
        unsigned int InArkCount = 0;
    };
}
