#include "Maps/TileMap.h"

using namespace MAPS;

TileMap::TileMap(
    const OverworldGridPosition& overworldGridPosition,
    const MATH::Vector2f& topLeftWorldPositionInPixels,
    const std::shared_ptr<Tmx::Map>& map, 
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
    m_tiles(),
    m_tmxMap(map),
    m_overworldGridPosition(overworldGridPosition),
    m_topLeftWorldPositionInPixels(topLeftWorldPositionInPixels)
{
    // BUILD THIS TILEMAP FROM THE TMX MAP.
    BuildFromTmxMap(topLeftWorldPositionInPixels, map, *graphicsSystem);
}

TileMap::~TileMap()
{
    // Nothing else to do.
}

OverworldGridPosition TileMap::GetOverworldGridPosition() const
{
    return m_overworldGridPosition;
}

MATH::Vector2f TileMap::GetTopLeftWorldPosition() const
{
    return m_topLeftWorldPositionInPixels;
}

unsigned int TileMap::GetWidthInTiles() const
{
    return static_cast<unsigned int>(m_tmxMap->GetWidth());
}

unsigned int TileMap::GetHeightInTiles() const
{
    return static_cast<unsigned int>(m_tmxMap->GetHeight());
}

unsigned int TileMap::GetTileWidthInPixels() const
{
    return static_cast<unsigned int>(m_tmxMap->GetTileWidth());
}

unsigned int TileMap::GetTileHeightInPixels() const
{
    return static_cast<unsigned int>(m_tmxMap->GetTileHeight());
}

void TileMap::BuildFromTmxMap(
    const MATH::Vector2f& topLeftWorldPositionInPixels,
    const std::shared_ptr<Tmx::Map>& map, 
    GRAPHICS::GraphicsSystem& graphicsSystem)
{
    // VERIFY THAT THE EXPECTED NUMBER OF LAYERS EXIST IN THE MAP.
    // For now, we only support a single ground layer.
    const int EXPECTED_LAYER_COUNT = 1;
    int actualLayerCount = map->GetNumLayers();
    bool expectedLayersProvided = (EXPECTED_LAYER_COUNT == actualLayerCount);
    if (!expectedLayersProvided)
    {
        // Don't populate the map if it isn't properly formed.
        return;
    }

    // VERIFY THAT THE EXPECTED NUMBER OF TILESETS EXIST IN THE MAP.
    // For now, we only support a single tileset.
    const int EXPECTED_TILESET_COUNT = 1;
    int actualTilesetCount = map->GetNumTilesets();
    bool expectedTilesetsProvided = (EXPECTED_TILESET_COUNT == actualTilesetCount);
    if (!expectedLayersProvided)
    {
        // Don't populate the map if it isn't properly formed.
        return;
    }
    
    // GET THE LAYER TO USE FOR BUILDING THE MAP.
    const int ONLY_LAYER = 0;
    const Tmx::Layer* pMapLayer = map->GetLayer(ONLY_LAYER);

    // GET THE TILESET TEXTURE NEEDED FOR CREATING GRAPHICS FOR THE TILES.
    const int ONLY_TILESET = 0;
    const Tmx::Tileset* pTileset = map->GetTileset(ONLY_TILESET);
    const std::string& tilesetName = pTileset->GetName();
    std::shared_ptr<GRAPHICS::Texture> tilesetTexture = graphicsSystem.GetTexture(tilesetName.c_str());
    bool tilesetTextureExists = (nullptr != tilesetTexture);
    if (!tilesetTextureExists)
    {
        return;
    }

    // GET THE TILESET IMAGE INFORMATION SO THAT WE KNOW HOW TO CONVERT A TILE ID TO PIXEL COORDINATES OF A TILE.
    const Tmx::Image* pTilesetImage = pTileset->GetImage();
    int tilesetImageWidthInPixels = pTilesetImage->GetWidth();
    int tilesetImageHeightInPixels = pTilesetImage->GetHeight();

    // POPULATE THE MAP FROM THE ONLY LAYER.
    int layerWidthInTiles = pMapLayer->GetWidth();
    int layerHeightInTiles = pMapLayer->GetHeight();
    // Reserve space in the container for all columns of tiles to avoid later reallocations.
    m_tiles.resize(layerWidthInTiles);
    for (int xPositionInTiles = 0; xPositionInTiles < layerWidthInTiles; ++xPositionInTiles)
    {
        // Reserve space for all rows of tiles in the current row to avoid later reallocations.
        m_tiles.at(xPositionInTiles).resize(layerHeightInTiles);
        for (int yPositionInTiles = 0; yPositionInTiles < layerHeightInTiles; ++yPositionInTiles)
        {
            // GET INFORMATION NEEDED TO CREATE THE TILE.
            const Tmx::MapTile& tmxTile = pMapLayer->GetTile(xPositionInTiles, yPositionInTiles);

            int tileWidthInPixels = pTileset->GetTileWidth();
            int tileHeightInPixels = pTileset->GetTileHeight();

            /// @todo   Move this logic to a helper function.
            float tilesetXPosition = static_cast<float>( (tileWidthInPixels * tmxTile.id) % tilesetImageWidthInPixels) ;
            unsigned int tilesPerRow = (tilesetImageWidthInPixels / tileWidthInPixels);
            float tilesetYPosition = static_cast<float>( (tileHeightInPixels * (tmxTile.id / tilesPerRow) ) % tilesetImageHeightInPixels );
            
            // CREATE A SPRITE FOR THE TILE.
            std::shared_ptr<GRAPHICS::Sprite> tileSprite = graphicsSystem.CreateSprite(
                tilesetTexture,
                tilesetXPosition,
                tilesetYPosition,
                static_cast<float>(tileWidthInPixels),
                static_cast<float>(tileHeightInPixels));

            // SET ADDITIONAL PROPERTIES OF THE TILE'S SPRITE.
            // Set any flipping for the sprite.
            tileSprite->SetFlip(tmxTile.flippedHorizontally, tmxTile.flippedVertically);

            // Set the sprite's world position.
            float worldXPosition = static_cast<float>(tileWidthInPixels * xPositionInTiles) + topLeftWorldPositionInPixels.X;
            float worldYPosition = static_cast<float>(tileHeightInPixels * yPositionInTiles) + topLeftWorldPositionInPixels.Y;
            tileSprite->SetWorldPosition(worldXPosition, worldYPosition);

            // Set the tile to the proper z-layer.
            tileSprite->SetZPosition(GRAPHICS::GraphicsSystem::GROUND_LAYER_Z_VALUE);

            // STORE A TILE IN THE COLLECTION OF TILES.
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileSprite);
            m_tiles.at(xPositionInTiles).push_back(tile);
        }
    }
}
