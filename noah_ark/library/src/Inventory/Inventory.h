#pragma once

#include <memory>
#include <set>
#include "Bible/BibleVerses.h"
#include "Objects/Axe.h"

namespace INVENTORY
{
    /// An inventory of items that have been collected by Noah.
    class Inventory
    {
    public:
        // WOOD.
        /// Adds a single item of wood to the inventory.
        void AddWood()
        {
            ++WoodCount;
        }

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
    };
}
