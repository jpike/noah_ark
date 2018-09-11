#include "Resources/Asset.h"

namespace RESOURCES
{
    /// Attempts to read the asset data from the provided binary data stream.
    /// @param[in]  data_stream - The data stream from which to read the asset's binary data.
    /// @return The asset, if successfully read; none otherwise.
    std::optional<Asset> Asset::Read(std::istream& data_stream)
    {
        // READ IN THE ASSET TYPE.
        AssetType asset_type = AssetType::INVALID;
        data_stream.read(reinterpret_cast<char*>(&asset_type), sizeof(Type));

        // READ IN THE ASSET ID.
        AssetId asset_id = AssetId::INTRO_MUSIC;
        data_stream.read(reinterpret_cast<char*>(&asset_id), sizeof(Id));

        // READ IN THE ASSET DATA.
        std::string::size_type asset_size_in_bytes = 0;
        data_stream.read(reinterpret_cast<char*>(&asset_size_in_bytes), sizeof(asset_size_in_bytes));

        constexpr char EMPTY_DATA = '\0';
        std::string asset_data(asset_size_in_bytes, EMPTY_DATA);
        data_stream.read(asset_data.data(), asset_size_in_bytes);

        // RETURN THE ASSET IF SUCCESSFULLY READ.
        bool asset_read_successfully = !data_stream.bad() && !data_stream.fail();
        if (asset_read_successfully)
        {
            Asset asset;
            asset.Type = asset_type;
            asset.Id = asset_id;
            asset.BinaryData = asset_data;
            return asset;
        }
        else
        {
            return std::nullopt;
        }
    }
}
