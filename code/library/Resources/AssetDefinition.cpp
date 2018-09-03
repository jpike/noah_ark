#include "Resources/AssetDefinition.h"

namespace RESOURCES
{
    /// Constructor.
    /// @param[in]  type - See \ref Type.
    /// @param[in]  id - See \ref Id.
    /// @param[in]  filepath - See \ref Filepath.
    AssetDefinition::AssetDefinition(const AssetType type, const AssetId id, const std::filesystem::path& filepath) :
        Type(type),
        Id(id),
        Filepath(filepath)
    {}
}
