#pragma once

namespace INVENTORY
{
    /// Statistics about an animal that can be collected in the game.
    class AnimalCollectionStatistics
    {
    public:
        // METHODS.
        bool Collected() const;
        bool MaleCollected() const;
        bool FemaleCollected() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The number of male animals collected and following the player.
        unsigned int MaleFollowingPlayerCount = 0;
        /// The number of female animals collected and following the player.
        unsigned int FemaleFollowingPlayerCount = 0;
        /// The number of male animals collected and in the ark.
        unsigned int MaleInArkCount = 0;
        /// The number of female animals collected and in the ark.
        unsigned int FemaleInArkCount = 0;
    };
}
