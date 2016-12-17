#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Input/KeyboardInputController.h"
#include "Resources/Assets.h"
#include "States/GameplayState.h"
#include "States/IntroSequence.h"
#include "States/SavedGameData.h"
#include "States/TitleScreen.h"

/// The game exited successfully.
int EXIT_CODE_SUCCESS = 0;
/// A standard exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN = 1;
/// An unknown exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN = 2;
/// Game assets failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_ASSETS = 3;
/// The font failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_FONT = 4;

/// Enumerates the different states the game could be in.
enum class GameState
{
    /// The introductory sequence for the game.
    INTRO_SEQUENCE,
    /// The game's title screen.
    TITLE_SCREEN,
    /// The main gameplay state for the game.
    GAMEPLAY
};

/// Populates the overworld based on data read from files and assets.
/// @param[in]  overworld_map_file - The overworld map file defining the contents of the overworld.
/// @param[in]  tile_map_files - The individual tile map files that comprise the overworld.
/// @param[in,out]  assets - The assets for the overworld.
/// @param[in,out]  overworld - The overworld to populate.
void PopulateOverworld(
    const MAPS::OverworldMapFile& overworld_map_file, 
    const CORE::Array2D<MAPS::TileMapFile>& tile_map_files, 
    RESOURCES::Assets& assets, 
    MAPS::Overworld& overworld)
{
    // LOAD THE TILESET TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::GROUND_TILESET_TEXTURE_ID);
    assert(tileset_texture);

    // LOAD THE TREE TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::TREE_TEXTURE_ID);
    assert(tree_texture);

    // LOAD TILE MAPS FOR EACH ROW.
    for (unsigned int row = 0; row < overworld_map_file.OverworldHeightInTileMaps; ++row)
    {
        // LOAD TILE MAPS FOR EACH COLUMN.
        for (unsigned int column = 0; column < overworld_map_file.OverworldWidthInTileMaps; ++column)
        {
            // GET THE CURRENT TILE MAP FILE.
            const auto& tile_map_file = tile_map_files(column, row);

            // CREATE THE TILESET FOR THE CURRENT FILE.
            std::unique_ptr<MAPS::Tileset> tileset = MAPS::Tileset::Create(tile_map_file.Tilesets, tileset_texture);
            bool tileset_created = (nullptr != tileset);
            if (!tileset_created)
            {
                assert(tileset_created);
                // Continue trying to create other tile maps.
                continue;
            }

            // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
            MATH::Vector2f map_center_world_position;

            float map_width_in_pixels = static_cast<float>(tile_map_file.MapWidthInTiles * tile_map_file.TileWidthInPixels);
            float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
            float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
            map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

            float map_height_in_pixels = static_cast<float>(tile_map_file.MapHeightInTiles * tile_map_file.TileHeightInPixels);
            float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
            float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
            map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

            // CREATE AN EMPTY TILE MAP.
            MATH::Vector2ui map_dimensions_in_tiles(
                tile_map_file.MapWidthInTiles,
                tile_map_file.MapHeightInTiles);
            MAPS::TileMap tile_map(
                row,
                column,
                map_center_world_position,
                map_dimensions_in_tiles,
                overworld_map_file.TileDimensionInPixels);

            // POPULATE THE TILE MAP FROM THE CURRENT FILE'S LAYERS.
            for (const auto& layer_description : tile_map_file.Layers)
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
                                current_tile_y < tile_map_file.MapHeightInTiles;
                                ++current_tile_y)
                            {
                                // CREATE TILES FOR THIS ROW.
                                for (unsigned int current_tile_x = 0;
                                    current_tile_x < tile_map_file.MapWidthInTiles;
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
                                    tile_map.Ground.SetTile(current_tile_x, current_tile_y, tile);
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
                                // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                                // Different sub-rectangles are used depending on the tree's size.
                                MATH::FloatRectangle tree_texture_sub_rectangle;
                                const MATH::Vector2ui SMALL_TREE_DIMENSIONS_IN_PIXELS(16, 16);
                                const MATH::Vector2ui TALL_TREE_DIMENSIONS_IN_PIXELS(16, 32);
                                const MATH::Vector2ui LARGE_TREE_DIMENSIONS_IN_PIXELS(32, 32);
                                MATH::Vector2ui tree_dimensions_in_pixels(object_description.WidthInPixels, object_description.HeightInPixels);
                                if (SMALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    MATH::FloatRectangle SMALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(0.0f, 0.0f, 16.0f, 16.0f);
                                    tree_texture_sub_rectangle = SMALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (TALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    MATH::FloatRectangle TALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(32.0f, 0.0f, 16.0f, 32.0f);
                                    tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (LARGE_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    MATH::FloatRectangle LARGE_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(0.0f, 16.0f, 32.0f, 32.0f);
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
                                GRAPHICS::Sprite tree_sprite(tree_texture, tree_texture_sub_rectangle);
                                MATH::Vector2f tree_local_center = tree_sprite.GetOrigin();
                                float tree_world_x_position = static_cast<float>(object_description.TopLeftPositionInPixels.X) + tree_local_center.X;
                                float tree_world_y_position = static_cast<float>(object_description.TopLeftPositionInPixels.Y) + tree_local_center.Y;
                                tree_sprite.SetWorldPosition(tree_world_x_position, tree_world_y_position);

                                // GET THE TREE SHAKING SOUND EFFECT.
                                // If the sound can't be retrieved, then the game will continue just with no sound playing
                                // when trees shake.
                                std::shared_ptr<AUDIO::SoundEffect> tree_shake_sound = assets.GetSound(RESOURCES::TREE_SHAKE_SOUND_ID);
                                bool tree_shake_sound_retrieved = (nullptr != tree_shake_sound);

                                // CREATE THE TREE.
                                OBJECTS::Tree tree;
                                tree.Sprite = tree_sprite;
                                tree.TreeShakeSound = tree_shake_sound;
                                tile_map.Trees.push_back(tree);
                            }
                        }

                        break;
                    }
                }
            }

            // SET THE TILE MAP IN THE OVERWORLD.
            overworld.TileMaps(column, row) = std::move(tile_map);
        }
    }
}

