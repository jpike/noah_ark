#include <stdexcept>
#include "Maps/TileMap.h"
#include "Maps/TileMapObjectLayer.h"
#include "Maps/TileMapTileLayer.h"

namespace MAPS
{
    TileMap::TileMap(
        const OverworldGridPosition& overworld_grid_position,
        const MATH::Vector2f& top_left_world_position_in_pixels,
        const std::shared_ptr<ITileMapData>& map_data,
        std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system) :
    Visible(false),
    OverworldPosition(overworld_grid_position),
    TopLeftWorldPositionInPixels(top_left_world_position_in_pixels),
    WidthInTiles(0),
    HeightInTiles(0),
    TileWidthInPixels(0),
    TileHeightInPixels(0),
    Tileset(),
    Layers()
    {
        // MAKE MAP DATA WAS PROVIDED.
        bool map_data_exists = (nullptr != map_data);
        if (!map_data_exists)
        {
            throw std::runtime_error("Cannot construct tile map with null map data.");
        }

        // MAKE SURE A GRAPHICS SYSTEM WAS PROVIDED.
        bool graphics_system_exists = (nullptr != graphics_system);
        if (!graphics_system_exists)
        {
            throw std::runtime_error("Cannot construct tile map with null graphics system.");
        }

        // BUILD THIS TILEMAP FROM THE MAP DATA.
        BuildFromMapData(TopLeftWorldPositionInPixels, *map_data, *graphics_system);
    }

    TileMap::~TileMap()
    {
        // Nothing else to do.
    }

    OverworldGridPosition TileMap::GetOverworldGridPosition() const
    {
        return OverworldPosition;
    }

    MATH::Vector2f TileMap::GetTopLeftWorldPosition() const
    {
        return TopLeftWorldPositionInPixels;
    }

    MATH::Vector2f TileMap::GetBottomRightWorldPosition() const
    {
        // CALCULATE THE BOTTOM RIGHT POSITION FROM OTHER TILE MAP PROPERTIES.
        // Calculate the bottom position.
        float map_height_in_tiles = static_cast<float>(GetHeightInTiles());
        float tile_height_in_pixels = static_cast<float>(GetTileHeightInPixels());
        float map_height_in_pixels = map_height_in_tiles * tile_height_in_pixels;
        float bottom_world_position = TopLeftWorldPositionInPixels.Y + map_height_in_pixels;

        // Calculate the right position.
        float map_width_in_tiles = static_cast<float>(GetWidthInTiles());
        float tile_width_in_pixels = static_cast<float>(GetTileWidthInPixels());
        float map_width_in_pixels = map_width_in_tiles * tile_width_in_pixels;
        float right_world_position = TopLeftWorldPositionInPixels.X + map_width_in_pixels;

        // Return the bottom right position.
        MATH::Vector2f bottom_right_world_position(right_world_position, bottom_world_position);
        return bottom_right_world_position;
    }

    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        // GET THE BOUNDING CORNERS OF THE TILE MAP.
        MATH::Vector2f top_left_world_position = GetTopLeftWorldPosition();
        MATH::Vector2f bottom_right_world_position = GetBottomRightWorldPosition();

        // CALCULATE THE CENTER USING THE MIDPOINT FORMULA.
        float center_x_position = (top_left_world_position.X + bottom_right_world_position.X) / 2.0f;
        float center_y_position = (top_left_world_position.Y + bottom_right_world_position.Y) / 2.0f;

