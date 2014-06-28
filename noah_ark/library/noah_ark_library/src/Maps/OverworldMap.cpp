#include <memory>
#include <TmxParser/Tmx.h>
#include "Maps/OverworldMap.h"

using namespace MAPS;

OverworldMap::OverworldMap() :
    m_currentTileMap(),
    m_topTileMap(),
    m_bottomTileMap(),
    m_leftTileMap(),
    m_rightTileMap()
{
    // Create an initial tile map for testing.
    /// @todo   Change the code around so that a hard coded path isn't used here.
    ///         The OverworldState should take in a filepath and possibly a resource
    ///         manager to load an XML file that specifies the entire overworld map.
    ///         That class should load the overworld map to pass to this constructor.
    ///         Alternatively, individual tile maps could reference adjacent tile maps.
    /* std::shared_ptr<Tmx::Map> initialTmxMap(new Tmx::Map());
    initialTmxMap->ParseFile("res/maps/test_map_1_1.tmx");
    std::shared_ptr<TileMap> initialTileMap( new TileMap(initialTmxMap, graphicsSystem) );
    m_tileMaps.push_back( std::vector< std::shared_ptr<TileMap> >() );
    m_tileMaps[0].push_back(initialTileMap); */
    
    // Nothing else to do.
}

OverworldMap::~OverworldMap()
{
    // Nothing else to do.
}

std::shared_ptr<TileMap> OverworldMap::GetCurrentTileMap() const
{
    return m_currentTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetTopTileMap() const
{
    return m_topTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetBottomTileMap() const
{
    return m_bottomTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetLeftTileMap() const
{
    return m_leftTileMap;
}

std::shared_ptr<TileMap> OverworldMap::GetRightTileMap() const
{
    return m_rightTileMap;
}

void OverworldMap::SetCurrentTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_currentTileMap = tileMap;
}

void OverworldMap::SetTopTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_topTileMap = tileMap;
}

void OverworldMap::SetBottomTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_bottomTileMap = tileMap;
}

void OverworldMap::SetLeftTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_leftTileMap = tileMap;
}

void OverworldMap::SetRightTileMap(const std::shared_ptr<TileMap>& tileMap)
{
    m_rightTileMap = tileMap;
}