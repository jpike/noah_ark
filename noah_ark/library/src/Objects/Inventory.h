#pragma once

#include <memory>
#include <set>
#include "Bible/BibleVerses.h"
#include "Objects/Axe.h"

namespace OBJECTS
{
    /// An inventory of items that have been collected by Noah.
    ///
    /// @todo   The inventory will eventually include animals
    /// and food as well.  It will need to be displayed in
    /// graphical format on the screen.
    /// For now, the plan is always have some minimal inventory GUI
    /// at the top screen:
    ///     [KeyImage] [AxeImage] [WoodImage] x[WoodCount] [KeyImage] INVENTORY
    ///
    /// A user can press a certain key to bring up the main inventory screen,
    /// which will fill the screen, except for the standard stuff described
    /// above (which will still be visible at the top of the screen).
    ///
    /// There will be 3 tabs in the main inventory screen that a user can
    /// switch between: Bible, Animals, and Food.
    ///
    /// The Bible tab will show text for verses in a pane on the left and
    /// a list of verse on the right.
    ///
    /// The Animals tab will show a grid of animal icon images, along with
    /// counts of how many of each animal type have been found.  An animal
    /// can be selected to bring up more detailed information.
    ///
    /// The Food tab will show a grid of food icon images, along with
    /// counts of how many of each food type have been found.  Hover-over-style
    /// tooltips will show slightly more information for each food icon.
    ///
    /// @todo   I'm not sure yet what namespace this should go in.
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