        MATH::Vector2f center_position(center_x_position, center_y_position);
        return center_position;
    }

    unsigned int TileMap::GetWidthInTiles() const
    {
        return WidthInTiles;
    }

    unsigned int TileMap::GetHeightInTiles() const
    {
        return HeightInTiles;
    }

    unsigned int TileMap::GetTileWidthInPixels() const
    {
        return TileHeightInPixels;
    }

    unsigned int TileMap::GetTileHeightInPixels() const
    {
        return TileHeightInPixels;
    }

    MATH::Vector2f TileMap::GetSizeInPixels() const
    {
        // GET THE WIDTH IN PIXELS.
        float map_width_in_tiles = static_cast<float>(GetWidthInTiles());
        float tile_width_in_pixels = static_cast<float>(GetTileWidthInPixels());
        float map_width_in_pixels = map_width_in_tiles * tile_width_in_pixels;

        // GET THE HEIGHT IN PIXELS.
        float map_height_in_tiles = static_cast<float>(GetHeightInTiles());
        float tile_height_in_pixels = static_cast<float>(GetTileHeightInPixels());
        float map_height_in_pixels = map_height_in_tiles * tile_height_in_pixels;

        // RETURN THE MAP SIZE IN PIXELS.
        MATH::Vector2f size_in_pixels(map_width_in_pixels, map_height_in_pixels);
        return size_in_pixels;
    }

    std::shared_ptr<Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        // CHECK IF THE WORLD POSITION IS WITHIN THE BOUNDS OF THIS TILE MAP.
        MATH::Vector2f center_world_position = GetCenterWorldPosition();
        MATH::Vector2f size_in_pixels = GetSizeInPixels();
        MATH::FloatRectangle tile_map_bounding_rect(
            center_world_position.X,
            center_world_position.Y,
            size_in_pixels.X,
            size_in_pixels.Y);
        bool tile_position_within_map = tile_map_bounding_rect.Contains(world_x_position, world_y_position);
        if (!tile_position_within_map)
        {
            // The requested tile position isn't within this map.
            return nullptr;
        }

        // ADJUST THE WORLD POSITIONS TO BE RELATIVE TO THIS TILE MAP.
        // The top-left corner of this map will serve as the new origin.
        MATH::Vector2f top_left_map_world_position = GetTopLeftWorldPosition();
        float relative_tile_x_position = world_x_position - top_left_map_world_position.X;
        float relative_tile_y_position = world_y_position - top_left_map_world_position.Y;

        // CALCULATE THE GRID COORDINATES OF THE REQUESTED TILE.
        unsigned int tile_x_grid_position = static_cast<unsigned int>(relative_tile_x_position / GetTileWidthInPixels());
        unsigned int tile_y_grid_position = static_cast<unsigned int>(relative_tile_y_position / GetTileHeightInPixels());

        // VALIDATE THE GRID COORDINATES OF THE TILE.
        // Validate the X coordinate.
        bool valid_tile_x_grid_position = (tile_x_grid_position < WidthInTiles);
        if (!valid_tile_x_grid_position)
        {
            // The tile grid position isn't valid, so no tile can be retrieved.
            return nullptr;
        }

        // Validate the Y coordinate.
        bool valid_tile_y_grid_position = (tile_y_grid_position < HeightInTiles);
        if (!valid_tile_y_grid_position)
        {
            // The tile grid position isn't valid, so no tile can be retrieved.
            return nullptr;
        }

        // GET THE TILE AT THE SPECIFIED GRID COORDINATES.
        /// @todo   Figure out which layer to get get this tile from.
        std::shared_ptr<Tile> tile = Layers.GetTileAtGridPosition(tile_x_grid_position, tile_y_grid_position);
        return tile;
    }

    void TileMap::SetVisible(const bool visible)
    {
        Visible = visible;
    }

    bool TileMap::IsVisible() const
    {
        return Visible;
    }
    
    void TileMap::Render(sf::RenderTarget& render_target)
    {
        // DON'T RENDER IF WE AREN'T VISIBLE.
        if (!Visible)
        {
            return;
        }

        // RENDER ALL LAYERS.
        Layers.Render(render_target);
    }
    
    void TileMap::Update(const float elapsed_time_in_seconds)
    {
        // Tile maps currently don't need any updating.
        // If we need something like animated tiles in
        // the future, then we can add logic here.
    }

    void TileMap::BuildFromMapData(
        const MATH::Vector2f& top_left_world_position_in_pixels,
        const ITileMapData& map_data,
        GRAPHICS::GraphicsSystem& graphics_system)
    {
        // GET THE MAP DIMENSIONS.
        WidthInTiles = map_data.GetWidthInTiles();
        HeightInTiles = map_data.GetHeightInTiles();
        TileWidthInPixels = map_data.GetTileWidthInPixels();
        TileHeightInPixels = map_data.GetTileHeightInPixels();

        // CREATE THE TILESET.
        std::vector<TilesetDescription> tileset_descriptions = map_data.GetTilesets();
        Tileset.Populate(tileset_descriptions, graphics_system);

        // CREATE EACH LAYER.
        Layers.Clear();
        std::vector<TileMapLayerDescription> layer_descriptions = map_data.GetLayers();
        for (const auto& layer_description : layer_descriptions)
        {
            // CREATE A LAYER OF THE APPROPRIATE TYPE.
            std::unique_ptr<ITileMapLayer> layer;
            bool is_tile_layer = (TileMapLayerType::TILE_LAYER == layer_description.Type);
            bool is_object_layer = (TileMapLayerType::OBJECT_LAYER == layer_description.Type);
            if (is_tile_layer)
            {
                layer = std::unique_ptr<ITileMapLayer>(new TileMapTileLayer(
                    layer_description.Name,
                    top_left_world_position_in_pixels,
                    layer_description.TileIds, 
                    Tileset));
            }
            else if (is_object_layer)
            {
                layer = std::unique_ptr<ITileMapLayer>(new TileMapObjectLayer(
                    layer_description.Name, 
                    layer_description.Objects, 
                    graphics_system));
            }
            else
            {
                // The layer is invalid, so skip over it to avoid adding a
                // null pointer to the container of layers.
            }

            // ADD THE LAYER TO THIS TILE MAP.
            Layers.AddToTop(layer);
        }
    }
}