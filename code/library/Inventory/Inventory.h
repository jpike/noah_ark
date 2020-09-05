#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include "Bible/BibleVerses.h"
#include "Gameplay/FollowingAnimalGroup.h"
#include "Inventory/AnimalCollectionStatistics.h"
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
        unsigned int GetCollectedFoodCount(const OBJECTS::FoodType food_type) const;
        void AddAnimal(const std::shared_ptr<OBJECTS::Animal>& animal);
        void GetAnimalCollectedCount(
            const OBJECTS::AnimalSpecies species, 
            unsigned int& species_male_animal_collected_count, 
            unsigned int& species_female_animal_collected_count) const;
        AnimalCollectionStatistics GetAnimalCollectionStatistics(const OBJECTS::AnimalSpecies species) const;

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
        /// Counts of collected food by type.
        std::unordered_map<OBJECTS::FoodType, unsigned int> CollectedFoodCounts = {};
        /// The group of animals currently following being Noah (as opposed to those in the ark).
        GAMEPLAY::FollowingAnimalGroup FollowingAnimals = {};
    };
}
