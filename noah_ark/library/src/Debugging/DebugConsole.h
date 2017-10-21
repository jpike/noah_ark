#pragma once

#include <string>
#include "Math/Vector2.h"

/// Holds code to assist with debugging.
namespace DEBUGGING
{
    /// A console for printing text for debugging.
    /// Currently uses the standard console output stream.
    class DebugConsole
    {
    public:
        static void WriteLine(const std::string& text);
        template <typename ComponentType>
        static void WriteLine(const std::string& prefix_text, const MATH::Vector2<ComponentType>& vector);
    };

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
#endif
    }
}
