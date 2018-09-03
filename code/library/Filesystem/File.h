#pragma once

#include <filesystem>
#include <string>

/// Holds code for interacting with computer filesystems.
namespace FILESYSTEM
{
    /// A single file containing data.
    class File
    {
    public:
        static std::string ReadBinary(const std::filesystem::path& path);
    };
}
