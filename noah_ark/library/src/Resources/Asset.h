#pragma once

#include <istream>
#include <optional>
#include <string>
#include "Resources/AssetId.h"
#include "Resources/AssetType.h"

namespace RESOURCES
{
    /// A generic representation of an asset.
    class Asset
    {
    public:
        // READING/WRITING.
        static std::optional<Asset> Read(std::istream& data_stream);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The type of the asset.
        AssetType Type = AssetType::INVALID;
        /// The unique ID of the asset.
        AssetId Id = AssetId::INVALID;
        /// The binary data of the asset.
        std::string BinaryData = "";
    };
}
