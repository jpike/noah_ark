#include <cstdlib>
#include <iostream>
#include "Resources/AssetPackage.h"
#include "Resources/PredefinedAssetPackages.h"

/// A basic asset packer for the Noah's ark game.
/// Packs assets into a smaller number of asset package files to simplify and
/// speed up the process of asset loading.
/// @return 0 for success; non-zero for failure.
int main()
{
    // SAVE THE INTRO SEQUENCE ASSETS TO FILE.
    bool intro_asset_file_created = RESOURCES::AssetPackage::WriteFile(
        RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_DEFINITION,
        RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);
    if (!intro_asset_file_created)
    {
        std::cerr << "Failed to create intro sequence asset package file." << std::endl;
        return EXIT_FAILURE;
    }

    // SAVE ALL REMAINING ASSETS TO FILE.
    bool main_asset_file_created = RESOURCES::AssetPackage::WriteFile(
        RESOURCES::MAIN_ASSET_PACKAGE_DEFINITION,
        RESOURCES::MAIN_ASSET_PACKAGE_FILENAME);
    if (!main_asset_file_created)
    {
        std::cerr << "Failed to create main asset package file." << std::endl;
        return EXIT_FAILURE;
    }

    // INDICATE THAT ALL ASSET PACKAGES WERE SUCCESSFULLY CREATED.
    std::cout << "Asset package files created successfully." << std::endl;
    return EXIT_SUCCESS;
}

