#include "Maps/TileMapLoader.h"

namespace MAPS
{
    TileMapLoader::TileMapLoader()
    {}

    TileMapLoader::~TileMapLoader()
    {}

    std::shared_ptr<TiledMapJsonFile> TileMapLoader::LoadMap(const std::string& filepath) const
    {
        std::shared_ptr<TiledMapJsonFile> map_file = std::make_shared<TiledMapJsonFile>();
        /// @todo   Error handling?
        map_file->Load(filepath);
        return map_file;
    }
}