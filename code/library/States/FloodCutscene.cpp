#include <SFML/Graphics.hpp>
#include "States/FloodCutscene.h"

namespace STATES
{
    /// Update the cutscene based on an elapsed amount of time.
    /// @param[in]  gaming_hardware - The hardware supplying input and output for the update.
    /// @return The state that the game should be in after the update.
    GameState FloodCutscene::Update(const HARDWARE::GamingHardware& gaming_hardware)
    {
        // UPDATE THE ELAPSED TIME.
        ElapsedTime += gaming_hardware.Clock.ElapsedTimeSinceLastFrame;

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
        // The mountain should be positioned to have its base at the bottom of the screen.
        // To account for centering of the mountain, it's half-height must be used.
        MATH::Vector2f screen_center = full_screen_rectangle.Center();
        float mountain_height_in_pixels = Mountain.GetHeightInPixels();
        float mountain_half_height_in_pixels = mountain_height_in_pixels / 2.0f;
        float screen_height_in_pixels = full_screen_rectangle.Height();
        float mountain_screen_y_position = screen_height_in_pixels - mountain_half_height_in_pixels;
        MATH::Vector2f mountain_screen_position(screen_center.X, mountain_screen_y_position);
        Mountain.SetWorldPosition(mountain_screen_position);
        renderer.Render(Mountain);

        // DRAW THE ARK IN FRONT OF THE MOUNTAIN.
        // It should be positioned just on top of the flood waters.
        constexpr float FLOOD_RISING_RATE_IN_PIXELS_PER_SECOND = 32.0f;
        float cutscene_elapsed_time_in_seconds = ElapsedTime.asSeconds();
        float flood_water_height_in_pixels = FLOOD_RISING_RATE_IN_PIXELS_PER_SECOND * cutscene_elapsed_time_in_seconds;
        float flood_water_bottom_screen_position = screen_height_in_pixels;
        float flood_water_top_screen_position = flood_water_bottom_screen_position - flood_water_height_in_pixels;
        // Since the flood sprite has "waves" that only peak at the top position calculated above
        // the ark is shifted down a bit further to have it appear "within" the waters.
        constexpr float FLOOD_WATER_WAVE_DIP_HEIGHT_IN_PIXELS = 8.0f;
        float ark_half_height_in_pixels = Ark.GetHeightInPixels() / 2.0f;
        float ark_center_y_position = flood_water_top_screen_position + FLOOD_WATER_WAVE_DIP_HEIGHT_IN_PIXELS - ark_half_height_in_pixels;
        Ark.SetWorldPosition(screen_center.X, ark_center_y_position);
        renderer.Render(Ark);

        // DRAW THE RISING FLOOD WATERS.
        // Since the flood sprite is technically designed to be the "full" height of the screen, the flood sprite should:
        // - Start at the bottom of the screen (being shifted down according to the sprite's half height to account for sprite centering)
        // - Rise up based on the height of the flood waters.
        float flood_water_sprite_half_height_in_pixels = FloodWaters.GetHeightInPixels() / 2.0f;
        float flood_water_sprite_bottom_screen_position = flood_water_bottom_screen_position + flood_water_sprite_half_height_in_pixels;
        float flood_water_sprite_center_y_screen_position = flood_water_sprite_bottom_screen_position - flood_water_height_in_pixels;
        FloodWaters.SetWorldPosition(screen_center.X, flood_water_sprite_center_y_screen_position);
        renderer.Render(FloodWaters);

        // DRAW SOME RAIN.
        constexpr std::size_t RAIN_LINE_MAX_COUNT = 50;
        const sf::Color RAIN_COLOR(64, 64, 255);
        sf::Vertex base_rain_line_top_vertex(sf::Vector2f(8.0f, 0.0f), RAIN_COLOR);
        sf::Vertex base_rain_line_bottom_vertex(sf::Vector2f(0.0f, 8.0f), RAIN_COLOR);
        std::vector<sf::Vertex> rain_line_vertices;
        for (std::size_t rain_line_count = 0; rain_line_count < RAIN_LINE_MAX_COUNT; ++rain_line_count)
        {
            // COMPUTE A RANDOM OFFSET FOR THE CURRENT RAIN LINE.
            float screen_width_in_pixels = full_screen_rectangle.Width();
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
        // The time has been carefully chosen to allow the flood waters to rise to the cover the entire screen
        // without the flood water sprite rising too high (making the bottom of the screen not blue).
        const sf::Time CUTSCENE_TOTAL_TIME = sf::seconds(12.4f);
        bool cutscene_completed = (ElapsedTime >= CUTSCENE_TOTAL_TIME);
        return cutscene_completed;
    }
}
