#include <Windows.h>
#include "Graphics/Gui/Font.h"

namespace GRAPHICS::GUI
{
    /// Loads the a default font from the system.
    /// @param[in]  font_id - The ID of the font to load; see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject
    ///     for valid values.
    /// @return The requested font, if successfully loaded; std::nullopt otherwise.
    std::shared_ptr<Font> Font::LoadSystemDefaultFont(const int font_id)
    {
        /// @todo   Error handling for return values?  Thus far, hasn't been too important...

        // GET A DEVICE CONTEXT IN WHICH TO LOAD THE FONT.
        HDC entire_screen_device_context = GetDC(NULL);
        HDC font_device_context = CreateCompatibleDC(entire_screen_device_context);

        // CREATE A BITMAP FOR RENDERING THE FONT.
        // This is necessary to be able to successfully write and retrieve pixels.
        BITMAPINFO bitmap_info = {};
        bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
        bitmap_info.bmiHeader.biWidth = FONT_BITMAP_DIMENSION_IN_PIXELS;
        // Height is negative to use a typical top-down bitmap.
        bitmap_info.bmiHeader.biHeight = -static_cast<long>(FONT_BITMAP_DIMENSION_IN_PIXELS);
        // Must be 1 according to Microsoft documentation.
        bitmap_info.bmiHeader.biPlanes = 1;
        // We use 32-bit color.
        bitmap_info.bmiHeader.biBitCount = 32;
        // Uncompressed, raw RGB colors are used.
        bitmap_info.bmiHeader.biCompression = BI_RGB;
        // Microsoft documentation say this "may be 0" for RGB bitmaps.
        bitmap_info.bmiHeader.biSizeImage = 0;
        // 0 indicates using the maximum number of colors.
        bitmap_info.bmiHeader.biClrUsed = 0;
        // 0 indicates all colors are required.
        bitmap_info.bmiHeader.biClrImportant = 0;

        // We don't actually need these raw bit values, but this parameter is required for the function below.
        void* font_bits_unused = nullptr;
        constexpr HANDLE ALLOCATE_MEMORY_RATHER_THAN_USE_FILE_MAPPING = nullptr;
        constexpr DWORD NO_FILE_MAPPING_OFFSET = 0;
        HBITMAP font_bitmap = CreateDIBSection(
            font_device_context,
            &bitmap_info,
            DIB_RGB_COLORS,
            &font_bits_unused,
            ALLOCATE_MEMORY_RATHER_THAN_USE_FILE_MAPPING,
            NO_FILE_MAPPING_OFFSET);
        SelectObject(font_device_context, font_bitmap);

        // SET A BACKGROUND COLOR FOR THE BITMAP.
        // Black is arbitrarily chosen for now.
        const COLORREF BACKGROUND_COLOR = RGB(0, 0, 0);
        SetBkColor(font_device_context, BACKGROUND_COLOR);

        // GET A HANDLE TO THE SYSTEM FIXED FONT.
        std::shared_ptr<Font> font = std::make_shared<Font>();
        HGDIOBJ font_handle = GetStockObject(font_id);

        // ENABLE USING THE FONT.
        SelectObject(font_device_context, font_handle);

        // GET THE BASIC TEXT METRICS FOR THE FONT.
        TEXTMETRIC text_metrics;
        GetTextMetrics(font_device_context, &text_metrics);
        long glyph_width = text_metrics.tmMaxCharWidth;
        long glyph_height = text_metrics.tmHeight;

        // CREATE AN IMAGE TO RENDER TO.
        // All images are expected to use magenta (R=255, G=0, B=255) for their parts
        // that should be transparent.
        const sf::Color TRANSPARENT_COLOR = sf::Color::Magenta;
        constexpr uint8_t MAKE_COMPLETELY_TRANSPARENT = 0;
        sf::Image font_image;
        font_image.create(FONT_BITMAP_DIMENSION_IN_PIXELS, FONT_BITMAP_DIMENSION_IN_PIXELS, TRANSPARENT_COLOR);
        font_image.createMaskFromColor(TRANSPARENT_COLOR, MAKE_COMPLETELY_TRANSPARENT);

        // RENDER ALL GLYPHS FOR THE FONT.
        // White is used as a contrast that allows easily altering colors.
        const COLORREF FONT_COLOR = RGB(255, 255, 255);
        SetTextColor(font_device_context, FONT_COLOR);
        RECT current_glyph_rectangle =
        {
            .left = 0,
            .top = 0,
            .right = glyph_width,
            .bottom = glyph_height
        };
        // Unsigned ints are used to avoid infinite wraparound that would occur if using chars directly.
        for (unsigned int character_value = 0; character_value < CHARACTER_COUNT; ++character_value)
        {
            // DRAW THE CURRENT CHARACTER.
            constexpr int SINGLE_CHARACTER_COUNT = 1;
            char character = static_cast<char>(character_value);
            DrawText(font_device_context, &character, SINGLE_CHARACTER_COUNT, &current_glyph_rectangle, DT_CENTER);

            // GET ALL PIXELS FOR THE CURRENT GLYPH.
            for (int y = current_glyph_rectangle.top; y <= current_glyph_rectangle.bottom; ++y)
            {
                for (int x = current_glyph_rectangle.left; x <= current_glyph_rectangle.right; ++x)
                {
                    // ONLY STORE THE PIXEL COLOR IF SOMETHING WAS DRAWN.
                    // If the pixel wasn't written at all, the result will be all zeroes.
                    // Since it only contains RGB colors, that means the alpha will also be 0.
                    COLORREF pixel_color = GetPixel(font_device_context, x, y);
                    if (pixel_color)
                    {
                        // The COLORREF is in ARGB format.
                        constexpr DWORD RED_COLOR_COMPONENT_MASK = 0x00FF0000;
                        constexpr DWORD RED_COLOR_COMPONENT_SHIFT_AMOUNT = 16;
                        uint8_t red_color_component = static_cast<uint8_t>((pixel_color & RED_COLOR_COMPONENT_MASK) >> RED_COLOR_COMPONENT_SHIFT_AMOUNT);

                        constexpr DWORD GREEN_COLOR_COMPONENT_MASK = 0x0000FF00;
                        constexpr DWORD GREEN_COLOR_COMPONENT_SHIFT_AMOUNT = 8;
                        uint8_t green_color_component = (pixel_color & GREEN_COLOR_COMPONENT_MASK) >> GREEN_COLOR_COMPONENT_SHIFT_AMOUNT;

                        constexpr DWORD BLUE_COLOR_COMPONENT_MASK = 0x000000FF;
                        uint8_t blue_color_component = (pixel_color & BLUE_COLOR_COMPONENT_MASK);

                        sf::Color color(red_color_component, green_color_component, blue_color_component);
                        font_image.setPixel(x, y, color);
                    }
                }
            }

            // STORE THE GLYPH FOR THE CURRENT CHARACTER.
            font->GlyphsByCharacter[static_cast<unsigned char>(character)] = Glyph
            {
                .Character = character,
                .Texture = font->Texture,
                .TextureSubRectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    static_cast<float>(current_glyph_rectangle.left),
                    static_cast<float>(current_glyph_rectangle.top),
                    static_cast<float>(glyph_width),
                    static_cast<float>(glyph_height))
            };

            // MOVE TO THE NEXT CHARACTER.
            current_glyph_rectangle.left += glyph_width;
            current_glyph_rectangle.right += glyph_width;
            bool current_row_of_glyphs_filled = current_glyph_rectangle.right >= FONT_BITMAP_DIMENSION_IN_PIXELS;
            if (current_row_of_glyphs_filled)
            {
                // MOVE TO THE NEXT ROW FOR DRAWING.
                current_glyph_rectangle.left = 0;
                current_glyph_rectangle.right = glyph_width;
                current_glyph_rectangle.top += glyph_height;
                current_glyph_rectangle.bottom += glyph_height;
            }
        }

        // FREE ALLOCATED RESOURCES.
        DeleteObject(font_bitmap);
        DeleteDC(font_device_context);

        // RETURN THE FONT.
        font->Texture->TextureResource.loadFromImage(font_image);
        return font;
    }
}
