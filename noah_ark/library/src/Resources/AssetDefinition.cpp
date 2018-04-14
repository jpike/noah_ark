#include "Resources/AssetDefinition.h"

namespace RESOURCES
{
    /// Constructor.
    /// @param[in]  type - See \ref Type.
    /// @param[in]  id - See \ref Id.
    AssetDefinition::AssetDefinition(const AssetType type, const AssetId id) :
        Type(type),
        Id(id)
    {}
}
