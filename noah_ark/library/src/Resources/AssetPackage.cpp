#include <cstdint>
#include <fstream>
#include <string>
#include "Filesystem/File.h"
#include "Resources/AssetPackage.h"

namespace RESOURCES
{
    /// Attempts to read an asset package from file.
    /// @param[in]  filepath - The path of the file to read from.
    ///     May be relative or absolute.
    /// @return The assets from the package, if read successfully from the file;
    ///     empty otherwise.
    std::vector<Asset> AssetPackage::ReadFile(const std::filesystem::path& filepath)
    {
        // TRY OPENING THE FILE.
        std::ifstream asset_package_file(filepath, std::ios::binary | std::ios::in);
        bool file_opened = asset_package_file.is_open();
        if (!file_opened)
        {
            // No assets could be read.
            return {};
        }

        // READ IN ALL ASSETS FROM THE FILE.
        std::vector<Asset> assets;

        auto more_assets_in_file = [](std::ifstream& file) -> bool
        {
            bool more_data_in_file = !file.eof() && !file.bad() && !file.fail();
            return more_data_in_file;
        };
        while (more_assets_in_file(asset_package_file))
        {
            // READ IN THE NEXT ASSET.
            std::optional<Asset> asset = Asset::Read(asset_package_file);
            if (asset)
            {
                assets.emplace_back(*asset);
            }
        }

        return assets;
    }

    /// Attempts to write an asset package to the specified file.
    /// The file will be overwritten if it exists.
    /// @param[in]  asset_package_definition - A definition of the assets to be included
    ///     in the file.
    /// @param[in]  filepath - The path of the file to write to.
    ///     May be relative or absolute.
    /// @return True if the asset package is successfully saved to file; false otherwise.
    bool AssetPackage::WriteFile(const AssetPackageDefinition& asset_package_definition, const std::filesystem::path& filepath)
    {
        // TRY OPENING THE FILE.
        std::ofstream asset_package_file(filepath, std::ios::binary | std::ios::out | std::ios::trunc);
        bool file_opened = asset_package_file.is_open();
        if (!file_opened)
        {
            return false;
        }

        // WRITE EACH ASSET TO THE PACKAGE FILE.
        for (const auto& asset : asset_package_definition.Assets)
        {
            /// @todo   Figure out what to do for shaders.
            /// We're skipping over them for now since we don't have them stored in separate files.
            bool asset_is_shader = (AssetType::SHADER == asset.Type);
            if (asset_is_shader)
            {
                continue;
            }

            // READ IN THE ASSET FILE'S DATA.
            std::string asset_data = FILESYSTEM::File::ReadBinary(asset.Filepath);
            bool asset_data_read = !asset_data.empty();
            if (!asset_data_read)
            {
                return false;
            }

            // WRITE OUT THE ASSET'S METADATA.
            // The raw binary form of each metadata attribute is written.
            // This metadata is needed to properly read back in the data
            // and make it usable for the game.
            asset_package_file.write(reinterpret_cast<const char*>(&asset.Type), sizeof(asset.Type));
            asset_package_file.write(reinterpret_cast<const char*>(&asset.Id), sizeof(asset.Id));
            std::string::size_type asset_size_in_bytes = asset_data.size();
            asset_package_file.write(reinterpret_cast<const char*>(&asset_size_in_bytes), sizeof(asset_size_in_bytes));

            // WRITE OUT THE ASSET'S ACTUAL DATA.
            asset_package_file.write(asset_data.data(), asset_data.size());

            // CHECK IF ANY ERRORS OCCURRED.
            bool file_writing_failed = asset_package_file.bad() || asset_package_file.fail();
            if (file_writing_failed)
            {
                return false;
            }
        }

        // INDICATE THAT THE ASSET FILE WAS SUCCESSFULLY LOADED.
        return true;
    }
}
