#include <array>
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"
#include "Graphics/TimeOfDayLighting.h"
#include "String/String.h"

namespace GRAPHICS
{
    /// Renders the final screen based on the current state of rendering operations.
    /// @param[in]  render_settings - The settings to use for rendering.
    /// @return The rendered screen.
    sf::Sprite Renderer::RenderFinalScreen(const sf::RenderStates& render_settings)
    {
        // RENDER THE DEFAULT STATE OF THE SCREEN.
        Screen->RenderTarget.display();
        sf::Sprite screen(Screen->RenderTarget.getTexture());

        // APPLY THE RENDER SETTINGS TO THE SCREEN.
        Screen->RenderTarget.draw(screen, render_settings);
        Screen->RenderTarget.display();
        screen.setTexture(Screen->RenderTarget.getTexture());

        return screen;
    }

    /// Renders the final screen with time-of-day-shading.
    /// @return The rendered screen.
    sf::Sprite Renderer::RenderFinalScreenWithTimeOfDayShading()
    {
        // COMPUTE THE LIGHTING FOR THE CURRENT GAMEPLAY.
        // For main gameplay, the world should be tinted based on the time of day most of the time.
        sf::RenderStates lighting = sf::RenderStates::Default;
        std::shared_ptr<sf::Shader> time_of_day_shader = GraphicsDevice->GetShader(RESOURCES::AssetId::TIME_OF_DAY_SHADER);
        if (time_of_day_shader)
        {
            // COMPUTE THE LIGHTING FOR THE SHADER.
            TimeOfDayLighting::Compute(*time_of_day_shader);
            lighting.shader = time_of_day_shader.get();
        }

        // RENDER THE FINAL SCREEN.
        sf::Sprite screen = RenderFinalScreen(lighting);
        return screen;
    }

