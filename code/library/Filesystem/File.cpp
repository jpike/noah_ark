#include <fstream>
#include "Filesystem/File.h"

namespace FILESYSTEM
{
    /// Attempts to read all data in binary format from the specified file.
    /// @param[in]  path - The path of the file to read.
    /// @return All binary data from the file, if successfully read; empty otherwise.
    std::string File::ReadBinary(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        auto beginning_of_file = std::istreambuf_iterator<char>(file);
        auto end_of_file = std::istreambuf_iterator<char>();
        std::string binary_data(beginning_of_file, end_of_file);
        return binary_data;
    }
}