/// Loads tile map files specified in the overworld map file.
/// @param[in]  overworld_map_file - The overworld map file definining the tile map files to load.
/// @param[out] tile_map_files - The tile map files, if successfully loaded.
/// @return True if loading succeeded; false otherwise.
bool LoadTileMapFiles(
    const MAPS::OverworldMapFile& overworld_map_file,
    CORE::Array2D<MAPS::TileMapFile>& tile_map_files)
{
    // ALLOCATE SPACE FOR ALL TILE MAP FILES IN THE OVERWORLD.
    // Resizing is done to allocate enough space for the new tile map files
    // while clearing old data at the same time.
    unsigned int tile_map_row_count = overworld_map_file.OverworldHeightInTileMaps;
    unsigned int tile_map_column_count = overworld_map_file.OverworldWidthInTileMaps;
    tile_map_files.Resize(tile_map_column_count, tile_map_row_count);

    // DEFINE AN ARRAY OF FUTURES FOR LOADING THE TILE MAPS IN PARALLEL.
    // Asynchronous loading is done since it is a little faster than synchronous loading
    // (about 7-8 seconds of total asset load time as opposed to 12-13 seconds total).
    CORE::Array2D< std::future< std::unique_ptr<MAPS::TileMapFile> > > tile_map_loaders(
        tile_map_column_count,
        tile_map_row_count);

    // START TO LOAD ALL TILE MAP FILES IN THE OVERWORLD MAP FILE.
    for (unsigned int tile_map_row = 0; tile_map_row < tile_map_row_count; ++tile_map_row)
    {
        for (unsigned int tile_map_column = 0; tile_map_column < tile_map_column_count; ++tile_map_column)
        {
            // STARTING LOADING THE CURRENT TILE MAP.
            std::string tile_map_filepath = overworld_map_file.GetTileMapFilepath(tile_map_row, tile_map_column);
            tile_map_loaders(tile_map_column, tile_map_row) = std::async(MAPS::TileMapFile::Load, tile_map_filepath);
        }
    }

    // OBTAIN ALL OF THE TILE MAP FILES BEING LOADED ASYNCHRONOUSLY.
    for (unsigned int tile_map_row = 0; tile_map_row < tile_map_row_count; ++tile_map_row)
    {
        for (unsigned int tile_map_column = 0; tile_map_column < tile_map_column_count; ++tile_map_column)
        {
            try
            {
                // CHECK IF THE TILE MAP LOADER FOR THE CURRENT ROW/COLUMN IS VALID.
                auto& tile_map_loader = tile_map_loaders(tile_map_column, tile_map_row);
                if (tile_map_loader.valid())
                {
                    // WAIT AND GET THE LOADED TILE MAP FILE.
                    std::unique_ptr<MAPS::TileMapFile> tile_map_file = tile_map_loader.get();
                    bool tile_map_file_loaded = (nullptr != tile_map_file);
                    if (tile_map_file_loaded)
                    {
                        // STORE THE TILE MAP FILE IN THIS COLLECTION OF ASSETS.
                        tile_map_files(tile_map_column, tile_map_row) = *tile_map_file;
                    }
                    else
                    {
                        assert(false);
                        return false;
                    }
                }
            }
            catch (const std::exception&)
            {
                assert(false);
                return false;
            }
        }
    }

    // All tile map files were loaded successfully.
    return true;
}

