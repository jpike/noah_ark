#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include "Bible/BibleVerses.h"
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
        bool AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const;
        void GetAnimalCollectedCount(
            const OBJECTS::AnimalSpecies species, 
            unsigned int& species_male_animal_collected_count, 
            unsigned int& species_female_animal_collected_count) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The axe that can be swung.  It is stored as a shared pointer
        /// to allow passing it around as part of an axe swing event.
        std::shared_ptr<OBJECTS::Axe> Axe = std::make_shared<OBJECTS::Axe>();
        /// The number of wooden logs in the inventory.
        unsigned int WoodCount = 0;
        /// Bible verses collected so far by the player.
        /// A set is used to allow easily determining
        /// if a verse is already in the inventory.
        std::set<BIBLE::BibleVerse> BibleVerses = std::set<BIBLE::BibleVerse>();
        /// Counts of collected food by type.
        std::unordered_map<OBJECTS::FoodType, unsigned int> CollectedFoodCounts = {};
        /// Counts of collected animals by type.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> CollectedAnimalCounts = {};
    };
}