#pragma once

#include <string>

/// Holds code to assist with debugging.
namespace DEBUGGING
{
    /// A console for printing text for debugging.
    /// Currently uses the standard console output stream.
    class DebugConsole
    {
    public:
        static void WriteLine(const std::string& text);
    };
}
