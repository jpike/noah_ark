#include <cassert>
#include "Maps/Ark.h"
#include "Maps/Tileset.h"
#include "Maps/Data/ArkInteriorTileMapData.h"

namespace MAPS
{
    /// constructor.  Populates the ark interior based on data read from in-memory assets.
    /// @param[in]  world - The world this ark exists in.
    Ark::Ark(MEMORY::NonNullRawPointer<World> world) :
        Interior()
    {
        // CLEAR ANY PREVIOUS INFORMATION IN THE INTERIOR.
        Interior.LayersFromBottomToTop.clear();

        // LOAD TILE MAPS FOR EACH LAYER INTO THE ARK INTERIOR.
        Tileset tileset;
        std::size_t layer_count = DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP.size();
        // Space for the layers must be reserved to avoid reallocations that would invalidate pointers.
        /// @todo   Maybe this means we should go with shared pointers?
        Interior.LayersFromBottomToTop.reserve(layer_count);
        for (std::size_t layer_index = 0; layer_index < layer_count; ++layer_index)
        {
            // CREATE AN EMPTY MAP GRID FOR THE CURRENT LAYER.
            auto& current_layer_data = DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP[layer_index];
            Interior.LayersFromBottomToTop.emplace_back(Ark::INTERIOR_WIDTH_IN_TILE_MAPS, Ark::INTERIOR_HEIGHT_IN_TILE_MAPS, world);
            auto& current_map_layer = Interior.LayersFromBottomToTop.back();

            // LOAD TILE MAPS FOR EACH ROW.
            for (unsigned int row = 0; row < Ark::INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // LOAD TILE MAPS FOR EACH COLUMN.
                for (unsigned int column = 0; column < Ark::INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
                {
                    // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
                    MATH::Vector2f map_center_world_position;

                    float map_width_in_pixels = static_cast<float>(TileMap::WIDTH_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                    float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
                    float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
                    map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

                    float map_height_in_pixels = static_cast<float>(TileMap::HEIGHT_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                    float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
                    float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
                    map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

                    // CREATE AN EMPTY TILE MAP.
                    MATH::Vector2ui map_dimensions_in_tiles(
                        MAPS::TileMap::WIDTH_IN_TILES,
                        MAPS::TileMap::HEIGHT_IN_TILES);
                    auto tile_map = std::make_shared<TileMap>(
                        TileMapType::ARK_INTERIOR,
                        MEMORY::NonNullRawPointer<MultiTileMapGrid>(&current_map_layer),
                        row,
                        column,
                        map_center_world_position,
                        map_dimensions_in_tiles);

                    // GET THE CURRENT TILE MAP.
                    const auto& tile_map_data = (*current_layer_data)(column, row);

                    // CREATE TILES IN THE GROUND LAYER.
                    for (unsigned int current_tile_y = 0;
                        current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                        ++current_tile_y)
                    {
                        // CREATE TILES FOR THIS ROW.
                        for (unsigned int current_tile_x = 0;
                            current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                            ++current_tile_x)
                        {
                            // CREATE THE CURRENT TILE.
                            MAPS::TileId tile_id = (*tile_map_data)(current_tile_x, current_tile_y);
                            std::shared_ptr<MAPS::Tile> tile = tileset.CreateTile(tile_id);
                            bool tile_exists_in_tileset = (nullptr != tile);
                            if (!tile_exists_in_tileset)
                            {
                                // Skip to trying to create the next tile.  The layer
                                // simply won't have any tile at this location.
                                continue;
                            }

                            // SET THE TILE IN THE GROUND LAYER.
                            tile_map->Ground.SetTile(current_tile_x, current_tile_y, tile);

                            // CHECK IF THE TILE IS FOR AN ANIMAL PEN ENTRANCE.
                            bool is_animal_pen_entrance = (TileType::ANIMAL_PEN_ENTRANCE == tile->Type);
                            if (is_animal_pen_entrance)
                            {
                                // ADD AN ANIMAL PEN TO THE MAP.
                                // For simplicity, all animal pens have the same structure, with the
                                // entrance (marked by E below) and then an set of walls (W).
                                // That means the interior bounds can be computed given the entrance.
                                // W W W W W
                                // W       W
                                // W       W
                                // W       W
                                // W W E W W
                                AnimalPen animal_pen;
                                constexpr float ANIMAL_PEN_INTERIOR_DIMENSION_IN_TILES = 3;
                                constexpr float ANIMAL_PEN_INTERIOR_DIMENSION_IN_PIXELS = ANIMAL_PEN_INTERIOR_DIMENSION_IN_TILES * Tile::DIMENSION_IN_PIXELS<float>;
                                constexpr float ANIMAL_PEN_HALF_INTERIOR_DIMENSION_IN_TILES = ANIMAL_PEN_INTERIOR_DIMENSION_IN_TILES / 2.0f;
                                constexpr float ANIMAL_PEN_HALF_INTERIOR_DIMENSION_IN_PIXELS = ANIMAL_PEN_HALF_INTERIOR_DIMENSION_IN_TILES * Tile::DIMENSION_IN_PIXELS<float>;
                                MATH::Vector2f entrance_center_world_position = tile->Sprite.GetWorldPosition();
                                float animal_pen_interior_center_y_position = entrance_center_world_position.Y - ANIMAL_PEN_HALF_INTERIOR_DIMENSION_IN_PIXELS;
                                animal_pen.InteriorBoundingBox = MATH::FloatRectangle::FromCenterAndDimensions(
                                    entrance_center_world_position.X,
                                    animal_pen_interior_center_y_position,
                                    ANIMAL_PEN_INTERIOR_DIMENSION_IN_PIXELS,
                                    ANIMAL_PEN_INTERIOR_DIMENSION_IN_PIXELS);
                                tile_map->AnimalPens.emplace_back(animal_pen);
                            }
                        }
                    }

                    // STORE THE TILE MAP OF THE ARK INTERIOR.
                    current_map_layer.TileMaps(column, row) = tile_map;
                }
            }
        }

        // CREATE EXIT POINTS FOR MOVING BETWEEN FLOORS OF THE ARK INTERIOR.
        for (std::size_t layer_index = 0; layer_index < layer_count; ++layer_index)
        {
            // GET THE APPLICABLE LAYERS.
            MAPS::MultiTileMapGrid& current_map_layer = Interior.LayersFromBottomToTop.at(layer_index);

            MAPS::MultiTileMapGrid* previous_map_layer = nullptr;
            bool previous_layer_exists = (layer_index > 0);
            if (previous_layer_exists)
            {
                std::size_t previous_map_layer_index = layer_index - 1;
                previous_map_layer = &Interior.LayersFromBottomToTop.at(previous_map_layer_index);
            }

            MAPS::MultiTileMapGrid* next_map_layer = nullptr;
            std::size_t last_layer_index = layer_count - 1;
            bool next_layer_exists = (layer_index < last_layer_index);
            if (next_layer_exists)
            {
                std::size_t next_map_layer_index = layer_index + 1;
                next_map_layer = &Interior.LayersFromBottomToTop.at(next_map_layer_index);
            }

            // CHECK ALL MAPS IN THE CURRENT LAYER'S ROWS FOR STAIRCASES.
            for (unsigned int row = 0; row < Ark::INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // CHECK ALL MAPS IN THE CURRENT LAYER'S COLUMNS FOR STAIRCASES.
                for (unsigned int column = 0; column < Ark::INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
                {
                    // GET THE CURRENT TILE MAP.
                    auto& current_tile_map = current_map_layer.TileMaps(column, row);

                    // CHECK TILES ACROSS ALL ROWS.
                    for (unsigned int current_tile_y = 0;
                        current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                        ++current_tile_y)
                    {
                        // CHECK TILES ACROSS ALL COLUMNS.
                        for (unsigned int current_tile_x = 0;
                            current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                            ++current_tile_x)
                        {
                            // GET THE CURRENT TILE.
                            auto& current_tile = current_tile_map->Ground.Tiles(current_tile_x, current_tile_y);
                            if (!current_tile)
                            {
                                // SKIP THIS TILE SINCE IT DOESN'T EXIST.
                                continue;
                            }

                            // CHECK IF THE TILE IS FOR AN UPWARD STAIRCASE.
                            bool tile_is_upward_stairs = (MAPS::TileType::ARK_INTERIOR_UP_STAIRS == current_tile->Type);
                            if (tile_is_upward_stairs)
                            {
                                // ADD AN EXIT POINT TO THE ABOVE FLOOR IF ONE EXISTS.
                                if (next_map_layer)
                                {
                                    auto& above_tile_map = next_map_layer->TileMaps(column, row);

                                    MAPS::ExitPoint ark_floor_exit_point;
                                    MATH::FloatRectangle tile_bounding_box = current_tile->Sprite.GetWorldBoundingBox();
                                    ark_floor_exit_point.BoundingBox = tile_bounding_box;
                                    ark_floor_exit_point.NewMapGrid = next_map_layer;
                                    ark_floor_exit_point.NewTileMap = &*above_tile_map.get();
                                    ark_floor_exit_point.NewPlayerWorldPosition = tile_bounding_box.Center();

                                    current_tile_map->ExitPoints.push_back(ark_floor_exit_point);
                                }
                            }

                            // CHECK IF THE TILE IS FOR AN DOWNWARD STAIRCASE.
                            bool tile_is_downward_stairs = (MAPS::TileType::ARK_INTERIOR_DOWN_STAIRS == current_tile->Type);
                            if (tile_is_downward_stairs)
                            {
                                // ADD AN EXIT POINT TO THE BELOW FLOOR IF ONE EXISTS.
                                if (previous_map_layer)
                                {
                                    auto& below_tile_map = previous_map_layer->TileMaps(column, row);

                                    MAPS::ExitPoint ark_floor_exit_point;
                                    MATH::FloatRectangle tile_bounding_box = current_tile->Sprite.GetWorldBoundingBox();
                                    ark_floor_exit_point.BoundingBox = tile_bounding_box;
                                    ark_floor_exit_point.NewMapGrid = previous_map_layer;
                                    ark_floor_exit_point.NewTileMap = &*below_tile_map.get();
                                    ark_floor_exit_point.NewPlayerWorldPosition = tile_bounding_box.Center();

                                    current_tile_map->ExitPoints.push_back(ark_floor_exit_point);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /// Adds an animal to an appropriate pen in the ark.
    /// @param[in,out]  animal - The animal to add.  It's position will be updated for the animal pen.
    void Ark::AddAnimalToPen(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal)
    {
        // SEARCH FOR AN APPROPRIATE PEN.
        // If a pen already exists for the animal species, the animal should be placed in it.
        // Otherwise, the animal should go into first empty pen.
        MAPS::AnimalPen* animal_pen_for_species = nullptr;
        MAPS::AnimalPen* first_empty_animal_pen = nullptr;
        for (auto& ark_interior_layer : Interior.LayersFromBottomToTop)
        {
            // CHECK EACH TILE MAP IN THE LAYER.
            unsigned int height_in_tile_maps = ark_interior_layer.TileMaps.GetHeight();
            unsigned int width_in_tile_maps = ark_interior_layer.TileMaps.GetWidth();
            for (unsigned int tile_map_row_index = 0; tile_map_row_index < height_in_tile_maps; ++tile_map_row_index)
            {
                for (unsigned int tile_map_column_index = 0; tile_map_column_index < width_in_tile_maps; ++tile_map_column_index)
                {
                    // MAKE SURE THE CURRENT TILE MAP EXISTS.
                    std::shared_ptr<MAPS::TileMap>& current_tile_map = ark_interior_layer.TileMaps(
                        tile_map_column_index, 
                        tile_map_row_index);
                    if (!current_tile_map)
                    {
                        continue;
                    }

                    // CHECK THE ANIMAL PENS IN THE CURRENT MAP.
                    for (auto& animal_pen : current_tile_map->AnimalPens)
                    {
                        // CHECK IF AN ANIMAL PEN FOR THE SPECIES NEEDS TO BE FOUND.
                        if (!animal_pen_for_species)
                        {
                            // GET THE ANIMAL PEN IF IT'S FOR THE CORRECT SPECIES.
                            bool animal_pen_matches_species = (animal_pen.Species == animal->Type.Species);
                            if (animal_pen_matches_species)
                            {
                                animal_pen_for_species = &animal_pen;
                            }
                        }

                        // CHECK IF AN EMPTY ANIMAL PEN NEEDS TO BE FOUND.
                        if (!first_empty_animal_pen)
                        {
                            // GET THE ANIMAL PEN IF IT'S EMPTY.
                            bool animal_pen_empty = animal_pen.Animals.empty();
                            if (animal_pen_empty)
                            {
                                first_empty_animal_pen = &animal_pen;
                            }
                        }
                    }
                }
            }
        }

        // STORE THE ANIMAL IN THE PEN FOR THE APPROPRIATE SPECIES IF APPLICABLE.
        if (animal_pen_for_species)
        {
            MATH::Vector2f animal_pen_center = animal_pen_for_species->InteriorBoundingBox.Center();
            animal->Sprite.SetWorldPosition(animal_pen_center);
            animal_pen_for_species->Animals.emplace_back(animal);
            return;
        }

        // STORE THE ANIMAL IN THE EMPTY PEN.
        if (first_empty_animal_pen)
        {
            MATH::Vector2f animal_pen_center = first_empty_animal_pen->InteriorBoundingBox.Center();
            animal->Sprite.SetWorldPosition(animal_pen_center);
            first_empty_animal_pen->Animals.emplace_back(animal);
            first_empty_animal_pen->Species = animal->Type.Species;
            return;
        }

        // VERIFY THAT THE ANIMAL WAS PLACED.
        assert(false && "Failed to place animal in pen.");
    }
}
