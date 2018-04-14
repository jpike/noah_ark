#include "Resources/AssetPackage.h"

namespace RESOURCES
{
    /// Constructor.
    /// @param[in]  assets - See \ref Assets.
    AssetPackage::AssetPackage(const std::initializer_list<AssetDefinition>& assets) :
        Assets(assets)
    {}
}
