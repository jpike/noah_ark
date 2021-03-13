#pragma once

#include <SFML/System.hpp>

namespace GAMEPLAY
{
    /// The elapsed time for the flood.
    /// The flood lasts about 150 days (Genesis 8:3) before subsiding (maybe more, depending on if
    /// the initial 40 days/nights of rain overlap or not).  For simplicity for this game, we'll
    /// assume 150 days.  That's still too long for this game in terms of real gameplay time,
    /// so "game time" is sped up so that 1 day takes 12 minutes.  "Game time" still results
    /// in a decent amount of real-world time in order to still communicate the feeling of the
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
