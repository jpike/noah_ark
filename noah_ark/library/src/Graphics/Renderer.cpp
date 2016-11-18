#include <iostream>
#include "Core/String.h"
#include "Graphics/Renderer.h"

namespace GRAPHICS
{
    /// Constructor.
    /// @param[in]  camera_view_bounds - The bounding rectangle (in world coordinates) of the camera's view.
    Renderer::Renderer(const MATH::FloatRectangle& camera_view_bounds) :
        Camera(camera_view_bounds),
        Font(nullptr),
        ColoredTextShader()
    {}

    /// @todo   Document.
    /*void Renderer::Render(const GRAPHICS::GUI::Text& text, Screen& screen)
    {
        text.Render(screen);
    }*/

    /// @todo   Document.
    void Renderer::RenderScreenRectangle(
        const MATH::FloatRectangle& rectangle,
        const GRAPHICS::Color& color,
        Screen& screen)
    {
        // CONVERT THE RECTANGLE POSITION TO A WORLD POSITION.
        // This is necessary so that the rectangle can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        int left_screen_position = static_cast<int>(rectangle.GetLeftXPosition());
        int top_screen_position = static_cast<int>(rectangle.GetTopYPosition());
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            left_screen_position,
            top_screen_position));

        // CREATE THE RECTANGLE TO RENDER.
        sf::RectangleShape renderable_rectangle;
        renderable_rectangle.setFillColor(sf::Color(color.Red, color.Green, color.Blue));

        float width = rectangle.GetWidth();
        float height = rectangle.GetHeight();
        renderable_rectangle.setSize(sf::Vector2f(width, height));

        renderable_rectangle.setPosition(top_left_world_position);

        // RENDER THE RECTANGLE.
        screen.RenderTarget->draw(renderable_rectangle);
    }

    /// @todo   Document.
    void Renderer::RenderKeyIcon(
        const char key,
        const GRAPHICS::GUI::Font& font,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        Screen& screen)
    {
        // CONVERT THE SCREEN POSITION TO A WORLD POSITION.
        // This is necessary so that the key icon can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));

        // CREATE A RECTANGLE TO RESEMBLE A KEY ON A KEYBOARD.
        const sf::Color DARK_GRAY(128, 128, 128);
        const sf::Color LIGHT_GRAY(176, 176, 176);
        sf::RectangleShape key_background_icon;
        key_background_icon.setFillColor(LIGHT_GRAY);
        key_background_icon.setOutlineColor(DARK_GRAY);
        key_background_icon.setOutlineThickness(2.0f);
        key_background_icon.setSize(sf::Vector2f(
            static_cast<float>(GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS)));
        key_background_icon.setPosition(top_left_world_position);

        // RENDER THE BACKGROUND RECTANGLE FOR THE KEY.
        screen.RenderTarget->draw(key_background_icon);

        // GET THE GLYPH FOR THE KEY.
        GRAPHICS::GUI::Glyph glyph = font.GetGlyph(key);

        // CALCULATE THE CENTER WORLD POSITION OF THE GLYPH.
        float glyph_width = glyph.TextureSubRectangle.GetWidth();
        float glyph_half_width = glyph_width / 2.0f;
        float glyph_center_world_x_position = top_left_world_position.x + glyph_half_width;
        float glyph_height = glyph.TextureSubRectangle.GetHeight();
        float glyph_half_height = glyph_height / 2.0f;
        float glyph_center_world_y_position = top_left_world_position.y + glyph_half_height;
        MATH::Vector2f glyph_center_world_position(
            glyph_center_world_x_position,
            glyph_center_world_y_position);

        // CREATE A SPRITE FOR THE GLYPH.
        Sprite key_character_sprite(
            glyph.Texture,
            glyph.TextureSubRectangle);
        key_character_sprite.SetWorldPosition(glyph_center_world_position);

        // RENDER THE GLYPH FOR THE KEY.
        /// @todo   Remove this duplicated code.
        sf::RenderStates render_states = sf::RenderStates::Default;
        const sf::Transform RENDER_IN_SCREEN_SPACE = sf::Transform::Identity;
        render_states.transform = RENDER_IN_SCREEN_SPACE;
        ColoredTextShader.setParameter("color", sf::Color::Black);
        ColoredTextShader.setParameter("texture", sf::Shader::CurrentTexture);
        render_states.shader = &ColoredTextShader;
        screen.RenderTarget->draw(key_character_sprite.SpriteResource, render_states);
    }

    /// @todo   Document.
    void Renderer::RenderGuiIcon(
        const GRAPHICS::Texture& texture,
        const MATH::FloatRectangle& texture_sub_rectangle,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        Screen& screen)
    {
        // CREATE A SPRITE FOR THE ICON USING THE TEXTURE INFORMATION.
        sf::IntRect texture_rectangle;
        texture_rectangle.top = static_cast<int>(texture_sub_rectangle.GetTopYPosition());
        texture_rectangle.left = static_cast<int>(texture_sub_rectangle.GetLeftXPosition());
        texture_rectangle.width = static_cast<int>(texture_sub_rectangle.GetWidth());
        texture_rectangle.height = static_cast<int>(texture_sub_rectangle.GetHeight());
        sf::Sprite gui_icon(texture.TextureResource, texture_rectangle);

        // POSITION THE GUI ICON SPRITE.
        // The screen position must be converted to a world position so that the GUI icon
        // can be rendered appropriately on screen regardless of how the camera might
        // move around the world.
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));
        gui_icon.setPosition(top_left_world_position);

        // RENDER THE GUI ICON.
        screen.RenderTarget->draw(gui_icon);
    }

    /// Renders an overworld.
    /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last rendering of the world.
    ///     @todo   Look at ways to remove this parameter.
    /// @param[in]  overworld - The overworld to render.
    ///     @todo   Look at how to make overworld parameter const.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(
        const float elapsed_time_in_seconds, 
        MAPS::Overworld& overworld, 
        Screen& screen)
    {
        /// @todo   Factor out updating from this method?

        MATH::FloatRectangle camera_bounds = Camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

        /// @note   This view only needs to be set here.
        /// Private methods assume it has already been set.
        sf::View camera_view;
        camera_view.setCenter(camera_view_center.X, camera_view_center.Y);
        camera_view.setSize(camera_bounds.GetWidth(), camera_bounds.GetHeight());
        screen.RenderTarget->setView(camera_view);

        MAPS::TileMap* current_tile_map = overworld.GetTileMap(camera_view_center.X, camera_view_center.Y);
        assert(current_tile_map);

        // RENDER THE CURRENT TILE MAP.
        /// @todo   Consider signed ints.  Right now, we're just
        /// rendering a few surrounding tile maps.
        unsigned int min_tile_map_row = current_tile_map->OverworldRowIndex;
        bool top_tile_map_exists = (min_tile_map_row > 0);
        if (top_tile_map_exists)
        {
            --min_tile_map_row;
        }
        unsigned int min_tile_map_column = current_tile_map->OverworldColumnIndex;
        bool left_tile_map_exists = (min_tile_map_column > 0);
        if (left_tile_map_exists)
        {
            --min_tile_map_column;
        }
        unsigned int max_tile_map_row = min_tile_map_row + 2;
        unsigned int max_tile_map_column = min_tile_map_column + 2;
        for (unsigned int tile_map_row = min_tile_map_row; tile_map_row <= max_tile_map_row; ++tile_map_row)
        {
            for (unsigned int tile_map_column = min_tile_map_column; tile_map_column <= max_tile_map_column; ++tile_map_column)
            {
                // GET THE CURRENT TILE MAP.
                MAPS::TileMap* tile_map = overworld.GetTileMap(tile_map_row, tile_map_column);
                bool tile_map_exists = (nullptr != tile_map);
                if (!tile_map_exists)
                {
                    // Continue trying to render other tile maps.
                    continue;
                }

                // UPDATE THE CURRENT TILE MAP'S TREES.
                /// @todo   Figure out if this maybe should be handled elsewhere.
                for (auto tree = tile_map->Trees.begin(); tree != tile_map->Trees.end(); ++tree)
                {
                    tree->Update(elapsed_time_in_seconds);
                }

                // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
                /// @todo   Maybe we only need a single dust cloud?  At least in theory, it shouldn't be
                /// possible for more than one tree to in this state at a given time.
                for (auto dust_cloud = tile_map->TreeDustClouds.begin(); dust_cloud != tile_map->TreeDustClouds.end();)
                {
                    // UPDATE THE CURRENT DUST CLOUD.
                    dust_cloud->Update(elapsed_time_in_seconds);

                    // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
                    bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
                    if (dust_cloud_disappeared)
                    {
                        // REMOVE THE DUST CLOUD.
                        dust_cloud = tile_map->TreeDustClouds.erase(dust_cloud);
                    }
                    else
                    {
                        // MOVE TO UPDATING THE NEXT DUST CLOUD.
                        ++dust_cloud;
                    }
                }

                // RENDER THE TILE MAP.
                Render(*tile_map, screen);
            }
        }

        // RENDER THE PLAYER.
        // Make sure his axe/sprite are updated.
        overworld.NoahPlayer.Inventory->Axe->Update(elapsed_time_in_seconds);
        overworld.NoahPlayer.Sprite.Update(elapsed_time_in_seconds);
        Render(overworld.NoahPlayer.Sprite, screen);

        // The axe should only be rendered if it is swinging.
        if (overworld.NoahPlayer.Inventory->Axe->IsSwinging())
        {
            Render(overworld.NoahPlayer.Inventory->Axe->Sprite, screen);
        }
    }

    /// Renders a HUD.
    /// @param[in]  hud - The HUD to render.
    /// @param[in,out]  screen - The screen to render to.
    /*void Renderer::Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, Screen& screen)
    {
        hud.Render(screen);
    }*/

    /// Renders text to the screen at the specified position.
    /// All text will be rendered on the same line.
    /// @param[in]  text - The text to render.
    /// @param[in]  left_top_screen_position_in_pixels - The left/top screen position
    ///     at which to render the text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::RenderText(
        const std::string& text, 
        const MATH::Vector2f& left_top_screen_position_in_pixels,
        const Color& text_color,
        Screen& screen)
    {
        // RENDER THE TEXT TO THE CONSOLE IF NO FONT EXISTS.
        // This is intended primarily to provide debug support.
        bool font_exists = (nullptr != Font);
        if (!font_exists)
        {
            std::cout << text << std::endl;
            return;
        }

        // RENDER EACH CHARACTER OF THE TEXT.
        MATH::Vector2f current_character_left_top_screen_position = left_top_screen_position_in_pixels;
        for (const char character : text)
        {
            // GET THE GLYPH FOR THE CURRENT CHARACTER.
            GUI::Glyph glyph = Font->GetGlyph(character);

            // CREATE A SPRITE FOR THE CURRENT GLYPH.
            sf::Sprite current_character_sprite;
            current_character_sprite.setTexture(glyph.Texture->TextureResource);
            sf::IntRect texture_rectangle(
                static_cast<int>(glyph.TextureSubRectangle.GetLeftXPosition()),
                static_cast<int>(glyph.TextureSubRectangle.GetTopYPosition()),
                static_cast<int>(glyph.TextureSubRectangle.GetWidth()),
                static_cast<int>(glyph.TextureSubRectangle.GetHeight()));
            current_character_sprite.setTextureRect(texture_rectangle);
            current_character_sprite.setPosition(
                current_character_left_top_screen_position.X,
                current_character_left_top_screen_position.Y);

            // RENDER THE CURRENT GLYPH.
            /// @todo   Contemplate potential alternative interfaces for rendering sprites in screen-space.
            sf::RenderStates render_states = sf::RenderStates::Default;
            const sf::Transform RENDER_IN_SCREEN_SPACE = sf::Transform::Identity;
            render_states.transform = RENDER_IN_SCREEN_SPACE;
            ColoredTextShader.setParameter("color", sf::Color(text_color.Red, text_color.Green, text_color.Blue, text_color.Alpha));
            ColoredTextShader.setParameter("texture", sf::Shader::CurrentTexture);
            render_states.shader = &ColoredTextShader;
            screen.RenderTarget->draw(current_character_sprite, render_states);

            // CALCULATE THE LEFT-TOP SCREEN POSITION OF THE NEXT CHARACTER.
            float glyph_width = glyph.TextureSubRectangle.GetWidth();
            current_character_left_top_screen_position.X += glyph_width;
        }
    }

    /// Renders text to the screen within the specified rectangle.
    /// Text is wrapped based on words and can be rendered on separate
    /// lines based on newline characters.  If the amount of text to be
    /// rendered exceeds the dimensions of the specified rectangle,
    /// then it will be rendered outside of rectangle.
    /// @param[in]  text - The text to render.
    /// @param[in]  bounding_screen_rectangle - The bounding rectangle
    ///     within the screen in which to render text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::RenderText(
        const std::string& text,
        const MATH::FloatRectangle& bounding_screen_rectangle,
        const Color& text_color,
        Screen& screen)
    {
        // SPLIT THE PROVIDED TEXT INTO LINES BASED ON EMBEDDED LINE BREAKS.
        std::vector<std::string> original_lines_from_text = CORE::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.GetWidth();
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / GUI::Glyph::WIDTH_IN_PIXELS);
        for (const auto& line : original_lines_from_text)
        {
            // SPLIT THE CURRENT LINE INTO INDIVIDUAL WORDS.
            std::deque<std::string> words_in_current_line = CORE::String::SplitIntoWords(
                line,
                max_characters_per_line);

            // CREATE NEW LINE(S) BASED ON THE CURRENT LINE'S WORDS.
            std::string current_new_line;
            while (!words_in_current_line.empty())
            {
                // GET THE NEXT WORD TO BE ADDED.
                std::string next_word = words_in_current_line.front();
                words_in_current_line.pop_front();

                // CHECK IF THE CURRENT LINE IS EMPTY.
                if (current_new_line.empty())
                {
                    // ADD THE CURRENT WORD TO THE NEW LINE.
                    // An empty line should always be able to hold at least 1 word.
                    current_new_line += next_word;
                }
                else
                {
                    // CHECK IF THE CURRENT LINE CAN HANDLE THE NEXT WORD.
                    unsigned int current_line_length_in_characters = current_new_line.length();
                    const unsigned int SPACE_CHARACTER_BETWEEN_WORDS_COUNT = 1;
                    unsigned int line_length_with_next_word_in_characters =
                        current_line_length_in_characters +
                        SPACE_CHARACTER_BETWEEN_WORDS_COUNT +
                        next_word.length();
                    bool current_line_can_handle_next_word = (line_length_with_next_word_in_characters <= max_characters_per_line);
                    if (current_line_can_handle_next_word)
                    {
                        // ADD A SPACE BEFORE ADDING THE NEW WORD.
                        const char SPACE_BETWEEN_WORDS = ' ';
                        current_new_line += SPACE_BETWEEN_WORDS + next_word;
                    }
                    else
                    {
                        // ADD THE CURRENT NEW LINE TO THE LIST OF LINES.
                        new_lines_of_text.push_back(current_new_line);
                        
                        // PREPARE FOR THE NEXT NEW LINE OF TEXT.
                        current_new_line.clear();

                        // ADD THE CURRENT WORD TO THE NEW LINE.
                        current_new_line += next_word;
                    }
                }

                // CHECK IF THERE ARE ANY MORE WORDS TO ADD.
                bool more_words_left = !words_in_current_line.empty();
                if (!more_words_left)
                {
                    // ADD THE CURRENT NEW LINE IF IT ISN'T EMPTY.
                    if (!current_new_line.empty())
                    {
                        new_lines_of_text.push_back(current_new_line);
                        current_new_line.clear();
                    }
                }
            }
        }

        // RENDER EACH LINE OF TEXT.
        float bounding_rectangle_left_x_screen_position = bounding_screen_rectangle.GetLeftXPosition();
        float bounding_rectangle_top_y_screen_position = bounding_screen_rectangle.GetTopYPosition();
        MATH::Vector2f current_line_left_top_screen_position(
            bounding_rectangle_left_x_screen_position,
            bounding_rectangle_top_y_screen_position);
        for (const auto& line : new_lines_of_text)
        {
            // RENDER THE CURRENT LINE.
            RenderText(line, current_line_left_top_screen_position, text_color, screen);

            // MOVE TO THE NEXT LINE.
            current_line_left_top_screen_position.Y += GUI::Glyph::HEIGHT_IN_PIXELS;
        }
    }

    /// Renders text to the screen centered horizontally and vertically within the specified rectangle.
    /// Text is wrapped based on words and can be rendered on separate
    /// lines based on newline characters.  If the amount of text to be
    /// rendered exceeds the dimensions of the specified rectangle,
    /// then it will be rendered outside of rectangle.
    /// @param[in]  text - The text to render.
    /// @param[in]  bounding_screen_rectangle - The bounding rectangle
    ///     within the screen in which to render text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::RenderCenteredText(
        const std::string& text,
        const MATH::FloatRectangle& bounding_screen_rectangle,
        const Color& text_color,
        Screen& screen)
    {
        // SPLIT THE PROVIDED TEXT INTO LINES BASED ON EMBEDDED LINE BREAKS.
        std::vector<std::string> original_lines_from_text = CORE::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.GetWidth();
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / GUI::Glyph::WIDTH_IN_PIXELS);
        for (const auto& line : original_lines_from_text)
        {
            // SPLIT THE CURRENT LINE INTO INDIVIDUAL WORDS.
            std::deque<std::string> words_in_current_line = CORE::String::SplitIntoWords(
                line,
                max_characters_per_line);

            // CREATE NEW LINE(S) BASED ON THE CURRENT LINE'S WORDS.
            std::string current_new_line;
            while (!words_in_current_line.empty())
            {
                // GET THE NEXT WORD TO BE ADDED.
                std::string next_word = words_in_current_line.front();
                words_in_current_line.pop_front();

                // CHECK IF THE CURRENT LINE IS EMPTY.
                if (current_new_line.empty())
                {
                    // ADD THE CURRENT WORD TO THE NEW LINE.
                    // An empty line should always be able to hold at least 1 word.
                    current_new_line += next_word;
                }
                else
                {
                    // CHECK IF THE CURRENT LINE CAN HANDLE THE NEXT WORD.
                    unsigned int current_line_length_in_characters = current_new_line.length();
                    const unsigned int SPACE_CHARACTER_BETWEEN_WORDS_COUNT = 1;
                    unsigned int line_length_with_next_word_in_characters =
                        current_line_length_in_characters +
                        SPACE_CHARACTER_BETWEEN_WORDS_COUNT +
                        next_word.length();
                    bool current_line_can_handle_next_word = (line_length_with_next_word_in_characters <= max_characters_per_line);
                    if (current_line_can_handle_next_word)
                    {
                        // ADD A SPACE BEFORE ADDING THE NEW WORD.
                        const char SPACE_BETWEEN_WORDS = ' ';
                        current_new_line += SPACE_BETWEEN_WORDS + next_word;
                    }
                    else
                    {
                        // ADD THE CURRENT NEW LINE TO THE LIST OF LINES.
                        new_lines_of_text.push_back(current_new_line);

                        // PREPARE FOR THE NEXT NEW LINE OF TEXT.
                        current_new_line.clear();

                        // ADD THE CURRENT WORD TO THE NEW LINE.
                        current_new_line += next_word;
                    }
                }

                // CHECK IF THERE ARE ANY MORE WORDS TO ADD.
                bool more_words_left = !words_in_current_line.empty();
                if (!more_words_left)
                {
                    // ADD THE CURRENT NEW LINE IF IT ISN'T EMPTY.
                    if (!current_new_line.empty())
                    {
                        new_lines_of_text.push_back(current_new_line);
                        current_new_line.clear();
                    }
                }
            }
        }

        // CALCULATE THE STARTING POSITION FOR THE FIRST LINE.
        float bounding_rectangle_left_x_screen_position = bounding_screen_rectangle.GetLeftXPosition();
        
        // The starting y-position for the first line is offset from the bounding rectangle's
        // top y-position such that half of the unused space appears before and after the text.
        unsigned int bounding_rectangle_height_in_pixels = static_cast<unsigned int>(bounding_screen_rectangle.GetHeight());
        unsigned int new_line_count = new_lines_of_text.size();
        unsigned int total_text_height_in_pixels = new_line_count * GUI::Glyph::HEIGHT_IN_PIXELS;
        unsigned int unused_vertical_space_in_pixels = bounding_rectangle_height_in_pixels - total_text_height_in_pixels;
        unsigned int half_of_unused_vertical_space_in_pixels = unused_vertical_space_in_pixels / 2;
        float bounding_rectangle_top_y_screen_position = bounding_screen_rectangle.GetTopYPosition();
        float first_line_top_y_screen_position = bounding_rectangle_top_y_screen_position + half_of_unused_vertical_space_in_pixels;

        MATH::Vector2f current_line_left_top_screen_position(
            bounding_rectangle_left_x_screen_position,
            first_line_top_y_screen_position);

        // RENDER EACH LINE OF TEXT.
        for (const auto& line : new_lines_of_text)
        {
            // CENTER THE CURRENT LINE HORIZONTALLY.
            unsigned int current_line_character_count = line.length();
            unsigned int current_line_width_in_pixels = current_line_character_count * GUI::Glyph::WIDTH_IN_PIXELS;
            unsigned int bounding_rectangle_width_in_pixels = static_cast<unsigned int>(bounding_screen_rectangle.GetWidth());
            unsigned int unused_space_on_current_line_in_pixels = bounding_rectangle_width_in_pixels - current_line_width_in_pixels;
            unsigned int half_of_unused_space_on_current_line_in_pixels = unused_space_on_current_line_in_pixels / 2;
            current_line_left_top_screen_position.X = bounding_screen_rectangle.GetLeftXPosition();
            current_line_left_top_screen_position.X += half_of_unused_space_on_current_line_in_pixels;

            // RENDER THE CURRENT LINE.
            RenderText(line, current_line_left_top_screen_position, text_color, screen);

            // MOVE TO THE NEXT LINE.
            current_line_left_top_screen_position.Y += GUI::Glyph::HEIGHT_IN_PIXELS;
        }
    }

    /// Renders a tile map.
    /// @param[in]  tile_map - The tile map to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const MAPS::TileMap& tile_map, Screen& screen)
    {
        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
        MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
            {
                const std::shared_ptr<MAPS::Tile>& tile = tile_map.Ground.Tiles(tile_column, tile_row);
                Render(tile->Sprite, screen);
            }
        }

        // RENDER THE CURRENT TILE MAP'S WOOD LOGS.
        for (const auto& wood_log : tile_map.WoodLogs)
        {
            Render(wood_log.Sprite, screen);
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            Render(tree.Sprite, screen);
        }

        // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
        for (const auto& dust_cloud : tile_map.TreeDustClouds)
        {
            Render(dust_cloud.Sprite, screen);
        }
    }

    /// Renders a sprite.
    /// @param[in]  sprite - The sprite to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const GRAPHICS::Sprite& sprite, Screen& screen)
    {
        sprite.Render(screen);
    }

    /// Renders an animated sprite.
    /// @param[in]  sprite - The animated sprite to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const GRAPHICS::AnimatedSprite& sprite, Screen& screen)
    {
        sprite.Render(screen);
    }
}
