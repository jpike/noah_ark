#pragma once

#include <filesystem>
#include <unordered_map>
#include "Resources/Asset.h"
#include "Resources/AssetId.h"
#include "Resources/AssetPackageDefinition.h"

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
        // FILE ACCESS.
        static std::unordered_map<AssetId, Asset> ReadFile(const std::filesystem::path& filepath);
        static bool WriteFile(const AssetPackageDefinition& asset_package_definition, const std::filesystem::path& filepath);
    };
}
