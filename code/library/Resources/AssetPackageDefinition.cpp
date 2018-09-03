#include "Resources/AssetPackageDefinition.h"

namespace RESOURCES
{
    /// Constructor.
    /// @param[in]  assets - See \ref Assets.
    AssetPackageDefinition::AssetPackageDefinition(const std::initializer_list<AssetDefinition>& assets) :
        Assets(assets)
    {}
}
