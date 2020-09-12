#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Maps/World.h"
#include "Math/Number.h"
#include "Resources/FoodGraphics.h"

namespace MAPS
{
    /// Creates the world in its initial state.
    /// @return The world in its initial state.
    ///     The world is dynamically allocated to ensure that pointers between
    ///     parts of the world remain consistent, which would be harder to do
    ///     if a regular constructor were used due to the potential for copies.
    MEMORY::NonNullSharedPointer<World> World::CreateInitial()
    {
        // POPULATE THE WORLD.
        MEMORY::NonNullSharedPointer<World> world = MEMORY::NonNullSharedPointer<World>(std::make_shared<World>());

        // RETURN THE POPULATED WORLD.
        return world;
    }

    /// Default constructor.
    World::World() :
        Overworld(MEMORY::NonNullRawPointer<World>(this)),
        Ark(MEMORY::NonNullRawPointer<World>(this)),
        NoahPlayer(std::make_shared<OBJECTS::Noah>()),
        AnimalsGoingIntoArk()
    {
        // CREATE THE EXIT POINTS BETWEEN MAP GRIDS.
        // The ark interior data used for exit points is currently hard-coded to simplify
        // things since I'm not sure yet exactly how we want to structure this.
        // The ark should always be entered at the bottom layer.
        auto& starting_ark_map_grid = Ark.Interior.LayersFromBottomToTop.front();
        TileMap* starting_ark_interior_tile_map = &*starting_ark_map_grid.TileMaps(1, 0);
        // For now, this is hardcoded to an arbitrary place near the bottom of the starting tile map.
        MATH::FloatRectangle ark_interior_bounding_box = starting_ark_interior_tile_map->GetWorldBoundingBox();
        float ark_interior_center_x_position = ark_interior_bounding_box.CenterX();
        float ark_interior_bottom_y_position = ark_interior_bounding_box.RightBottom.Y;
        MATH::Vector2f ark_interior_bottom_center_position(ark_interior_center_x_position, ark_interior_bottom_y_position);
        MATH::Vector2f ark_interior_player_start_position = ark_interior_bottom_center_position;
        ark_interior_player_start_position.Y -= Tile::DIMENSION_IN_PIXELS<float> * 3.0f;

        MATH::FloatRectangle ark_interior_exit_point_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            ark_interior_player_start_position.X,
            ark_interior_player_start_position.Y + Tile::DIMENSION_IN_PIXELS<float>,
            Tile::DIMENSION_IN_PIXELS<float> * 4.0f,
            Tile::DIMENSION_IN_PIXELS<float> * 2.0f);

        // Note that this is intentionally done in a a not-very-efficient way right now.
        // While we could theoretically create the initial exit points when creating
        // the different map grids, that's not currently being done now due to risk in
        // having incomplete exit points and the fact that I'm not sure yet exactly
        // what form we'll want these exit points in.
        for (unsigned int tile_map_row = 0; tile_map_row < Overworld::HEIGHT_IN_TILE_MAPS; ++tile_map_row)
        {
            for (unsigned int tile_map_column = 0; tile_map_column < Overworld::WIDTH_IN_TILE_MAPS; ++tile_map_column)
            {
                // SEARCH THE CURRENT TILE MAP FOR ANY ARK PIECE EXISTS.
                const auto& overworld_tile_map = Overworld.MapGrid.TileMaps(tile_map_column, tile_map_row);
                for (const auto& ark_piece : overworld_tile_map->ArkPieces)
                {
                    // CREATE EXIT POINTS FOR ANY DOORWAYS.
                    if (ark_piece.IsExternalDoorway)
                    {
                        // CREATE AN EXIT POINT FROM THE OVERWORLD INTO THE ARK.
                        ExitPoint overworld_to_ark_exit_point;
                        MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
                        overworld_to_ark_exit_point.BoundingBox = ark_piece_bounding_box;
                        overworld_to_ark_exit_point.NewMapGrid = &starting_ark_map_grid;
                        overworld_to_ark_exit_point.NewTileMap = starting_ark_interior_tile_map;
                        overworld_to_ark_exit_point.NewPlayerWorldPosition = ark_interior_player_start_position;

                        overworld_tile_map->ExitPoints.push_back(overworld_to_ark_exit_point);

                        // CREATE AN EXIT POINT FROM THE ARK INTERIOR TO THE OVERWORLD.
                        ExitPoint ark_to_overworld_exit_point;
                        ark_to_overworld_exit_point.BoundingBox = ark_interior_exit_point_bounding_box;
                        ark_to_overworld_exit_point.NewMapGrid = &Overworld.MapGrid;
                        ark_to_overworld_exit_point.NewTileMap = &*overworld_tile_map;
                        MATH::Vector2f overworld_exit_starting_position = ark_piece_bounding_box.Center();
                        overworld_exit_starting_position.Y = ark_piece_bounding_box.RightBottom.Y;
                        ark_to_overworld_exit_point.NewPlayerWorldPosition = overworld_exit_starting_position;

                        starting_ark_interior_tile_map->ExitPoints.push_back(ark_to_overworld_exit_point);
                    }
                }
            }
        }
    }


    /// Initializes the built ark within the overworld.
    /// @param[in]  built_ark_pieces - The built pieces of the ark.
    void World::InitializeBuiltArkInOverworld(const std::vector<STATES::BuiltArkPieceTileMapData>& built_ark_pieces)
    {
        for (const auto& built_ark_piece_data : built_ark_pieces)
        {
            // GET THE TILE MAP FOR THE BUILT ARK PIECES.
            TileMap* current_tile_map = Overworld.MapGrid.GetTileMap(built_ark_piece_data.TileMapGridYPosition, built_ark_piece_data.TileMapGridXPosition);
            if (!current_tile_map)
            {
                // Continue trying to initialize other ark pieces.
                /// @todo   Different error handling?
                continue;
            }

            // UPDATE THE BUILT ARK PIECES IN THE CURRENT TILE MAP.
            for (std::size_t ark_piece_index : built_ark_piece_data.BuiltArkPieceIndices)
            {
                // SET THE CURRENT ARK PIECE AS BUILT.
                auto& ark_piece = current_tile_map->ArkPieces.at(ark_piece_index);
                ark_piece.Built = true;
            }
        }
    }
}
