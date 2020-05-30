#include <SFML/Graphics.hpp>
#include "States/FloodCutscene.h"

namespace STATES
{
    /// Update the cutscene based on an elapsed amount of time.
    /// @param[in]  elapsed_time - The amount of elapsed time since the last update of the cutscene.
    /// @return The state that the game should be in after the update.
    GameState FloodCutscene::Update(const sf::Time& elapsed_time)
    {
        // UPDATE THE ELAPSED TIME.
        ElapsedTime += elapsed_time;

        // CHECK IF THE CUTSCENE HAS COMPLETED.
        bool cutscene_completed = Completed();
        if (cutscene_completed)
        {
            // RETURN TO THE MAIN GAMEPLAY STATE.
            return GameState::GAMEPLAY;
        }
        else
        {
            // REMAIN ON THE CUTSCENE UNTIL IT COMPLETES.
            return GameState::FLOOD_CUTSCENE;
        }
    }

    /// Renders the current frame of the cutscene.
    /// @param[in,out]  renderer - The renderer to use.
    /// @return The rendered flood cutscene.
    sf::Sprite FloodCutscene::Render(GRAPHICS::Renderer& renderer)
    {
        // DON'T RENDER ANYTHING IF THE CUTSCENE IS COMPLETE.
        bool cutscene_completed = Completed();
        if (cutscene_completed)
        {
            return sf::Sprite();
        }

        // DRAW THE SKY BACKGROUND.
        const GRAPHICS::Color SKY_COLOR(160, 192, 255);
        MATH::FloatRectangle full_screen_rectangle = renderer.Screen->GetBoundingRectangle<float>();
        renderer.RenderScreenRectangle(full_screen_rectangle, SKY_COLOR);

        // DRAW A MOUNTAIN IN THE BACKGROUND.
        float screen_width_in_pixels = full_screen_rectangle.Width();
        float mountain_radius_in_pixels = screen_width_in_pixels / 2.0f;
        constexpr std::size_t TRIANGLE_POINT_COUNT = 3;
        sf::CircleShape mountain(mountain_radius_in_pixels, TRIANGLE_POINT_COUNT);

        const sf::Color MOUNTAIN_COLOR(96, 32, 0);
        mountain.setFillColor(MOUNTAIN_COLOR);

        float screen_height_in_pixels = full_screen_rectangle.Height();
        float one_third_down_screen_y_position = screen_height_in_pixels / 3.0f;
        sf::Vector2f mountain_screen_position(0.0f, one_third_down_screen_y_position);
        mountain.setPosition(mountain_screen_position);

        renderer.Screen->RenderTarget.draw(mountain);

        // DRAW THE ARK IN FRONT OF THE MOUNTAIN.
        const sf::Vector2f ARK_SIZE_IN_PIXELS(100.0f, 50.0f);
        sf::RectangleShape ark(ARK_SIZE_IN_PIXELS);

        const sf::Color ARK_COLOR(128, 64, 0);
        ark.setFillColor(ARK_COLOR);

        constexpr float FLOOD_RISING_RATE_IN_PIXELS_PER_SECOND = 32.0f;
        float cutscene_elapsed_time_in_seconds = ElapsedTime.asSeconds();
        float flood_water_height_in_pixels = FLOOD_RISING_RATE_IN_PIXELS_PER_SECOND * cutscene_elapsed_time_in_seconds;
        float flood_water_bottom_screen_position = screen_height_in_pixels;
        float flood_water_top_screen_position = flood_water_bottom_screen_position - flood_water_height_in_pixels;

        float ark_top_y_position = flood_water_top_screen_position - ARK_SIZE_IN_PIXELS.y;

        float screen_center_x_position = full_screen_rectangle.CenterX();
        float ark_left_x_position = screen_center_x_position - (ARK_SIZE_IN_PIXELS.x / 2.0f);
        ark.setPosition(ark_left_x_position, ark_top_y_position);

        renderer.Screen->RenderTarget.draw(ark);

        // DRAW THE RISING FLOOD WATERS.
        sf::Vector2f water_size_in_pixels(screen_width_in_pixels, flood_water_height_in_pixels);
        sf::RectangleShape flood_waters(water_size_in_pixels);

        flood_waters.setFillColor(sf::Color::Blue);

        flood_waters.setPosition(0.0f, flood_water_top_screen_position);

        renderer.Screen->RenderTarget.draw(flood_waters);

        // DRAW SOME RAIN.
        constexpr std::size_t RAIN_LINE_MAX_COUNT = 50;
        const sf::Color RAIN_COLOR(64, 64, 255);
        sf::Vertex base_rain_line_top_vertex(sf::Vector2f(8.0f, 0.0f), RAIN_COLOR);
        sf::Vertex base_rain_line_bottom_vertex(sf::Vector2f(0.0f, 8.0f), RAIN_COLOR);
        std::vector<sf::Vertex> rain_line_vertices;
        for (std::size_t rain_line_count = 0; rain_line_count < RAIN_LINE_MAX_COUNT; ++rain_line_count)
        {
            // COMPUTE A RANDOM OFFSET FOR THE CURRENT RAIN LINE.
            float current_rain_line_x_offset = RandomNumberGenerator.RandomNumberLessThan<float>(screen_width_in_pixels);
            float current_rain_line_y_offset = RandomNumberGenerator.RandomNumberLessThan<float>(screen_height_in_pixels);

            // ADD VERTICES FOR THE CURRENT LINE.
            sf::Vertex current_rain_line_top_vertex = base_rain_line_top_vertex;
            current_rain_line_top_vertex.position.x += current_rain_line_x_offset;
            current_rain_line_top_vertex.position.y += current_rain_line_y_offset;
            rain_line_vertices.push_back(current_rain_line_top_vertex);

            sf::Vertex current_rain_line_bottom_vertex = base_rain_line_bottom_vertex;
            current_rain_line_bottom_vertex.position.x += current_rain_line_x_offset;
            current_rain_line_bottom_vertex.position.y += current_rain_line_y_offset;
            rain_line_vertices.push_back(current_rain_line_bottom_vertex);
        }

        renderer.Screen->RenderTarget.draw(rain_line_vertices.data(), rain_line_vertices.size(), sf::Lines);

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen();
        return screen;
    }

    // Determines if the cutscene has been completed.
    /// @return True if the cutscene has completed; false otherwise.
    bool FloodCutscene::Completed() const
    {
        const sf::Time CUTSCENE_TOTAL_TIME = sf::seconds(12);
        bool cutscene_completed = (ElapsedTime >= CUTSCENE_TOTAL_TIME);
        return cutscene_completed;
    }
}
