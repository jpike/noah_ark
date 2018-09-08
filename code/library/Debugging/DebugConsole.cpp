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
#endif
    }

    /// Writes a line of error text to the console.
    /// @param[in]  text - The text to write.
    void DebugConsole::WriteErrorLine(const std::string& text)
    {
#ifdef _DEBUG
        std::cerr << text << std::endl;
#endif
    }
}