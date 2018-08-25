#pragma once

#include <filesystem>
#include "Resources/AssetId.h"
#include "Resources/AssetType.h"

namespace RESOURCES
{
    /// Defines identifying information about an asset for the game.
    class AssetDefinition
    {
    public:
        // CONSTRUCTION.
        explicit AssetDefinition(const AssetType type, const AssetId id, const std::filesystem::path& filepath);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The type of the asset.
        AssetType Type;
        /// The unique ID of the asset.
        AssetId Id;
        /// The path (relative or absolute) to the asset file.
        std::filesystem::path Filepath;
    };
}
