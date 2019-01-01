#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>

/// A basic utility to generate code for font metrics for the Noah's ark game.
/// This is automated to help simplify font-rendering.
/// @return 0 for success; non-zero for failure.
int main()
{
    // OPEN UP THE FONT IMAGE FILE.
    // The serif one is used for now since it is assumed to have some slightly
    // wider characters, but the differences aren't important enough yet to be
    // worth calculating different metrics for the sans-serif font as well.
    const std::string FONT_IMAGE_FILEPATH = "assets/res/images/main_font_serif.png";
    sf::Image font_image;
    bool font_image_loaded = font_image.loadFromFile(FONT_IMAGE_FILEPATH);
    if (!font_image_loaded)
    {
        std::cerr << "Failed to load font image." << std::endl;
        return EXIT_FAILURE;
    }

    // WRITE OUT THE INITIAL C++ CODE FOR THE GLYPH DATA.
    // It can be redirected to file to have it output to an actual file.
    std::cout
        << "#include \"Graphics/Gui/Glyph.h\"" << std::endl
        << std::endl
        << "namespace GRAPHICS" << std::endl
        << "{" << std::endl
        << "namespace GUI" << std::endl
        << "{" << std::endl
        << "    const std::array<MATH::FloatRectangle, Glyph::TOTAL_COUNT> Glyph::TEXTURE_SUB_RECTANGLES = " << std::endl
        << "    {" << std::endl;

    // CALCULATE FONT METRICS FOR EACH CHARACTER.
    // An unsigned short must be used for loop iteration in order to avoid an infinite loop
    // caused by numeric wrap-around for the last character.
    for (unsigned short character = 0;
        character <= std::numeric_limits<unsigned char>::max();
        ++character)
    {
        // CALCULATE THE COORDINATES OF THE GLYPH'S BOUNDING BOX.
        constexpr char CHARACTERS_PER_ROW_COUNT = 16;
        constexpr unsigned int GLYPH_MAX_WIDTH_IN_PIXELS = 16;
        constexpr unsigned int SINGLE_PIXEL_RELATED_TO_DOUBLE_COUNTING = 1;
        unsigned int character_horizontal_index = character % CHARACTERS_PER_ROW_COUNT;
        unsigned int glyph_box_left_position = character_horizontal_index * GLYPH_MAX_WIDTH_IN_PIXELS;
        unsigned int glyph_box_right_position = glyph_box_left_position + GLYPH_MAX_WIDTH_IN_PIXELS - SINGLE_PIXEL_RELATED_TO_DOUBLE_COUNTING;

        constexpr unsigned int GLYPH_MAX_HEIGHT_IN_PIXELS = 16;
        unsigned int character_vertical_index = character / CHARACTERS_PER_ROW_COUNT;
        unsigned int glyph_box_top_position = character_vertical_index * GLYPH_MAX_HEIGHT_IN_PIXELS;
        unsigned int glyph_box_bottom_position = glyph_box_top_position + GLYPH_MAX_HEIGHT_IN_PIXELS - SINGLE_PIXEL_RELATED_TO_DOUBLE_COUNTING;

        // FIND THE MIN/MAX BOUNDARIES FOR THE CHARACTER'S GLYPH.
        // The min/max coordinates are initialized to values such that they will always
        // be overwritten if any glyph data is found.
        // A separate boolean variable is used to determine if these variables are updated
        // (since some glyphs may be blank).
        bool glyph_data_found = false;
        unsigned int glyph_min_x = glyph_box_right_position;
        unsigned int glyph_max_x = glyph_box_left_position;
        for (unsigned int current_glyph_y = glyph_box_top_position;
            current_glyph_y <= glyph_box_bottom_position;
            ++current_glyph_y)
        {
            for (unsigned int current_glyph_x = glyph_box_left_position;
                current_glyph_x <= glyph_box_right_position;
                ++current_glyph_x)
            {
                // CHECK IF THE CURRENT PIXEL CONTAINS GLYPH DATA OR THE BACKGROUND.
                // The glyph data should be surrounded by a color used for transparency.
                const sf::Color TRANSPARENT_COLOR = sf::Color::Magenta;
                sf::Color current_pixel_color = font_image.getPixel(current_glyph_x, current_glyph_y);
                bool current_pixel_contains_glyph_data = (TRANSPARENT_COLOR != current_pixel_color);
                if (current_pixel_contains_glyph_data)
                {
                    // INDICATE THAT GLYPH DATA WAS FOUND.
                    glyph_data_found = true;

                    // UPDATE THE MIN/MAX BOUNDARIES OF THE GLYPH.
                    glyph_min_x = std::min(glyph_min_x, current_glyph_x);
                    glyph_max_x = std::max(glyph_max_x, current_glyph_x);
                }
            }
        }

        // FINALIZE THE METRICS FOR THE GLYPH.
        if (!glyph_data_found)
        {
            glyph_min_x = glyph_box_left_position;
            glyph_max_x = glyph_box_right_position;
        }

        unsigned int glyph_width_in_pixels = glyph_max_x - glyph_min_x + SINGLE_PIXEL_RELATED_TO_DOUBLE_COUNTING;

        // OUTPUT THE C++ CODE FOR THE GLYPH.
        std::cout
            << "        MATH::FloatRectangle::FromLeftTopAndDimensions(" << std::endl
            << "            " << glyph_min_x << ".0f," << std::endl
            << "            " << glyph_box_top_position << ".0f," << std::endl
            << "            " << glyph_width_in_pixels << ".0f," << std::endl
            << "            static_cast<float>(Glyph::MAX_HEIGHT_IN_PIXELS))," << std::endl;
    }

    // WRITE OUT THE FINAL GLYPH DATA FOR THE C++ CODE.
    std::cout
        << "    };" << std::endl
        << "}" << std::endl
        << "}" << std::endl;

    // INDICATE THAT ALL THE FONT METRICS WERE SUCCESSFULLY CALCULATED.
    return EXIT_SUCCESS;
}

