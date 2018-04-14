#pragma once

#include <initializer_list>
#include <vector>
#include "Resources/AssetDefinition.h"

namespace RESOURCES
{
    /// A collection of assets in a single package.
    ///
    /// This class is primarily defined to assist in grouping
    /// assets so that different ones can be loaded at different
    /// times to reduce overall loading time for users.
    ///
    /// This class doesn't currently corresponding to any kind
    /// of asset package file, but that may be a future option.
    class AssetPackage
    {
    public:
        // CONSTRUCTION.
        explicit AssetPackage(const std::initializer_list<AssetDefinition>& assets);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The assets in the package.
        std::vector<AssetDefinition> Assets;
    };
}
