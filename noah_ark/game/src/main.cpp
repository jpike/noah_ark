#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Resources/Assets.h"

/// The game exited successfully.
int EXIT_CODE_SUCCESS = 0;
/// A standard exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN = 1;
/// An unknown exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN = 2;
/// Game assets failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_ASSETS = 3;

void PopulateOverworld(const MAPS::OverworldMapFile& overworld_map_file, RESOURCES::Assets& assets, MAPS::Overworld& overworld)
{
    // LOAD TILE MAPS FOR EACH ROW.
    for (unsigned int row = 0; row < overworld_map_file.OverworldHeightInTileMaps; ++row)
    {
        // LOAD TILE MAPS FOR EACH COLUMN.
        for (unsigned int column = 0; column < overworld_map_file.OverworldWidthInTileMaps; ++column)
        {
            // GET THE CURRENT TILE MAP FILE.
            const auto& tile_map_file = assets.TileMapFiles(column, row);
            bool tile_map_file_found = (nullptr != tile_map_file);
            if (!tile_map_file_found)
            {
                assert(tile_map_file_found);
                // Continue trying to create other tile maps.
                continue;
            }

            // GET THE TILESET FOR THE CURRENT FILE.
            std::shared_ptr<MAPS::Tileset> tileset = assets.GetTileset(tile_map_file->Tilesets);
            bool tileset_found = (nullptr != tileset);
            if (!tileset_found)
            {
                assert(tileset_found);
                // Continue trying to create other tile maps.
                continue;
            }

            // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
            MATH::Vector2f map_center_world_position;

            float map_width_in_pixels = static_cast<float>(tile_map_file->MapWidthInTiles * tile_map_file->TileWidthInPixels);
            float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
            float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
            map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

            float map_height_in_pixels = static_cast<float>(tile_map_file->MapHeightInTiles * tile_map_file->TileHeightInPixels);
            float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
            float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
            map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

            // CREATE AN EMPTY TILE MAP.
            MATH::Vector2ui map_dimensions_in_tiles(
                tile_map_file->MapWidthInTiles,
                tile_map_file->MapHeightInTiles);
            std::shared_ptr<MAPS::TileMap> tile_map = std::make_shared<MAPS::TileMap>(
                map_center_world_position,
                map_dimensions_in_tiles);

            // POPULATE THE TILE MAP FROM THE CURRENT FILE'S LAYERS.
            for (const auto& layer_description : tile_map_file->Layers)
            {
                // DETERMINE THE TYPE OF THE LAYER.
                switch (layer_description.Type)
                {
                    case MAPS::TileMapLayerType::TILE_LAYER:
                    {
                        // CHECK IF THIS LAYER IS THE GROUND LAYER.
                        const std::string GROUND_LAYER_NAME = "GroundLayer";
                        bool is_ground_layer = (GROUND_LAYER_NAME == layer_description.Name);
                        if (is_ground_layer)
                        {
                            // CREATE TILES IN THE GROUND LAYER.
                            for (unsigned int current_tile_y = 0;
                                current_tile_y < tile_map_file->MapHeightInTiles;
                                ++current_tile_y)
                            {
                                // CREATE TILES FOR THIS ROW.
                                for (unsigned int current_tile_x = 0;
                                    current_tile_x < tile_map_file->MapWidthInTiles;
                                    ++current_tile_x)
                                {
                                    // CREATE THE CURRENT TILE.
                                    MAPS::TileId tile_id = layer_description.TileIds(current_tile_x, current_tile_y);
                                    std::shared_ptr<MAPS::Tile> tile = tileset->CreateTile(tile_id);
                                    bool tile_exists_in_tileset = (nullptr != tile);
                                    if (!tile_exists_in_tileset)
                                    {
                                        assert(tile_exists_in_tileset);
                                        // Skip to trying to create the next tile.  The layer
                                        // simply won't have any tile at this location.
                                        continue;
                                    }

                                    // SET THE TILE IN THE GROUND LAYER.
                                    tile_map->Ground.SetTile(current_tile_x, current_tile_y, tile);
                                }
                            }
                        }

                        break;
                    }
                    case MAPS::TileMapLayerType::OBJECT_LAYER:
                    {
                        // CREATE ANY TREES IN THE LAYER.
                        for (const auto& object_description : layer_description.Objects)
                        {
                            const std::string TREE_OBJECT_TYPE = "TREE";
                            bool is_tree = (TREE_OBJECT_TYPE == object_description.Type);
                            if (is_tree)
                            {
                                // GET THE TREE TEXTURE.
                                std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::TREE_TEXTURE_ID);
                                bool tree_texture_retrieved = (nullptr != tree_texture);
                                if (!tree_texture_retrieved)
                                {
                                    assert(tree_texture_retrieved);
                                    // Continue trying to create other objects.
                                    /// @todo   Maybe we should just load this once...as opposed to performing each iteration?
                                    continue;
                                }

                                // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                                // Different sub-rectangles are used depending on the tree's size.
                                sf::IntRect tree_texture_sub_rectangle;
                                const MATH::Vector2ui SMALL_TREE_DIMENSIONS_IN_PIXELS(16, 16);
                                const MATH::Vector2ui TALL_TREE_DIMENSIONS_IN_PIXELS(16, 32);
                                const MATH::Vector2ui LARGE_TREE_DIMENSIONS_IN_PIXELS(32, 32);
                                MATH::Vector2ui tree_dimensions_in_pixels(object_description.WidthInPixels, object_description.HeightInPixels);
                                if (SMALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect SMALL_TREE_TEXTURE_SUB_RECTANGLE(0, 0, 16, 16);
                                    tree_texture_sub_rectangle = SMALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (TALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect TALL_TREE_TEXTURE_SUB_RECTANGLE(32, 0, 16, 32);
                                    tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (LARGE_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect LARGE_TREE_TEXTURE_SUB_RECTANGLE(0, 16, 32, 32);
                                    tree_texture_sub_rectangle = LARGE_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else
                                {
                                    // An invalid size was specified for the tree.
                                    // Continue trying to create other objects.
                                    assert(false);
                                    continue;
                                }

                                // CREATE THE TREE'S SPRITE.
                                std::shared_ptr<sf::Sprite> tree_sprite_resource = std::make_shared<sf::Sprite>(
                                    *tree_texture->GetTextureResource(),
                                    tree_texture_sub_rectangle);

                                // The center of the sprite should be the center of its visible portion of the texture.
                                float tree_local_center_x = static_cast<float>(tree_texture_sub_rectangle.width) / 2.0f;
                                float tree_local_center_y = static_cast<float>(tree_texture_sub_rectangle.height) / 2.0f;
                                tree_sprite_resource->setOrigin(tree_local_center_x, tree_local_center_y);

                                float tree_world_x_position = static_cast<float>(object_description.TopLeftPositionInPixels.X) + tree_local_center_x;
                                float tree_world_y_position = static_cast<float>(object_description.TopLeftPositionInPixels.Y) + tree_local_center_y;
                                tree_sprite_resource->setPosition(tree_world_x_position, tree_world_y_position);

                                std::shared_ptr<GRAPHICS::Sprite> tree_sprite = std::make_shared<GRAPHICS::Sprite>(tree_sprite_resource);

                                // CREATE THE TREE.
                                OBJECTS::Tree tree(tree_sprite);
                                tile_map->Trees.push_back(tree);
                            }
                        }

                        break;
                    }
                }
            }

            // SET THE TILE MAP IN THE OVERWORLD.
            overworld.TileMaps(column, row) = tile_map;
        }
    }
}

void Render(GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target)
{
    sprite.Render(render_target);
}

/// The main entry point for the game.
/// Runs the Noah's Ark game until the user
/// chooses to exit or an error occurs.
/// @param[in]  argumentCount - The number of command line arguments.
/// @param[in]  arguments - The command line arguments.
/// @return     One of the exit codes defined in NoahArkGame.
int main(int argumentCount, char* arguments[])
{
    try
    {
        // LOAD ALL GAME ASSETS.
        // This will prevent slowdowns from having to wait on disk I/O later
        // for reading assets in the middle of gameplay.
        RESOURCES::Assets assets;
        bool assets_loaded = assets.LoadAll();
        if (!assets_loaded)
        {
            /// @todo   Provide a more specific error from Assets class for loading failures?
            std::cerr << "Failed to load assets." << std::endl;
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        // CREATE THE OVERWORLD.
        const MAPS::OverworldMapFile* const overworld_map_file = assets.GetOverworldMapFile();
        MAPS::Overworld overworld(overworld_map_file->OverworldWidthInTileMaps, overworld_map_file->OverworldHeightInTileMaps);
        PopulateOverworld(*overworld_map_file, assets, overworld);

        // CREATE THE WINDOW.
        // The width and height are currently set to match the dimensions
        // of a single tile map in the game.
        const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
        const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        sf::RenderWindow window(
            sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        unsigned int min_tile_map_row = 0;
        unsigned int min_tile_map_column = 0;
        sf::Clock game_loop_clock;
        while (window.isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window.pollEvent(event))
            {
                // Handle the current event based on its type.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        switch (event.key.code)
                        {
                            case sf::Keyboard::Up:
                                --min_tile_map_row;
                                break;
                            case sf::Keyboard::Down:
                                ++min_tile_map_row;
                                break;
                            case sf::Keyboard::Left:
                                --min_tile_map_column;
                                break;
                            case sf::Keyboard::Right:
                                ++min_tile_map_column;
                                break;
                        }

                        if (min_tile_map_row < 0)
                        {
                            min_tile_map_row = 0;
                        }
                        if (min_tile_map_row > 6)
                        {
                            min_tile_map_row = 6;
                        }
                        if (min_tile_map_column < 0)
                        {
                            min_tile_map_column = 0;
                        }
                        if (min_tile_map_column > 6)
                        {
                            min_tile_map_column = 6;
                        }

                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window.isOpen())
            {
                // Update the game for the new frame.
                sf::Time elapsed_time = game_loop_clock.restart();
                /// @todo   Update here.

                // Render the current state of the game.
                window.clear();
                /// @todo   Render here.
                unsigned int max_tile_map_row = min_tile_map_row + 1;
                unsigned int max_tile_map_column = min_tile_map_column + 1;
                for (unsigned int tile_map_row = min_tile_map_row; tile_map_row < max_tile_map_row; ++tile_map_row)
                {
                    for (unsigned int tile_map_column = min_tile_map_column; tile_map_column < max_tile_map_column; ++tile_map_column)
                    {
                        // GET THE CURRENT TILE MAP.
                        std::shared_ptr<MAPS::TileMap> tile_map = overworld.TileMaps(tile_map_column, tile_map_row);
                        bool tile_map_exists = (nullptr != tile_map);
                        if (!tile_map_exists)
                        {
                            // Continue trying to render other tile maps.
                            continue;
                        }

                        /// @todo   Camera.
                        auto view = window.getView();
                        MATH::Vector2f center_world_position = tile_map->GetCenterWorldPosition();
                        view.setCenter(center_world_position.X, center_world_position.Y);
                        window.setView(view);

                        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
                        MATH::Vector2ui ground_dimensions_in_tiles = tile_map->GetDimensionsInTiles();
                        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
                        {
                            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
                            {
                                std::shared_ptr<MAPS::Tile> tile = tile_map->Ground.Tiles(tile_column, tile_row);
                                std::shared_ptr<GRAPHICS::Sprite> tile_sprite = tile->GetSprite();
                                Render(*tile_sprite, window);
                            }
                        }

                        // RENDER THE CURRENT TILE MAP'S TREES.
                        for (const auto& tree : tile_map->Trees)
                        {
                            std::shared_ptr<GRAPHICS::Sprite> tree_sprite = tree.GetSprite();
                            Render(*tree_sprite, window);
                        }
                    }
                }

                window.display();
            }
        }

        // INDICATE THAT THE GAME EXITED SUCCESSFULLY.
        return EXIT_CODE_SUCCESS;
    }
    catch (std::exception& exception)
    {
        std::cerr << "Standard exception in main: " << exception.what() << std::endl;
        return EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN;
    }
    catch (...)
    {   
        std::cerr << "Unknown exception in main." << std::endl;
        return EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN;
    }
}