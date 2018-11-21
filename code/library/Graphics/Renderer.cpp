#include "Core/NullChecking.h"
#include "Core/String.h"
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"

namespace GRAPHICS
{
    /// Constructor.
    /// @param[in]  colored_text_shader - The shader to use for coloring text.
    /// @param[in,out]  screen - The screen to render to.
    /// @throws std::exception - Thrown if a parameter is null.
    Renderer::Renderer(
        const std::shared_ptr<sf::Shader>& colored_text_shader,
        std::unique_ptr<GRAPHICS::Screen>&& screen) :
    Screen(std::move(screen)),
    Camera(MATH::FloatRectangle::FromCenterAndDimensions(
        Screen->RenderTarget.getView().getCenter().x,
        Screen->RenderTarget.getView().getCenter().y,
        Screen->RenderTarget.getView().getSize().x,
        Screen->RenderTarget.getView().getSize().y)),
    ColoredTextShader(colored_text_shader)
    {
        // MAKE SURE REQUIRED PARAMETERS EXISTS.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Screen,
            "The screen for the renderer cannot be null.");
        
        CORE::ThrowInvalidArgumentExceptionIfNull(
            ColoredTextShader,
            "The colored text shader for the renderer cannot be null.");
    }

    /// Renders a colored rectangle in screen coordinates.
    /// @param[in]  rectangle - The rectangle (in screen coordinates) to render.
    /// @param[in]  color - The color of the rectangle to render.
    void Renderer::RenderScreenRectangle(
        const MATH::FloatRectangle& rectangle,
        const GRAPHICS::Color& color)
    {
        // CONVERT THE RECTANGLE POSITION TO A WORLD POSITION.
        // This is necessary so that the rectangle can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        int left_screen_position = static_cast<int>(rectangle.GetLeftXPosition());
        int top_screen_position = static_cast<int>(rectangle.GetTopYPosition());
        sf::Vector2f top_left_world_position = Screen->RenderTarget.mapPixelToCoords(sf::Vector2i(
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
        Screen->RenderTarget.draw(renderable_rectangle);
    }

    /// Renders an icon on the screen that indicates that a specific key
    /// should be pressed for something.
    /// @param[in]  key - The character of the key for the icon.
    /// @param[in]  left_top_screen_position_in_pixels - The left-top screen position
    ///     of the key icon.
    void Renderer::RenderKeyIcon(
        const char key,
        const MATH::Vector2ui& left_top_screen_position_in_pixels)
    {
        // CONVERT THE SCREEN POSITION TO A WORLD POSITION.
        // This is necessary so that the key icon can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        sf::Vector2f left_top_world_position = Screen->RenderTarget.mapPixelToCoords(sf::Vector2i(
            left_top_screen_position_in_pixels.X,
            left_top_screen_position_in_pixels.Y));

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
        key_background_icon.setPosition(left_top_world_position);

        // RENDER THE BACKGROUND RECTANGLE FOR THE KEY.
        Screen->RenderTarget.draw(key_background_icon);

        // GET THE DEFAULT FONT.
        auto id_with_font = Fonts.find(RESOURCES::AssetId::FONT_TEXTURE);
        bool font_exists = (id_with_font != Fonts.cend()) && (nullptr != id_with_font->second);
        if (!font_exists)
        {
            // The text can't be rendered without the font.
            return;
        }

        // GET THE GLYPH FOR THE KEY.
        auto font = id_with_font->second;
        GRAPHICS::GUI::Glyph glyph = font->GetGlyph(key);

        // CREATE A SPRITE FOR THE GLYPH.
        sf::IntRect key_texture_sub_rectangle = glyph.TextureSubRectangle.ToSfmlRectangle<int>();
        sf::Sprite key_character_sprite(glyph.Texture->TextureResource, key_texture_sub_rectangle);
        key_character_sprite.setPosition(
            static_cast<float>(left_top_screen_position_in_pixels.X),
            static_cast<float>(left_top_screen_position_in_pixels.Y));

        // CONFIGURE THE RENDER TARGET FOR SCREEN-SPACE RENDERING.
        sf::View screen_space_view = Screen->RenderTarget.getDefaultView();
        Screen->RenderTarget.setView(screen_space_view);

        // RENDER THE GLYPH FOR THE KEY.
        sf::RenderStates render_states = ConfigureColoredTextShader(Color::BLACK);
        Screen->RenderTarget.draw(key_character_sprite, render_states);
    }

    /// Renders a GUI icon on the screen.
    /// @param[in]  texture - The texture containing the icon to render.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture defining
    ///     the bounding box of the icon to render.
    /// @param[in]  left_top_screen_position_in_pixels - The left-top screen position
    ///     of the icon.
    void Renderer::RenderGuiIcon(
        const GRAPHICS::Texture& texture,
        const MATH::FloatRectangle& texture_sub_rectangle,
        const MATH::Vector2ui& left_top_screen_position_in_pixels)
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
        sf::Vector2f left_top_world_position = Screen->RenderTarget.mapPixelToCoords(sf::Vector2i(
            left_top_screen_position_in_pixels.X,
            left_top_screen_position_in_pixels.Y));
        gui_icon.setPosition(left_top_world_position);

        // RENDER THE GUI ICON.
        Screen->RenderTarget.draw(gui_icon);
    }

    /// Renders a sprite as a GUI icon on the screen.
    /// @param[in]  sprite - The sprite to render as a GUI icon.
    /// @param[in]  left_top_screen_position_in_pixels - The left-top screen position
    ///     of the icon.
    void Renderer::RenderGuiIcon(
        const GRAPHICS::Sprite& sprite,
        const MATH::Vector2f& left_top_screen_position_in_pixels)
    {
        // CREATE A SPRITE FOR THE ICON USING THE TEXTURE INFORMATION.
        // This allow repositioning of the icon to be in screen coordinates.
        sf::IntRect texture_rectangle = sprite.SpriteResource.getTextureRect();
        sf::Sprite gui_icon(sprite.SpriteTexture->TextureResource, texture_rectangle);

        // POSITION THE GUI ICON SPRITE.
        // The screen position must be converted to a world position so that the GUI icon
        // can be rendered appropriately on screen regardless of how the camera might
        // move around the world.
        sf::Vector2f left_top_world_position = Screen->RenderTarget.mapPixelToCoords(sf::Vector2i(
            static_cast<int>(left_top_screen_position_in_pixels.X),
            static_cast<int>(left_top_screen_position_in_pixels.Y)));
        gui_icon.setPosition(left_top_world_position);

        // RENDER THE GUI ICON.
        Screen->RenderTarget.draw(gui_icon);
    }

    /// Renders text to the screen at the specified position.
    /// All text will be rendered on the same line.
    /// @param[in]  text - The text to render.
    /// @param[in]  font_id - The ID of the font to use for the text.
    /// @param[in]  left_top_screen_position_in_pixels - The left/top screen position
    ///     at which to render the text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in]  text_scale_ratio - The scaling of the text, relative to the default
    ///     size of the font's glyphs.  1.0f is normal scaling.
    void Renderer::RenderText(
        const std::string& text, 
        const RESOURCES::AssetId font_id,
        const MATH::Vector2f& left_top_screen_position_in_pixels,
        const Color& text_color,
        const float text_scale_ratio)
    {
        // RENDER THE TEXT TO THE CONSOLE IF NO FONT EXISTS.
        // This is intended primarily to provide debug support.
        auto id_with_font = Fonts.find(font_id);
        bool font_exists = (id_with_font != Fonts.cend()) && (nullptr != id_with_font->second);
        if (!font_exists)
        {
            DEBUGGING::DebugConsole::WriteLine(text);
            return;
        }

        // RENDER EACH CHARACTER OF THE TEXT.
        auto font = id_with_font->second;
        MATH::Vector2f current_character_left_top_screen_position = left_top_screen_position_in_pixels;
        for (const char character : text)
        {
            // GET THE GLYPH FOR THE CURRENT CHARACTER.
            GUI::Glyph glyph = font->GetGlyph(character);

            // CREATE A SPRITE FOR THE CURRENT GLYPH.
            sf::IntRect texture_rectangle = glyph.TextureSubRectangle.ToSfmlRectangle<int>();
            sf::Sprite current_character_sprite(glyph.Texture->TextureResource, texture_rectangle);
            current_character_sprite.setPosition(
                current_character_left_top_screen_position.X,
                current_character_left_top_screen_position.Y);
            current_character_sprite.setScale(text_scale_ratio, text_scale_ratio);

            // CONFIGURE THE RENDER TARGET FOR SCREEN-SPACE RENDERING.
            sf::View screen_space_view = Screen->RenderTarget.getDefaultView();
            Screen->RenderTarget.setView(screen_space_view);

            // RENDER THE CURRENT GLYPH.
            sf::RenderStates render_states = ConfigureColoredTextShader(text_color);
            Screen->RenderTarget.draw(current_character_sprite, render_states);

            // CALCULATE THE LEFT-TOP SCREEN POSITION OF THE NEXT CHARACTER.
            float glyph_width = GUI::Glyph::WidthInPixels<float>(text_scale_ratio);
            current_character_left_top_screen_position.X += glyph_width;
        }
    }

    /// Renders text to the screen within the specified rectangle.
    /// Text is wrapped based on words and can be rendered on separate
    /// lines based on newline characters.  If the amount of text to be
    /// rendered exceeds the dimensions of the specified rectangle,
    /// then it will be rendered outside of rectangle.
    /// @param[in]  text - The text to render.
    /// @param[in]  font_id - The ID of the font to use for the text.
    /// @param[in]  bounding_screen_rectangle - The bounding rectangle
    ///     within the screen in which to render text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in]  text_scale_ratio - The scaling of the text, relative to the default
    ///     size of the font's glyphs.  1.0f is normal scaling.
    void Renderer::RenderText(
        const std::string& text,
        const RESOURCES::AssetId font_id,
        const MATH::FloatRectangle& bounding_screen_rectangle,
        const Color& text_color,
        const float text_scale_ratio)
    {
        // SPLIT THE PROVIDED TEXT INTO LINES BASED ON EMBEDDED LINE BREAKS.
        std::vector<std::string> original_lines_from_text = CORE::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.GetWidth();
        float glyph_width_in_pixels = GUI::Glyph::WidthInPixels<float>(text_scale_ratio);
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / glyph_width_in_pixels);
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
                    size_t current_line_length_in_characters = current_new_line.length();
                    const size_t SPACE_CHARACTER_BETWEEN_WORDS_COUNT = 1;
                    size_t line_length_with_next_word_in_characters =
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
            RenderText(line, font_id, current_line_left_top_screen_position, text_color, text_scale_ratio);

            // MOVE TO THE NEXT LINE.
            float glyph_height_in_pixels = GUI::Glyph::HeightInPixels<float>(text_scale_ratio);
            current_line_left_top_screen_position.Y += glyph_height_in_pixels;
        }
    }

    /// Renders text to the screen centered horizontally and vertically within the specified rectangle.
    /// Text is wrapped based on words and can be rendered on separate
    /// lines based on newline characters.  If the amount of text to be
    /// rendered exceeds the dimensions of the specified rectangle,
    /// then it will be rendered outside of rectangle.
    /// @param[in]  text - The text to render.
    /// @param[in]  font_id - The ID of the font to use for the text.
    /// @param[in]  bounding_screen_rectangle - The bounding rectangle
    ///     within the screen in which to render text.
    /// @param[in]  text_color - The color of the text.
    /// @param[in]  text_scale_ratio - The scaling of the text, relative to the default
    ///     size of the font's glyphs.  1.0f is normal scaling.
    void Renderer::RenderCenteredText(
        const std::string& text,
        const RESOURCES::AssetId font_id,
        const MATH::FloatRectangle& bounding_screen_rectangle,
        const Color& text_color,
        const float text_scale_ratio)
    {
        // SPLIT THE PROVIDED TEXT INTO LINES BASED ON EMBEDDED LINE BREAKS.
        std::vector<std::string> original_lines_from_text = CORE::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.GetWidth();
        float glyph_width_in_pixels = GUI::Glyph::WidthInPixels<float>(text_scale_ratio);
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / glyph_width_in_pixels);
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
                    size_t current_line_length_in_characters = current_new_line.length();
                    const size_t SPACE_CHARACTER_BETWEEN_WORDS_COUNT = 1;
                    size_t line_length_with_next_word_in_characters =
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
        size_t new_line_count = new_lines_of_text.size();
        unsigned int glyph_height_in_pixels = GUI::Glyph::HeightInPixels<unsigned int>(text_scale_ratio);
        size_t total_text_height_in_pixels = new_line_count * GUI::Glyph::HEIGHT_IN_PIXELS;
        size_t unused_vertical_space_in_pixels = bounding_rectangle_height_in_pixels - total_text_height_in_pixels;
        size_t half_of_unused_vertical_space_in_pixels = unused_vertical_space_in_pixels / 2;
        float bounding_rectangle_top_y_screen_position = bounding_screen_rectangle.GetTopYPosition();
        float first_line_top_y_screen_position = bounding_rectangle_top_y_screen_position + half_of_unused_vertical_space_in_pixels;

        MATH::Vector2f current_line_left_top_screen_position(
            bounding_rectangle_left_x_screen_position,
            first_line_top_y_screen_position);

        // RENDER EACH LINE OF TEXT.
        for (const auto& line : new_lines_of_text)
        {
            // CENTER THE CURRENT LINE HORIZONTALLY.
            size_t current_line_character_count = line.length();
            unsigned int current_line_width_in_pixels = static_cast<unsigned int>(current_line_character_count * glyph_width_in_pixels);
            unsigned int bounding_rectangle_width_in_pixels = static_cast<unsigned int>(bounding_screen_rectangle.GetWidth());
            unsigned int unused_space_on_current_line_in_pixels = bounding_rectangle_width_in_pixels - current_line_width_in_pixels;
            unsigned int half_of_unused_space_on_current_line_in_pixels = unused_space_on_current_line_in_pixels / 2;
            current_line_left_top_screen_position.X = bounding_screen_rectangle.GetLeftXPosition();
            current_line_left_top_screen_position.X += half_of_unused_space_on_current_line_in_pixels;

            // RENDER THE CURRENT LINE.
            RenderText(line, font_id, current_line_left_top_screen_position, text_color, text_scale_ratio);

            // MOVE TO THE NEXT LINE.
            current_line_left_top_screen_position.Y += glyph_height_in_pixels;
        }
    }

    /// Renders a multi-tile-map grid.
    /// @param[in]  tile_map_grid - The tile map grid to render.
    void Renderer::Render(const MAPS::MultiTileMapGrid& tile_map_grid)
    {
        MATH::FloatRectangle camera_bounds = Camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

        /// @note   This view only needs to be set here.
        /// Private methods assume it has already been set.
        sf::View camera_view;
        camera_view.setCenter(camera_view_center.X, camera_view_center.Y);
        camera_view.setSize(camera_bounds.GetWidth(), camera_bounds.GetHeight());
        Screen->RenderTarget.setView(camera_view);

        // GET THE CURRENT TILE MAP.
        const MAPS::TileMap* current_tile_map = tile_map_grid.GetTileMap(camera_view_center.X, camera_view_center.Y);
        if (!current_tile_map)
        {
            // A current tile map is required for rendering.
            return;
        }

        // RENDER THE CURRENT TILE MAP.
        unsigned int min_tile_map_row = current_tile_map->GridRowIndex;
        bool top_tile_map_exists = (min_tile_map_row > 0);
        if (top_tile_map_exists)
        {
            --min_tile_map_row;
        }
        unsigned int min_tile_map_column = current_tile_map->GridColumnIndex;
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
                const MAPS::TileMap* tile_map = tile_map_grid.GetTileMap(tile_map_row, tile_map_column);
                bool tile_map_exists = (nullptr != tile_map);
                if (!tile_map_exists)
                {
                    // Continue trying to render other tile maps.
                    continue;
                }

                // RENDER THE TILE MAP.
                Render(*tile_map);
            }
        }
    }

    /// Renders a tile map.
    /// @param[in]  tile_map - The tile map to render.
    void Renderer::Render(const MAPS::TileMap& tile_map)
    {
        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
        MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
            {
                const std::shared_ptr<MAPS::Tile>& tile = tile_map.Ground.Tiles(tile_column, tile_row);
                tile->Sprite.Render(*Screen);
            }
        }
        
        // RENDER THE CURRENT TILE MAP'S ARK PIECES.
        for (const auto& ark_piece : tile_map.ArkPieces)
        {
            // Only ark pieces that have been built should be visible.
            if (ark_piece.Built)
            {
                ark_piece.Sprite.Render(*Screen);
            }

        }

        // RENDER THE CURRENT TILE MAP'S WOOD LOGS.
        for (const auto& wood_log : tile_map.WoodLogs)
        {
            wood_log.Sprite.Render(*Screen);
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            // RENDER THE TREE.
            tree.Sprite.Render(*Screen);

            // RENDER ANY FOOD ON THE TREE.
            if (tree.Food)
            {
                tree.Food->Sprite.Render(*Screen);
            }
        }

        // RENDER THE CURRENT TILE MAP'S FALLING FOOD.
        for (const auto& food : tile_map.FallingFood)
        {
            food.FoodItem.Sprite.Render(*Screen);
        }

        // RENDER THE CURRENT TILE MAP'S FOOD ON THE GROUND.
        for (const auto& food : tile_map.FoodOnGround)
        {
            food.Sprite.Render(*Screen);
        }

        // RENDER THE CURRENT TILE MAP'S ANIMALS.
        for (const auto& animal : tile_map.Animals)
        {
            animal->Sprite.Render(*Screen);
        }

        // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
        for (const auto& dust_cloud : tile_map.DustClouds)
        {
            dust_cloud.Sprite.Render(*Screen);
        }
    }

    /// Configures the colored text shader to render text using the specified color,
    /// returning the corresponding render states.
    /// @param[in]  color - The color for the text to render.
    /// @return The render states for the configured colored text shader.
    sf::RenderStates Renderer::ConfigureColoredTextShader(const Color& color)
    {
        // CONFIGURE THE SHADER IN THE RENDER STATES.
        sf::RenderStates render_states = sf::RenderStates::Default;
        ColoredTextShader->setUniform("color", sf::Glsl::Vec4(sf::Color(color.Red, color.Green, color.Blue, color.Alpha)));
        ColoredTextShader->setUniform("texture", sf::Shader::CurrentTexture);
        render_states.shader = ColoredTextShader.get();
        return render_states;
    }
}
