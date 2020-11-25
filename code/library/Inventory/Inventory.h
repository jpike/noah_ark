#pragma once

#include <array>
#include <memory>
#include <set>
#include "Bible/BibleVerses.h"
#include "Gameplay/FollowingAnimalGroup.h"
#include "Inventory/AnimalCollectionStatistics.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"
#include "Objects/Axe.h"
#include "Objects/Food.h"

/// Holds code related to the player's inventory of items in the game.
namespace INVENTORY
{
    /// An inventory of items that have been collected by Noah.
    class Inventory
    {
    public:
        // PUBLIC METHODS.
        void AddWood(const unsigned int wood_count);
        void AddFood(const OBJECTS::Food& food);
        void AddAnimal(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The axe.
        /// Needs to be a shared pointer for sharing with axe swing events.
        MEMORY::NonNullSharedPointer<OBJECTS::Axe> Axe = MEMORY::NonNullSharedPointer<OBJECTS::Axe>(std::make_shared<OBJECTS::Axe>());
        /// The number of wooden logs in the inventory.
        unsigned int WoodCount = 0;
        /// Bible verses collected so far by the player.
        /// A set is used to allow easily determining
        /// if a verse is already in the inventory.
        std::set<BIBLE::BibleVerse> BibleVerses = std::set<BIBLE::BibleVerse>();
        /// Counts of collected food (currently in the inventory) indexed by type.
        std::array<unsigned int, OBJECTS::Food::TypeId::COUNT> FoodCounts = {};
        /// The group of animals currently following being Noah (as opposed to those in the ark).
        GAMEPLAY::FollowingAnimalGroup FollowingAnimals = {};
    };
}
