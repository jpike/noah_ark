#pragma once

#include "Maps/TileMapLoader.h"

using namespace MAPS;

TileMapLoader::TileMapLoader()
{}
        
TileMapLoader::~TileMapLoader()
{}

std::shared_ptr<Tmx::Map> TileMapLoader::LoadMap(const std::string& filepath) const
{
    std::shared_ptr<Tmx::Map> tmxMap(new Tmx::Map());
    tmxMap->ParseFile(filepath);
    return tmxMap;
}