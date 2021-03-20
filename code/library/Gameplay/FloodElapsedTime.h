#pragma once

#include <SFML/System.hpp>

namespace GAMEPLAY
{
    /// The elapsed time for the flood.
    ///
    /// The timeline for the flood is as follows:
    /// - Noah's age = 600 - 2nd month, 17th day - Flood starts (Genesis 7:11)
    /// - 40 days/nights - Flood continues (Genesis 7:12, Genesis 7:17)
    /// - 150 days - Water prevails (Genesis 7:24, Genesis 8:3)
    /// - 7th month, 17th day (5 months later) - Ark comes to rest on mountains of Ararat (Genesis 8:4)
    /// - 10th month, 1st day - Waters recede to point where mountaintops seen (Genesis 8:5)
    /// - +40 days - Noah sends raven + dove out (Genesis 8:6-7)
    /// - +7 days - Noah sends dove out again (Genesis 8:10)
    /// - +7 days - Noah sends dove out again; it doesn't return (Genesis 8:12)
    /// - 601st year of Noah's life - 1st month, 1st day - Noah removes covering of ark (Genesis 8:13)
    /// - 601st year of Noah's life - 2nd month, 27th day - Earth dried out, so exit from the ark (Genesis 8:14)
    ///
    /// That's effectively 1 year + 10 days.  Although exact number of "days" in a year could have varied back then,
    /// this game will assume 365 days in a year according to modern calendar.  That means 375 days total on the ark.
    ///
    /// That's too long for this game in terms of real gameplay time, so "game time" is sped up so that 1 day takes 3 minutes.
    /// "Game time" still results in a decent amount of real-world time in order to still communicate the feeling of the
    /// flood taking a long time.
    class FloodElapsedTime
    {
    public:
        static void GetCurrentDayAndHour(
            const sf::Time& total_flood_elapsed_game_time,
            unsigned int& day_count,
            unsigned int& hour_of_current_day);
    };
}
