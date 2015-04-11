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

MATH::Vector2f TileMap::GetBottomRightWorldPosition() const
{
    // CALCULATE THE BOTTOM RIGHT POSITION FROM OTHER TILE MAP PROPERTIES.
    // Calculate the bottom position.
    float mapHeightInTiles = static_cast<float>(GetHeightInTiles());
    float tileHeightInPixels = static_cast<float>(GetTileHeightInPixels());
    float mapHeightInPixels = mapHeightInTiles * tileHeightInPixels;
    float bottomWorldPosition = m_topLeftWorldPositionInPixels.Y + mapHeightInPixels;

    // Calculate the right position.
    float mapWidthInTiles = static_cast<float>(GetWidthInTiles());
    float tileWidthInPixels = static_cast<float>(GetTileWidthInPixels());
    float mapWidthInPixels = mapWidthInTiles * tileWidthInPixels;
    float rightWorldPosition = m_topLeftWorldPositionInPixels.X + mapWidthInPixels;

    // Return the bottom right position.
    MATH::Vector2f bottomRightWorldPosition(rightWorldPosition, bottomWorldPosition);
    return bottomRightWorldPosition;
}

MATH::Vector2f TileMap::GetCenterWorldPosition() const
{
    // GET THE BOUNDING CORNERS OF THE TILE MAP.
    MATH::Vector2f topLeftWorldPostion = GetTopLeftWorldPosition();
    MATH::Vector2f bottomRightWorldPosition = GetBottomRightWorldPosition();

    // CALCULATE THE CENTER USING THE MIDPOINT FORMULA.
    float centerXPosition = (topLeftWorldPostion.X + bottomRightWorldPosition.X) / 2.0f;
    float centerYPosition = (topLeftWorldPostion.Y + bottomRightWorldPosition.Y) / 2.0f;

    MATH::Vector2f centerPosition(centerXPosition, centerYPosition);
    return centerPosition;
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

MATH::Vector2f TileMap::GetSizeInPixels() const
{
    // GET THE WIDTH IN PIXELS.
    float mapWidthInTiles = static_cast<float>(GetWidthInTiles());
    float tileWidthInPixels = static_cast<float>(GetTileWidthInPixels());
    float mapWidthInPixels = mapWidthInTiles * tileWidthInPixels;

    // GET THE HEIGHT IN PIXELS.
    float mapHeightInTiles = static_cast<float>(GetHeightInTiles());
    float tileHeightInPixels = static_cast<float>(GetTileHeightInPixels());
    float mapHeightInPixels = mapHeightInTiles * tileHeightInPixels;

    // RETURN THE MAP SIZE IN PIXELS.
    MATH::Vector2f sizeInPixels(mapWidthInPixels, mapHeightInPixels);
    return sizeInPixels;
}

std::shared_ptr<Tile> TileMap::GetTileAtWorldPosition(const float worldXPosition, const float worldYPosition) const
{
    // CHECK IF THE WORLD POSITION IS WITHIN THE BOUNDS OF THIS TILE MAP.
    MATH::Vector2f centerWorldPosition = GetCenterWorldPosition();
    MATH::Vector2f sizeInPixels = GetSizeInPixels();
    MATH::FloatRectangle tileMapBoundingRect(
        centerWorldPosition.X,
        centerWorldPosition.Y,
        sizeInPixels.X,
        sizeInPixels.Y);
    bool tilePositionWithinMap = tileMapBoundingRect.Contains(worldXPosition, worldYPosition);
    if (!tilePositionWithinMap)
    {
        // The requested tile position isn't within this map.
        return nullptr;
    }

    // ADJUST THE WORLD POSITIONS TO BE RELATIVE TO THIS TILE MAP.
    // The top-left corner of this map will serve as the new origin.
    MATH::Vector2f topLeftMapWorldPosition = GetTopLeftWorldPosition();
    float relativeTileXPosition = worldXPosition - topLeftMapWorldPosition.X;
    float relativeTileYPosition = worldYPosition - topLeftMapWorldPosition.Y;

    // CALCULATE THE GRID COORDINATES OF THE REQUESTED TILE.
    unsigned int tileXGridPosition = static_cast<int>(relativeTileXPosition / GetTileWidthInPixels());
    unsigned int tileYGridPosition = static_cast<int>(relativeTileYPosition / GetTileHeightInPixels());

    // VALIDATE THE GRID COORDINATES OF THE TILE.
    // Validate the X coordinate.
    bool validTileXGridPosition = (tileXGridPosition < m_tiles.size());
    if (!validTileXGridPosition)
    {
        // The tile grid position isn't valid, so no tile can be retrieved.
        return nullptr;
    }

    // Validate the Y coordinate.
    bool validTileYGridPosition = (tileYGridPosition < m_tiles[tileXGridPosition].size());
    if (!validTileYGridPosition)
    {
        // The tile grid position isn't valid, so no tile can be retrieved.
        return nullptr;
    }
    
    // GET THE TILE AT THE SPECIFIED GRID COORDINATES.
    return m_tiles[tileXGridPosition][tileYGridPosition];
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
                static_cast<float>(tileHeightInPixels),
                GRAPHICS::GraphicsLayer::GROUND);

            // SET ADDITIONAL PROPERTIES OF THE TILE'S SPRITE.
            // Set the sprite's world position.
            float worldXPosition = static_cast<float>(tileWidthInPixels * xPositionInTiles) + topLeftWorldPositionInPixels.X;
            // Position it based on its center.
            /// @todo   Re-examine commented-out code.
            /*float tileHalfWidth = static_cast<float>(tileWidthInPixels) / 2.0f;
            worldXPosition += tileHalfWidth;*/
            float worldYPosition = static_cast<float>(tileHeightInPixels * yPositionInTiles) + topLeftWorldPositionInPixels.Y;
            // Position it based on its center.
            /*float tileHalfHeight = static_cast<float>(tileHeightInPixels) / 2.0f;
            worldYPosition += tileHalfHeight;*/
            tileSprite->SetWorldPosition(worldXPosition, worldYPosition);

            // STORE A TILE IN THE COLLECTION OF TILES.
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tmxTile.id, tileSprite);
            m_tiles[xPositionInTiles][yPositionInTiles] = tile;
        }
    }
}
