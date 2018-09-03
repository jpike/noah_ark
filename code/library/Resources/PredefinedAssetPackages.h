#pragma once

#include <string>
#include "Resources/AssetPackageDefinition.h"

namespace RESOURCES
{
    /// The filename for the intro sequence asset package.
    extern const std::string INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME;
    /// The package with all assets needed for the intro sequence.
    extern const AssetPackageDefinition INTRO_SEQUENCE_ASSET_PACKAGE_DEFINITION;

    /// The filename for the main asset package.
    extern const std::string MAIN_ASSET_PACKAGE_FILENAME;
    /// The package with all remaining assets for the game.
    /// Assets from the intro sequence package aren't repeated here.
    extern const AssetPackageDefinition MAIN_ASSET_PACKAGE_DEFINITION;
}
