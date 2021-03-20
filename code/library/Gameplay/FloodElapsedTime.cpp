#include <cmath>
#include "Gameplay/FloodElapsedTime.h"

namespace GAMEPLAY
{
    /// Gets the current day and hour of the flood based on elapsed gameplay time.
    /// @param[in]  total_flood_elapsed_game_time - The total amount of elapsed gameplay time
    ///     (the sped-up timeframe).
    /// @param[out] day_count - The number of days that have elapsed for the flood.
    /// @param[out] hour_of_current_day - The hour of the current day for the flood.
    void FloodElapsedTime::GetCurrentDayAndHour(
        const sf::Time& total_flood_elapsed_game_time,
        unsigned int& day_count,
        unsigned int& hour_of_current_day)
    {
        // INDICATE THE DAY/HOUR HAVEN'T BEEN COMPUTED YET.
        day_count = 0;
        hour_of_current_day = 0;

        // COMPUTE THE ELAPSED GAMEPLAY TIME IN MINUTES.
        constexpr float SECONDS_PER_MINUTE = 60.0f;
        float flood_elapsed_gameplay_time_in_seconds = total_flood_elapsed_game_time.asSeconds();
        float flood_elapsed_gameplay_time_in_minutes = flood_elapsed_gameplay_time_in_seconds / SECONDS_PER_MINUTE;

        // COMPUTE THE MORE "REAL-WORLD" (NARRATIVE) ELAPSED TIME.
#if _DEBUG
        constexpr float GAMEPLAY_MINUTES_PER_REAL_DAY = 0.5f;
#else
        // For simplicity, 3 minutes is set for a day (1 minute of game time per 8 hours of real time) for non-fast debug builds.
        constexpr float GAMEPLAY_MINUTES_PER_REAL_DAY = 3.0f;
#endif
        float flood_day_and_hour_count = flood_elapsed_gameplay_time_in_minutes / GAMEPLAY_MINUTES_PER_REAL_DAY;

        // SEPARATE THE DAY/HOUR COUNT.
        // The values need to be separated into whole integer (day) and fractional (hour) components.
        float flood_day_count = 0;
        float hour_ratio_for_current_day = std::modf(flood_day_and_hour_count, &flood_day_count);
        day_count = static_cast<unsigned int>(flood_day_count);

        // COMPUTE THE CURRENT HOUR OF THE DAY.
        constexpr float HOURS_PER_DAY = 24.0f;
        hour_of_current_day = static_cast<unsigned int>(hour_ratio_for_current_day * HOURS_PER_DAY);
    }
}
