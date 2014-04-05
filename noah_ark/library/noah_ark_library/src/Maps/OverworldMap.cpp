#include <memory>
#include <TmxParser/Tmx.h>
#include "Maps/OverworldMap.h"

using namespace MAPS;

OverworldMap::OverworldMap(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
    m_tileMaps()
{
    // Create an initial tile map for testing.
    /// @todo   Change the code around so that a hard coded path isn't used here.
    ///         The OverworldState should take in a filepath and possibly a resource
    ///         manager to load an XML file that specifies the entire overworld map.
    ///         That class should load the overworld map to pass to this constructor.
    ///         Alternatively, individual tile maps could reference adjacent tile maps.
    std::shared_ptr<Tmx::Map> initialTmxMap(new Tmx::Map());
    initialTmxMap->ParseFile("res/maps/test_map_1_1.tmx");
    std::shared_ptr<TileMap> initialTileMap( new TileMap(initialTmxMap, graphicsSystem) );
    m_tileMaps.push_back( std::vector< std::shared_ptr<TileMap> >() );
    m_tileMaps[0].push_back(initialTileMap);
    
    // Nothing else to do.
}

OverworldMap::~OverworldMap()
{
    // Nothing else to do.
}
