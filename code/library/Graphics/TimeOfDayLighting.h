#pragma once

#include <SFML/Graphics.hpp>

namespace GRAPHICS
{
    /// Computes lighting based on the time of day.
    class TimeOfDayLighting
    {
    public:
        static void Compute(sf::Shader& time_of_day_shader);
        static void Compute(const unsigned int hour_of_day, sf::Shader& time_of_day_shader);
    };
}