/// Loads the overworld.
/// @param[in]  assets - The assets to use for the overworld.
/// @return The overworld, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::Overworld> LoadOverworld(RESOURCES::Assets& assets)
{
    // LOAD THE OVERWORLD MAP FILE.
    const std::string OVERWORLD_MAP_FILEPATH = "res/maps/overworld_map.json";
    std::unique_ptr<MAPS::OverworldMapFile> overworld_map_file = MAPS::OverworldMapFile::Load(OVERWORLD_MAP_FILEPATH);
    assert(overworld_map_file);

    // LOAD THE TILE MAPS FILES.
    CORE::Array2D<MAPS::TileMapFile> tile_map_files;
    bool tile_map_files_loaded = LoadTileMapFiles(*overworld_map_file, tile_map_files);
    assert(tile_map_files_loaded);
  
    // CREATE THE OVERWORLD.
    std::shared_ptr<MAPS::Overworld> overworld = std::make_shared<MAPS::Overworld>(
        overworld_map_file->OverworldWidthInTileMaps,
        overworld_map_file->OverworldHeightInTileMaps,
        overworld_map_file->TileMapWidthInTiles,
        overworld_map_file->TileMapHeightInTiles,
        overworld_map_file->TileDimensionInPixels);
    PopulateOverworld(*overworld_map_file, tile_map_files, assets, *overworld);

    // LOAD THE BACKGROUND MUSIC.
    std::shared_ptr<sf::Music> overworld_background_music = assets.GetMusic(RESOURCES::OVERWORLD_BACKGROUND_MUSIC_ID);
    bool background_music_loaded = (nullptr != overworld_background_music);
    assert(background_music_loaded);
    overworld->BackgroundMusic = overworld_background_music;

    return overworld;
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
        // CREATE THE WINDOW.
        // The display area is set to the dimensions of a single tile map.
        const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
        const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // Ensure that only one key event is generated for each key press.
        window->setKeyRepeatEnabled(false);

        // LOAD THE ASSETS.
        std::shared_ptr<RESOURCES::Assets> assets = std::make_shared<RESOURCES::Assets>();

        // Some assets are explicitly loaded first as they are needed for initial
        // rendering of the intro sequence and title screen.  By loading them first,
        // quick startup time for the game can be maintained, showing the intro
        // sequence while other assets are being loaded.
        auto font = assets->GetFont(RESOURCES::FONT_TEXTURE_ID);
        assert(font);
        auto colored_texture_shader = assets->GetShader(RESOURCES::ShaderId::COLORED_TEXTURE);
        assert(colored_texture_shader);

        // The overworld is loaded in the background in separate threads to avoid having
        // their loading slow the startup time of the rest of the game.
        std::future< std::shared_ptr<MAPS::Overworld> > overworld_being_loaded = std::async(LoadOverworld, std::ref(*assets));

        // INITIALIZE REMAINING SUBSYSTEMS.
        GRAPHICS::Renderer renderer(
            window,
            font,
            colored_texture_shader);
        INPUT_CONTROL::KeyboardInputController input_controller;
        STATES::IntroSequence intro_sequence;
        STATES::TitleScreen title_screen;
        STATES::GameplayState gameplay_state(assets);

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        GameState game_state = GameState::INTRO_SEQUENCE;
        bool game_started = false;
        sf::Clock game_loop_clock;
        while (window->isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window->pollEvent(event))
            {
                // Handle the current event based on its type.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window->close();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window->isOpen())
            {
                // READ USER INPUT.
                input_controller.ReadInput();

                // GET THE ELAPSED TIME FOR THE NEW FRAME.
                sf::Time elapsed_time = game_loop_clock.restart();

                // UPDATE THE GAME'S CURRENT STATE.
                switch (game_state)
                {
                    case GameState::INTRO_SEQUENCE:
                        intro_sequence.Update(elapsed_time);
                        break;
                    case GameState::TITLE_SCREEN:
                        game_started = title_screen.RespondToInput(input_controller);
                        break;
                    case GameState::GAMEPLAY:
                        gameplay_state.Update(elapsed_time, input_controller, renderer.Camera);
                        break;
                }

                // CLEAR THE SCREEN OF THE PREVIOUSLY RENDERED FRAME.
                renderer.Screen.Clear();

                // RENDER THE CURRENT STATE OF THE GAME.
                switch (game_state)
                {
                    case GameState::INTRO_SEQUENCE:
                        intro_sequence.Render(renderer);
                        break;
                    case GameState::TITLE_SCREEN:
                        title_screen.Render(renderer);
                        break;
                    case GameState::GAMEPLAY:
                        gameplay_state.Render(renderer);
                        break;
                }

                // DISPLAY THE RENDERED FRAME IN THE WINDOW.
                window->display();

                // MOVE TO A NEW GAME STATE IF THE APPROPRIATE CONDITIONS HAVE BEEN REACHED.
                switch (game_state)
                {
                    case GameState::INTRO_SEQUENCE:
                    {
                        // MOVE TO THE TITLE SCREEN IF THE INTRO SEQUENCE HAS FINISHED.
                        bool intro_sequence_finished = intro_sequence.Completed();
                        if (intro_sequence_finished)
                        {
                            game_state = GameState::TITLE_SCREEN;
                        }
                        break;
                    }
                    case GameState::TITLE_SCREEN:
                    {
                        // MOVE TO THE MAIN GAMEPLAY IF THE USER CHOSE TO START THE GAME.
                        if (game_started)
                        {
                            // SET THE GAME'S STATE.
                            game_state = GameState::GAMEPLAY;

                            // LOAD THE GAME'S SAVE FILE.
                            std::unique_ptr<STATES::SavedGameData> saved_game_data = STATES::SavedGameData::Load(STATES::SavedGameData::DEFAULT_FILENAME);
                            bool saved_game_data_loaded = (nullptr != saved_game_data);
                            if (!saved_game_data_loaded)
                            {
                                // USE THE DEFAULT SAVED GAME DATA FOR A NEW GAME.
                                saved_game_data = std::make_unique<STATES::SavedGameData>(STATES::SavedGameData::DefaultSavedGameData());
                            }

                            // INITIALIZE THE GAMEPLAY STATE.
                            assert(overworld_being_loaded.valid());
                            auto overworld = overworld_being_loaded.get();
                            assert(overworld);

                            bool gameplay_state_initialized = gameplay_state.Initialize(
                                SCREEN_WIDTH_IN_PIXELS,
                                *saved_game_data,
                                overworld);
                            assert(gameplay_state_initialized);

                            // FOCUS THE CAMERA ON THE PLAYER.
                            MATH::Vector2f player_start_world_position = gameplay_state.Overworld->NoahPlayer->GetWorldPosition();
                            renderer.Camera.SetCenter(player_start_world_position);
                        }
                        break;
                    }
                }
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
