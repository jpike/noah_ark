#include <array>
#include <ctime>
#include "Graphics/TimeOfDayLighting.h"

namespace GRAPHICS
{
    /// Computes lighting according to the time of day.
    /// @param[in,out]  time_of_day_shader - The shader to update with computed values.
    void TimeOfDayLighting::Compute(sf::Shader& time_of_day_shader)
    {
        // GET THE CURRENT COLOR SCALE BASED ON TIME-OF-DAY.
            // If an error occurs getting the current time of day,
            // the normal (maximum) color values will be used
            // (the time-of-day shading feature just won't exist for those users).
        float time_of_day_color_scale = 1.0f;
        std::time_t* const JUST_GET_RETURNED_TIME = nullptr;
        std::time_t current_posix_time = std::time(JUST_GET_RETURNED_TIME);
        std::tm current_time;
        errno_t get_local_time_return_code = localtime_s(&current_time, &current_posix_time);
        const errno_t GET_LOCAL_TIME_SUCCESS_RETURN_CODE = 0;
        bool current_time_retrieved_successfully = (GET_LOCAL_TIME_SUCCESS_RETURN_CODE == get_local_time_return_code);
        if (current_time_retrieved_successfully)
        {
            // 0.4f is the darkest we can go and still have the screen
            // remain reasonably visible.
            const std::size_t HOUR_COUNT_PER_DAY = 24;
            std::array<float, HOUR_COUNT_PER_DAY> HOUR_TO_COLOR_SCALE_LOOKUP =
            {
                0.45f, // 12am
                0.4f, // 1am (darkest time)
                0.45f, // 2am
                0.50f, // 3am
                0.55f, // 4am
                0.60f, // 5am
                0.65f, // 6am
                0.70f, // 7am
                0.75f, // 8am
                0.80f, // 9am
                0.85f, // 10am
                0.90f, // 11am
                0.95f, // 12pm
                1.0f, // 1pm (brightest time)
                0.95f, // 2pm
                0.90f, // 3pm
                0.85f, // 4pm
                0.80f, // 5pm
                0.75f, // 6pm
                0.70f, // 7pm
                0.65f, // 8pm
                0.60f, // 9pm
                0.55f, // 10pm
                0.50f, // 11pm
            };

            time_of_day_color_scale = HOUR_TO_COLOR_SCALE_LOOKUP[current_time.tm_hour];
        }

        // CONFIGURE THE SHADER.
        time_of_day_shader.setUniform("color_scale", time_of_day_color_scale);
        time_of_day_shader.setUniform("texture", sf::Shader::CurrentTexture);
    }
}
