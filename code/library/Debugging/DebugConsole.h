#pragma once

#include <string>
#include "Graphics/Color.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

/// Holds code to assist with debugging.
namespace DEBUGGING
{
    /// A console for printing text for debugging.
    /// Currently uses the standard console streams.
    class DebugConsole
    {
    public:
        // STANDARD OUTPUT WRITING.
        static void WriteLine(const std::string& text);
        template <typename PrimitiveType>
        static void WriteLine(const std::string& prefix_text, const PrimitiveType value);
        template <typename ComponentType>
        static void WriteLine(const std::string& prefix_text, const MATH::Vector2<ComponentType>& vector);
        template <typename ComponentType>
        static void WriteLine(const std::string& prefix_text, const MATH::Rectangle<ComponentType>& rectangle);
        static void WriteLine(const std::string& prefix_text, const GRAPHICS::Color& color);

        // STANDARD ERROR WRITING.
        static void WriteErrorLine(const std::string& text);
        template <typename PrimitiveType>
        static void WriteErrorLine(const std::string& prefix_text, const PrimitiveType value);
    };

    /// Writes a line of text to the console with the primitive value printed at the end.
    /// @tparam PrimitiveType - The primitive type of the value to write out.
    /// @param[in]  prefix_text - The text to write before the value.
    /// @param[in]  value - The primitive value to write out.
    template <typename PrimitiveType>
    void DebugConsole::WriteLine(const std::string& prefix_text, const PrimitiveType value)
    {
#ifdef _DEBUG
        WriteLine(prefix_text + std::to_string(value));
#else
        // Reference to avoid compiler warnings.
        prefix_text;
        value;
#endif
    }

    /// Writes a line of text to the console with the specified vector printed at the end.
    /// @tparam ComponentType - The type of the component in the vector.
    /// @param[in]  prefix_text - The text to write before the vector.
    /// @param[in]  vector - The vector to write out.
    template <typename ComponentType>
    void DebugConsole::WriteLine(const std::string& prefix_text, const MATH::Vector2<ComponentType>& vector)
    {
#ifdef _DEBUG
        std::string vector_text =
            "(" +
            std::to_string(vector.X) +
            "," +
            std::to_string(vector.Y) +
            ")";

        WriteLine(prefix_text + vector_text);
#else
        // Reference to avoid compiler warnings.
        prefix_text;
        vector;
#endif
    }

    /// Writes a line of text to the console with the specified rectangle printed at the end.
    /// @tparam ComponentType - The type of the component in the rectangle.
    /// @param[in]  prefix_text - The text to write before the rectamg;e.
    /// @param[in]  rectangle - The rectangle to write out.
    template <typename ComponentType>
    void DebugConsole::WriteLine(const std::string& prefix_text, const MATH::Rectangle<ComponentType>& rectangle)
    {
#ifdef _DEBUG
        std::string rectangle_text =
            std::to_string(rectangle.LeftTop.X) + " " +
            std::to_string(rectangle.LeftTop.Y) + " " +
            std::to_string(rectangle.RightBottom.X) + " " +
            std::to_string(rectangle.RightBottom.Y);

        WriteLine(prefix_text + rectangle_text);
#else
        // Reference to avoid compiler warnings.
        prefix_text;
        rectangle;
#endif
    }

    /// Writes a line of text to the error console with the primitive value printed at the end.
    /// @tparam PrimitiveType - The primitive type of the value to write out.
    /// @param[in]  prefix_text - The text to write before the value.
    /// @param[in]  value - The primitive value to write out.
    template <typename PrimitiveType>
    void DebugConsole::WriteErrorLine(const std::string& prefix_text, const PrimitiveType value)
    {
#ifdef _DEBUG
        WriteErrorLine(prefix_text + std::to_string(value));
#else
        // Reference to avoid compiler warnings.
        prefix_text;
        value;
#endif
    }
}
