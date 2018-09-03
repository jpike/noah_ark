#pragma once

#include <initializer_list>
#include <vector>
#include "Resources/AssetDefinition.h"

namespace RESOURCES
{
    /// Defines a collection of assets in a single package.
    ///
    /// This class simply a definition or description of assets
    /// within an asset package.  It only represents metadata,
    /// not any actual data or a file, related to an asset package.
    class AssetPackageDefinition
    {
    public:
        // CONSTRUCTION.
        explicit AssetPackageDefinition(const std::initializer_list<AssetDefinition>& assets);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The assets in the package.
        std::vector<AssetDefinition> Assets;
    };
}