    /// Renders a line in screen coordinates.
    /// @param[in]  start_position - The starting point of the line.
    /// @param[in]  end_position - The ending point of the line.
    /// @param[in]  color - The color of the line to render.
    void Renderer::RenderLine(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position, const GRAPHICS::Color& color)
    {
        // CONVERT THE COLOR TO SFML FORMAT.
        sf::Color line_color(color.Red, color.Green, color.Blue, color.Alpha);

        // CONVERT THE POSITIONS TO SFML FORMAT.
        constexpr std::size_t LINE_VERTEX_COUNT = 2;
        std::array<sf::Vertex, LINE_VERTEX_COUNT> line_vertices =
        {
            sf::Vertex(sf::Vector2f(start_position.X, start_position.Y), line_color),
            sf::Vertex(sf::Vector2f(end_position.X, end_position.Y), line_color)
        };

        // RENDER THE LINE.
        Screen->RenderTarget.draw(line_vertices.data(), LINE_VERTEX_COUNT, sf::Lines);
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
        int left_screen_position = static_cast<int>(rectangle.LeftTop.X);
        int top_screen_position = static_cast<int>(rectangle.LeftTop.Y);
        sf::Vector2f top_left_world_position = Screen->RenderTarget.mapPixelToCoords(sf::Vector2i(
            left_screen_position,
            top_screen_position));

        // CREATE THE RECTANGLE TO RENDER.
        sf::RectangleShape renderable_rectangle;
        renderable_rectangle.setFillColor(sf::Color(color.Red, color.Green, color.Blue));

        float width = rectangle.Width();
        float height = rectangle.Height();
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
            static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS),
            static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS)));
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
        GRAPHICS::GUI::Glyph& glyph = font->GlyphsByCharacter[static_cast<unsigned int>(key)];

        // GET THE LEFT POSITION OF THE KEY SPRITE.
        // It should be offset based on the amount of space on the left of the glyph
        // in order to center it.
        float key_glyph_width_in_pixels = glyph.TextureSubRectangle.Width();
        float space_around_glyph_in_pixels = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS) - key_glyph_width_in_pixels;
        float space_on_left_of_glyph_in_pixels = space_around_glyph_in_pixels / 2.0f;
        float key_character_sprite_left_screen_position_in_pixels =
            static_cast<float>(left_top_screen_position_in_pixels.X) + space_on_left_of_glyph_in_pixels;

        // CREATE A SPRITE FOR THE GLYPH.
        sf::IntRect key_texture_sub_rectangle = glyph.TextureSubRectangle.ToSfmlRectangle<int>();
        sf::Sprite key_character_sprite(glyph.Texture->TextureResource, key_texture_sub_rectangle);
        key_character_sprite.setPosition(
            key_character_sprite_left_screen_position_in_pixels,
            static_cast<float>(left_top_screen_position_in_pixels.Y));

        // CONFIGURE THE RENDER TARGET FOR SCREEN-SPACE RENDERING.
        sf::View screen_space_view = Screen->RenderTarget.getDefaultView();
        Screen->RenderTarget.setView(screen_space_view);

        // RENDER THE GLYPH FOR THE KEY.
        sf::RenderStates render_states = sf::RenderStates::Default;
        std::shared_ptr<sf::Shader> colored_text_shader = GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
        if (colored_text_shader)
        {
            render_states = ConfigureColoredTextShader(Color::BLACK, *colored_text_shader);
        }
        Screen->RenderTarget.draw(key_character_sprite, render_states);
    }

    /// Renders a GUI icon on the screen.
    /// @param[in]  texture_id - The ID of the texture containing the icon to render.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture defining
    ///     the bounding box of the icon to render.
    /// @param[in]  left_top_screen_position_in_pixels - The left-top screen position
    ///     of the icon.
    void Renderer::RenderGuiIcon(
        const RESOURCES::AssetId texture_id,
        const MATH::FloatRectangle& texture_sub_rectangle,
        const MATH::Vector2ui& left_top_screen_position_in_pixels)
    {
        // CREATE A SPRITE FOR THE ICON USING THE TEXTURE INFORMATION.
        sf::IntRect texture_rectangle;
        texture_rectangle.top = static_cast<int>(texture_sub_rectangle.LeftTop.Y);
        texture_rectangle.left = static_cast<int>(texture_sub_rectangle.LeftTop.X);
        texture_rectangle.width = static_cast<int>(texture_sub_rectangle.Width());
        texture_rectangle.height = static_cast<int>(texture_sub_rectangle.Height());
        MEMORY::NonNullSharedPointer<GRAPHICS::Texture> texture = GraphicsDevice->GetTexture(texture_id);
        sf::Sprite gui_icon(texture->TextureResource, texture_rectangle);

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
        sf::IntRect texture_rectangle;
        texture_rectangle.left = (int)sprite.TextureSubRectangle.LeftTop.X;
        texture_rectangle.top = (int)sprite.TextureSubRectangle.LeftTop.Y;
        texture_rectangle.width = (int)sprite.TextureSubRectangle.Width();
        texture_rectangle.height = (int)sprite.TextureSubRectangle.Height();
        MEMORY::NonNullSharedPointer<GRAPHICS::Texture> texture = GraphicsDevice->GetTexture(sprite.TextureId);
        sf::Sprite gui_icon(texture->TextureResource, texture_rectangle);

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

    /// Renders a sprite to the screen.
    /// @param[in]  sprite - The sprite to render.
    void Renderer::Render(const Sprite& sprite)
    {
        // CONVERT THE SPRITE TO SFML FORMAT.
        sf::Sprite sfml_sprite;
        sfml_sprite.setColor(sf::Color(sprite.Color.Red, sprite.Color.Green, sprite.Color.Blue, sprite.Color.Alpha));
        sfml_sprite.setOrigin(sprite.Origin.X, sprite.Origin.Y);
        sfml_sprite.setPosition(sprite.WorldPosition.X, sprite.WorldPosition.Y);
        sfml_sprite.setRotation(sprite.RotationAngleInDegrees);
        sfml_sprite.setScale(sprite.Scale.X, sprite.Scale.Y);

        sf::IntRect texture_rectangle;
        texture_rectangle.left = (int)sprite.TextureSubRectangle.LeftTop.X;
        texture_rectangle.top = (int)sprite.TextureSubRectangle.LeftTop.Y;
        texture_rectangle.width = (int)sprite.TextureSubRectangle.Width();
        texture_rectangle.height = (int)sprite.TextureSubRectangle.Height();
        sfml_sprite.setTextureRect(texture_rectangle);

        MEMORY::NonNullSharedPointer<GRAPHICS::Texture> texture = GraphicsDevice->GetTexture(sprite.TextureId);
        sfml_sprite.setTexture(texture->TextureResource);

        // DRAW THE SPRITE.
        Screen->RenderTarget.draw(sfml_sprite);
    }

    /// Renders text to the screen.
    /// @param[in]  text - The text to render.
    void Renderer::Render(const GUI::Text& text)
    {
        // RENDER THE TEXT TO THE CONSOLE IF NO FONT EXISTS.
        // This is intended primarily to provide debug support.
        auto id_with_font = Fonts.find(text.FontId);
        bool font_exists = (id_with_font != Fonts.cend()) && (nullptr != id_with_font->second);
        if (!font_exists)
        {
            DEBUGGING::DebugConsole::WriteLine(text.String);
            return;
        }

        // RENDER EACH CHARACTER OF THE TEXT.
        auto font = id_with_font->second;
        MATH::Vector2f current_character_left_top_screen_position = text.LeftTopPosition;
        for (unsigned int character_index = 0; character_index < text.String.size(); ++character_index)
        {
            // GET THE GLYPH FOR THE CURRENT CHARACTER.
            const char character = text.String.at(character_index);
            GUI::Glyph& glyph = font->GlyphsByCharacter[static_cast<unsigned char>(character)];

            // CREATE A SPRITE FOR THE CURRENT GLYPH.
            sf::IntRect texture_rectangle = glyph.TextureSubRectangle.ToSfmlRectangle<int>();
            sf::Sprite current_character_sprite(glyph.Texture->TextureResource, texture_rectangle);
            current_character_sprite.setPosition(
                current_character_left_top_screen_position.X,
                current_character_left_top_screen_position.Y);
            current_character_sprite.setScale(text.ScaleFactor, text.ScaleFactor);

            // CONFIGURE THE RENDER TARGET FOR SCREEN-SPACE RENDERING.
            sf::View screen_space_view = Screen->RenderTarget.getDefaultView();
            Screen->RenderTarget.setView(screen_space_view);

            // RENDER THE CURRENT GLYPH.
            sf::RenderStates render_states = sf::RenderStates::Default;
            std::shared_ptr<sf::Shader> colored_text_shader = GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
            if (colored_text_shader)
            {
                render_states = ConfigureColoredTextShader(text.Color, *colored_text_shader);
            }
            Screen->RenderTarget.draw(current_character_sprite, render_states);

            // CALCULATE THE LEFT-TOP SCREEN POSITION OF THE NEXT CHARACTER.
            
            float glyph_width = glyph.TextureSubRectangle.Width();
            float scaled_glyph_width = text.ScaleFactor * glyph_width;
            current_character_left_top_screen_position.X += scaled_glyph_width + GUI::Text::HORIZONTAL_SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER;
        }
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
        for (unsigned int character_index = 0; character_index < text.size(); ++character_index)
        {
            // GET THE GLYPH FOR THE CURRENT CHARACTER.
            const char character = text.at(character_index);
            GUI::Glyph& glyph = font->GlyphsByCharacter[static_cast<unsigned char>(character)];

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
            sf::RenderStates render_states = sf::RenderStates::Default;
            std::shared_ptr<sf::Shader> colored_text_shader = GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
            if (colored_text_shader)
            {
                render_states = ConfigureColoredTextShader(text_color, *colored_text_shader);
            }
            Screen->RenderTarget.draw(current_character_sprite, render_states);

            // CALCULATE THE LEFT-TOP SCREEN POSITION OF THE NEXT CHARACTER.
            float glyph_width = glyph.TextureSubRectangle.Width();
            // If a space is being rendered, then the width is shortened for better readability.
            // This isn't done for other kinds of whitespace to better preserve distance for those.
            bool character_is_space = ' ' == character;
            if (character_is_space)
            {
                glyph_width /= 2.0f;
            }
            float scaled_glyph_width = text_scale_ratio * glyph_width;
            current_character_left_top_screen_position.X += scaled_glyph_width + GUI::Text::HORIZONTAL_SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER;
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
        std::vector<std::string> original_lines_from_text = STRING::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.Width();
        float glyph_width_in_pixels = text_scale_ratio * static_cast<float>(GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS) / 2.0f;
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / glyph_width_in_pixels);
        for (const auto& line : original_lines_from_text)
        {
            // SPLIT THE CURRENT LINE INTO INDIVIDUAL WORDS.
            std::deque<std::string> words_in_current_line = STRING::String::SplitIntoWords(
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
                    std::shared_ptr<GUI::Font>& font = Fonts[font_id];
                    GUI::Text current_line_text =
                    {
                        .String = current_new_line,
                        .FontId = font_id,
                        .LeftTopPosition = MATH::Vector2f(0.0f, 0.0f),
                        .ScaleFactor = text_scale_ratio,
                        .Color = text_color
                    };
                    size_t current_line_width_in_pixels = current_line_text.Width<size_t>(*font);
                    size_t space_character_width_in_pixels = GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS / 2;
                    GUI::Text next_word_text =
                    {
                        .String = next_word,
                        .FontId = font_id,
                        .LeftTopPosition = MATH::Vector2f(0.0f, 0.0f),
                        .ScaleFactor = text_scale_ratio,
                        .Color = text_color
                    };
                    size_t next_word_width_in_pixels = next_word_text.Width<size_t>(*font);
                    size_t line_width_with_next_word_in_characters = 
                        current_line_width_in_pixels + 
                        space_character_width_in_pixels + 
                        next_word_width_in_pixels;
                    bool current_line_can_handle_next_word = (line_width_with_next_word_in_characters <= line_width_in_pixels);
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
        float bounding_rectangle_left_x_screen_position = bounding_screen_rectangle.LeftTop.X;
        float bounding_rectangle_top_y_screen_position = bounding_screen_rectangle.LeftTop.Y;
        MATH::Vector2f current_line_left_top_screen_position(
            bounding_rectangle_left_x_screen_position,
            bounding_rectangle_top_y_screen_position);
        for (const auto& line : new_lines_of_text)
        {
            // RENDER THE CURRENT LINE.
            RenderText(line, font_id, current_line_left_top_screen_position, text_color, text_scale_ratio);

            // MOVE TO THE NEXT LINE.
            float glyph_height_in_pixels = text_scale_ratio * static_cast<float>(GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
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
        std::vector<std::string> original_lines_from_text = STRING::String::SplitIntoLines(text);

        // SPLIT EACH LINE BASED ON WORD BOUNDARIES.
        // New lines will be created that ensure that words aren't broken up across
        // lines (assuming that each word can fit on a single line).
        std::vector<std::string> new_lines_of_text;
        float line_width_in_pixels = bounding_screen_rectangle.Width();
        float glyph_width_in_pixels = text_scale_ratio * (static_cast<float>(GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS) / 2.0f + GUI::Text::HORIZONTAL_SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER);
        unsigned int max_characters_per_line = static_cast<unsigned int>(line_width_in_pixels / glyph_width_in_pixels);
        for (const auto& line : original_lines_from_text)
        {
            // SPLIT THE CURRENT LINE INTO INDIVIDUAL WORDS.
            std::deque<std::string> words_in_current_line = STRING::String::SplitIntoWords(
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
                    std::shared_ptr<GUI::Font>& font = Fonts[font_id];
                    GUI::Text current_line_text =
                    {
                        .String = current_new_line,
                        .FontId = font_id,
                        .LeftTopPosition = MATH::Vector2f(0.0f, 0.0f),
                        .ScaleFactor = text_scale_ratio,
                        .Color = text_color
                    };
                    size_t current_line_width_in_pixels = current_line_text.Width<size_t>(*font);
                    size_t space_character_width_in_pixels = GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS / 2;
                    GUI::Text next_word_text =
                    {
                        .String = next_word,
                        .FontId = font_id,
                        .LeftTopPosition = MATH::Vector2f(0.0f, 0.0f),
                        .ScaleFactor = text_scale_ratio,
                        .Color = text_color
                    };
                    size_t next_word_width_in_pixels = next_word_text.Width<size_t>(*font);
                    size_t line_width_with_next_word_in_characters = 
                        current_line_width_in_pixels + 
                        space_character_width_in_pixels + 
                        next_word_width_in_pixels;
                    bool current_line_can_handle_next_word = (line_width_with_next_word_in_characters <= line_width_in_pixels);
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
        float bounding_rectangle_left_x_screen_position = bounding_screen_rectangle.LeftTop.X;
        
        // The starting y-position for the first line is offset from the bounding rectangle's
        // top y-position such that half of the unused space appears before and after the text.
        unsigned int bounding_rectangle_height_in_pixels = static_cast<unsigned int>(bounding_screen_rectangle.Height());
        size_t new_line_count = new_lines_of_text.size();
        unsigned int glyph_height_in_pixels = static_cast<unsigned int>(text_scale_ratio * GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        size_t total_text_height_in_pixels = new_line_count * glyph_height_in_pixels;
        size_t unused_vertical_space_in_pixels = bounding_rectangle_height_in_pixels - total_text_height_in_pixels;
        size_t half_of_unused_vertical_space_in_pixels = unused_vertical_space_in_pixels / 2;
        float bounding_rectangle_top_y_screen_position = bounding_screen_rectangle.LeftTop.Y;
        float first_line_top_y_screen_position = bounding_rectangle_top_y_screen_position + half_of_unused_vertical_space_in_pixels;

        MATH::Vector2f current_line_left_top_screen_position(
            bounding_rectangle_left_x_screen_position,
            first_line_top_y_screen_position);

        // RENDER EACH LINE OF TEXT.
        std::shared_ptr<GUI::Font>& font = Fonts[font_id];
        for (const auto& line : new_lines_of_text)
        {
            // CREATE THE GUI FORM OF THE TEXT.
            GUI::Text current_line_of_text =
            {
                .String = line,
                .FontId = font_id,
                .LeftTopPosition = current_line_left_top_screen_position,
                .ScaleFactor = text_scale_ratio,
                .Color = text_color
            };

            // CENTER THE CURRENT LINE HORIZONTALLY.
            unsigned int current_line_width_in_pixels = current_line_of_text.Width<unsigned int>(*font);
            unsigned int bounding_rectangle_width_in_pixels = static_cast<unsigned int>(bounding_screen_rectangle.Width());
            unsigned int unused_space_on_current_line_in_pixels = bounding_rectangle_width_in_pixels - current_line_width_in_pixels;
            unsigned int half_of_unused_space_on_current_line_in_pixels = unused_space_on_current_line_in_pixels / 2;
            current_line_left_top_screen_position.X = bounding_screen_rectangle.LeftTop.X;
            current_line_left_top_screen_position.X += half_of_unused_space_on_current_line_in_pixels;

            // RENDER THE CURRENT LINE.
            RenderText(line, font_id, current_line_left_top_screen_position, text_color, text_scale_ratio);

            // MOVE TO THE NEXT LINE.
            current_line_left_top_screen_position.Y += glyph_height_in_pixels;
        }
    }

    /// Renders text within a bordered box.
    /// \param[in]  text - The text to render.
    /// \param[in]  font_id - The ID of the font to use for the text.
    /// \param[in]  text_color - The color of the text.
    /// \param[in]  text_padding_in_pixels_from_border - The padding of the text from each side of the border.
    /// \param[in]  bounding_screen_rectangle - The bounding screen rectangle for the
    ///     box containing the box (and thus defining the border).
    /// \param[in]  border_color - The color for the border.
    /// \param[in]  border_thickness_in_pixels - The thickness of the border in pixels.
    void Renderer::RenderTextInBorderedBox(
        const std::string& text,
        const RESOURCES::AssetId font_id,
        const Color& text_color,
        const MATH::Vector2f& text_padding_in_pixels_from_border,
        const MATH::FloatRectangle& bounding_screen_rectangle,
        const Color& border_color,
        const float border_thickness_in_pixels)
    {
        // RENDER THE BORDER FOR THE BOX.
        sf::RectangleShape border;

        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color(border_color.Red, border_color.Green, border_color.Blue));
        border.setOutlineThickness(border_thickness_in_pixels);

        float box_width_in_pixels = bounding_screen_rectangle.Width();
        float box_height_in_pixels = bounding_screen_rectangle.Height();
        border.setSize(sf::Vector2f(box_width_in_pixels, box_height_in_pixels));

        float box_left_x_position_in_pixels = bounding_screen_rectangle.LeftTop.X;
        float box_top_y_position_in_pixels = bounding_screen_rectangle.LeftTop.Y;
        border.setPosition(box_left_x_position_in_pixels, box_top_y_position_in_pixels);

        Screen->RenderTarget.draw(border);

        // RENDER THE TEXT.
        float text_left_x_position_in_pixels = box_left_x_position_in_pixels + text_padding_in_pixels_from_border.X;
        float text_top_y_position_in_pixels = box_top_y_position_in_pixels + text_padding_in_pixels_from_border.Y;
        float text_box_width_in_pixels = box_width_in_pixels - text_padding_in_pixels_from_border.X;
        float text_box_height_in_pixels = box_height_in_pixels - text_padding_in_pixels_from_border.Y;
        MATH::FloatRectangle text_bounding_screen_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            text_left_x_position_in_pixels,
            text_top_y_position_in_pixels,
            text_box_width_in_pixels,
            text_box_height_in_pixels);
        RenderText(
            text,
            font_id,
            text_bounding_screen_rectangle,
            text_color);
    }

    /// Renders a multi-tile-map grid.
    /// @param[in]  tile_map_grid - The tile map grid to render.
    void Renderer::Render(const MAPS::MultiTileMapGrid& tile_map_grid)
    {
        MATH::FloatRectangle camera_bounds = Camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.Center();

        /// @note   This view only needs to be set here.
        /// Private methods assume it has already been set.
        sf::View camera_view;
        camera_view.setCenter(camera_view_center.X, camera_view_center.Y);
        camera_view.setSize(camera_bounds.Width(), camera_bounds.Height());
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
                Render(tile->Sprite.CurrentFrameSprite);
            }
        }
        
        // RENDER THE CURRENT TILE MAP'S ARK PIECES.
        for (const auto& ark_piece : tile_map.ArkPieces)
        {
            // Only ark pieces that have been built should be visible.
            if (ark_piece.Built)
            {
                Render(ark_piece.Sprite);
            }

        }

        // RENDER THE CURRENT TILE MAP'S WOOD LOGS.
        for (const auto& wood_log : tile_map.WoodLogs)
        {
            Render(wood_log.Sprite);
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            // RENDER THE TREE.
            Render(tree.Sprite.CurrentFrameSprite);

            // RENDER ANY FOOD ON THE TREE.
            if (tree.Food)
            {
                Render(tree.Food->Sprite);
            }
        }

        // RENDER THE CURRENT TILE MAP'S FALLING FOOD.
        for (const auto& food : tile_map.FallingFood)
        {
            Render(food.FoodItem.Sprite);
        }

        // RENDER THE CURRENT TILE MAP'S FOOD ON THE GROUND.
        for (const auto& food : tile_map.FoodOnGround)
        {
            Render(food.Sprite);
        }

        // RENDER THE CURRENT TILE MAP'S ROAMING ANIMALS.
        for (const auto& animal : tile_map.RoamingAnimals)
        {
            Render(animal->Sprite.CurrentFrameSprite);
        }

        // RENDER THE CURRENT TILE MAP'S ANIMALS IN PENS.
        for (const auto& animal_pen : tile_map.AnimalPens)
        {
            for (const auto& animal : animal_pen.Animals)
            {
                Render(animal->Sprite.CurrentFrameSprite);
            }
        }

        // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
        for (const auto& dust_cloud : tile_map.DustClouds)
        {
            Render(dust_cloud.Sprite.CurrentFrameSprite);
        }

        // RENDER ANY FLAMING SWORD.
        if (tile_map.FlamingSword)
        {
            Render(tile_map.FlamingSword->Sprite);
        }
    }

    /// Configures the colored text shader to render text using the specified color,
    /// returning the corresponding render states.
    /// @param[in]  color - The color for the text to render.
    /// @param[in,out]  colored_text_shader - The shader to configure.
    /// @return The render states for the configured colored text shader.
    sf::RenderStates Renderer::ConfigureColoredTextShader(const Color& color, sf::Shader& colored_text_shader)
    {
        // CONFIGURE THE SHADER IN THE RENDER STATES.
        sf::RenderStates render_states = sf::RenderStates::Default;
        colored_text_shader.setUniform("color", sf::Glsl::Vec4(sf::Color(color.Red, color.Green, color.Blue, color.Alpha)));
        colored_text_shader.setUniform("texture", sf::Shader::CurrentTexture);
        render_states.shader = &colored_text_shader;
        return render_states;
    }
}
