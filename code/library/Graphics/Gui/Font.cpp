#include <Windows.h>
#include "Debugging/DebugConsole.h"
#include "ErrorHandling/ErrorMessageBox.h"
#include "Graphics/Gui/Font.h"

namespace GRAPHICS::GUI
{
    /// Loads the a default font from the system.
    /// @param[in]  font_id - The ID of the font to load; see https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject
    ///     for valid values.
    /// @return The requested font, if successfully loaded; null otherwise.
    std::shared_ptr<Font> Font::LoadSystemDefaultFont(const int font_id)
    {
        // GET A DEVICE CONTEXT IN WHICH TO LOAD THE FONT.
        HDC entire_screen_device_context = GetDC(NULL);
        if (!entire_screen_device_context)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - GetDC() failed.");
            return nullptr;
        }
        HDC font_device_context = CreateCompatibleDC(entire_screen_device_context);
        if (!font_device_context)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - CreateCompatibleDC() failed.");
            return nullptr;
        }

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
        if (!font_bitmap)
        {
            // Note that there is some uncertainly about the error handling in this function (so it might be better to remove).
            // This is the only place where getting the last error is documented as being used,
            // which is why the error message box is only displayed here, though it may work in other places.
            ERROR_HANDLING::ErrorMessageBox::DisplayLastErrorMessage();
            return nullptr;
        }
        HGDIOBJ bitmap_object_being_replaced = SelectObject(font_device_context, font_bitmap);
        if (!bitmap_object_being_replaced)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - SelectObject() for font bitmap failed.");
            return nullptr;
        }

        // SET A BACKGROUND COLOR FOR THE BITMAP.
        // Black is arbitrarily chosen for now.
        const COLORREF BACKGROUND_COLOR = RGB(0, 0, 0);
        COLORREF replaced_background_color = SetBkColor(font_device_context, BACKGROUND_COLOR);
        bool background_color_set = (CLR_INVALID != replaced_background_color);
        if (!background_color_set)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - SetBkColor() failed.");
            return nullptr;
        }

        // GET A HANDLE TO THE SYSTEM FIXED FONT.
        std::shared_ptr<Font> font = std::make_shared<Font>();
        HGDIOBJ font_handle = GetStockObject(font_id);
        if (!font_handle)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - GetStockObject() failed.");
            return nullptr;
        }

        // ENABLE USING THE FONT.
        HGDIOBJ font_object_being_replaced = SelectObject(font_device_context, font_handle);
        if (!font_object_being_replaced)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - SelectObject() for font failed.");
            return nullptr;
        }

        // GET THE BASIC TEXT METRICS FOR THE FONT.
        TEXTMETRIC text_metrics;
        BOOL text_metrics_retrieved = GetTextMetrics(font_device_context, &text_metrics);
        if (!text_metrics_retrieved)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - GetTextMetrics() for font failed.");
            return nullptr;
        }
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
        COLORREF replaced_text_color = SetTextColor(font_device_context, FONT_COLOR);
        bool text_color_set = (CLR_INVALID != replaced_text_color);
        if (!text_color_set)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - SetTextColor() failed.");
            return nullptr;
        }
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
            int drawn_text_height = DrawText(font_device_context, &character, SINGLE_CHARACTER_COUNT, &current_glyph_rectangle, DT_CENTER);
            if (!drawn_text_height)
            {
                DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - DrawText() failed.");
                return nullptr;
            }

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
        // Not much we can do if these fail other than debug logging.
        int font_bitmap_freed = DeleteObject(font_bitmap);
        if (!font_bitmap_freed)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - DeleteObject() failed.");
        }
        BOOL font_device_context_freed = DeleteDC(font_device_context);
        if (!font_device_context_freed)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - DeleteDC() failed.");
        }

        // RETURN THE FONT.
        bool font_loaded_from_texture = font->Texture->TextureResource.loadFromImage(font_image);
        if (!font_loaded_from_texture)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Font::LoadSystemDefaultFont() - loadFromImage() failed.");
            return nullptr;
        }
        return font;
    }
}
