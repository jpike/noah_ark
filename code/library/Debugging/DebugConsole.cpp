#include <iostream>
#include "Debugging/DebugConsole.h"

namespace DEBUGGING
{
    /// Writes a line of text to the console.
    /// @param[in]  text - The text to write.
    void DebugConsole::WriteLine(const std::string& text)
    {
#ifdef _DEBUG
        std::cout << text << std::endl;
#else
        // Reference to avoid compiler warnings.
        text;
#endif
    }

    /// Write a line of text for a color to the console.
    /// @param[in]  prefix_text - The text to write before the color.
    /// @param[in]  color - The color to write out.
    void DebugConsole::WriteLine(const std::string& prefix_text, const GRAPHICS::Color& color)
    {
#ifdef _DEBUG
        std::string vector_text =
            "(" +
            std::to_string(color.Red) +
            "," +
            std::to_string(color.Green) +
            "," +
            std::to_string(color.Blue) +
            "," +
            std::to_string(color.Alpha) +
            ")";

        WriteLine(prefix_text + vector_text);
#else
        // Reference to avoid compiler warnings.
        prefix_text;
        color;
#endif
    }

    /// Writes a line of error text to the console.
    /// @param[in]  text - The text to write.
    void DebugConsole::WriteErrorLine(const std::string& text)
    {
#ifdef _DEBUG
        std::cerr << text << std::endl;
#else
        // Reference to avoid compiler warnings.
        text;
#endif
    }
}
