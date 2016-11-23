#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include "Bible/BibleVerses.h"
#include "Collision/Collisions.h"
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Gui/TextBox.h"
#include "Input/KeyboardInputController.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"
#include "States/IntroSequence.h"
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

void InitializePlayer(const MATH::Vector2f& initial_world_position, RESOURCES::Assets& assets, std::unique_ptr<OBJECTS::Noah>& noah_player)
{
    // GET THE TEXTURE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> noah_texture = assets.GetTexture(RESOURCES::NOAH_TEXTURE_ID);
    assert(noah_texture);

    // GET THE AXE TEXTURE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> axe_texture = assets.GetTexture(RESOURCES::AXE_TEXTURE_ID);
    assert(axe_texture);
    
    // GET THE AXE'S SOUND EFFECT.
    std::shared_ptr<AUDIO::SoundEffect> axe_hit_sound = assets.GetSound(RESOURCES::AXE_HIT_SOUND_ID);
    assert(axe_hit_sound);

    // CREATE THE AXE.
    std::shared_ptr<OBJECTS::Axe> axe = std::make_shared<OBJECTS::Axe>(axe_texture, axe_hit_sound);

    // CREATE NOAH.
    noah_player = std::make_unique<OBJECTS::Noah>(noah_texture, axe);

    // SET NOAH'S INITIAL POSITION.
    noah_player->SetWorldPosition(initial_world_position);
}

/// @todo   Document this function.  It looks like it can probably stay in this file for now.
void PopulateOverworld(
    const MAPS::OverworldMapFile& overworld_map_file, 
    const CORE::Array2D<MAPS::TileMapFile>& tile_map_files, 
    RESOURCES::Assets& assets, 
    MAPS::Overworld& overworld)
{
    // LOAD THE TILE MAP FILES.
    /* @todo    This is done in the background in a separate thread. CORE::Array2D<MAPS::TileMapFile> tile_map_files;
    bool tile_map_files_loaded = assets.LoadTileMapFiles(overworld_map_file, tile_map_files);
    assert(tile_map_files_loaded);*/

    // LOAD THE TILESET TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::GROUND_TILESET_TEXTURE_ID);
    assert(tileset_texture);

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

                                // The center of the sprite should be the center of its visible portion of the texture.
                                /// @todo   This is basically being done in the constructor too, but the origin is need for calculating
                                /// the world position too...
                                float tree_local_center_x = static_cast<float>(tree_texture_sub_rectangle.GetWidth()) / 2.0f;
                                float tree_local_center_y = static_cast<float>(tree_texture_sub_rectangle.GetHeight()) / 2.0f;
                                tree_sprite.SetOrigin(MATH::Vector2f(tree_local_center_x, tree_local_center_y));

                                float tree_world_x_position = static_cast<float>(object_description.TopLeftPositionInPixels.X) + tree_local_center_x;
                                float tree_world_y_position = static_cast<float>(object_description.TopLeftPositionInPixels.Y) + tree_local_center_y;
                                tree_sprite.SetWorldPosition(tree_world_x_position, tree_world_y_position);

                                // GET THE TREE SHAKING SOUND EFFECT.
                                std::shared_ptr<AUDIO::SoundEffect> tree_shake_sound = assets.GetSound(RESOURCES::TREE_SHAKE_SOUND_ID);
                                bool tree_shake_sound_retrieved = (nullptr != tree_shake_sound);
                                if (!tree_shake_sound_retrieved)
                                {
                                    /// @todo   Allow creating tree with no sound effect?
                                    assert(false);
                                    continue;
                                }

                                /// @todo   Create a better tree shaking sound effect so that stuff
                                /// doesn't have to be manually modified here.
                                tree_shake_sound->Sound.setVolume(25);
                                tree_shake_sound->Sound.setPitch(2);

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

std::unique_ptr<MAPS::Overworld> LoadOverworld(RESOURCES::Assets& assets)
{
    // LOAD THE OVERWORLD MAP FILE.
    const std::string OVERWORLD_MAP_FILEPATH = "res/maps/overworld_map.json";
    std::unique_ptr<MAPS::OverworldMapFile> overworld_map_file = MAPS::OverworldMapFile::Load(OVERWORLD_MAP_FILEPATH);
    assert(overworld_map_file);

    // LOAD THE TILE MAPS FILES.
    CORE::Array2D<MAPS::TileMapFile> tile_map_files;
    bool tile_map_files_loaded = assets.LoadTileMapFiles(*overworld_map_file, tile_map_files);
    assert(tile_map_files_loaded);
  
    // CREATE THE OVERWORLD.
    std::unique_ptr<MAPS::Overworld> overworld = std::make_unique<MAPS::Overworld>(
        overworld_map_file->OverworldWidthInTileMaps,
        overworld_map_file->OverworldHeightInTileMaps,
        overworld_map_file->TileMapWidthInTiles,
        overworld_map_file->TileMapHeightInTiles,
        overworld_map_file->TileDimensionInPixels);
    PopulateOverworld(*overworld_map_file, tile_map_files, assets, *overworld);
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
        RESOURCES::Assets assets;

        // Some assets are explicitly loaded first as they are needed for initial
        // rendering of the intro sequence and title screen.  By loading them first,
        // quick startup time for the game can be maintained, showing the intro
        // sequence while other assets are being loaded.
        auto font = assets.GetFont(RESOURCES::FONT_TEXTURE_ID);
        assert(font);
        auto colored_texture_shader = assets.GetShader(RESOURCES::ShaderId::COLORED_TEXTURE);
        assert(colored_texture_shader);

        // The overworld is loaded in the background in separate threads to avoid having
        // their loading slow the startup time of the rest of the game.
        std::future< std::unique_ptr<MAPS::Overworld> > overworld_being_loaded = std::async(LoadOverworld, std::ref(assets));

        // INITIALIZE REMAINING SUBSYSTEMS.
        GRAPHICS::Renderer renderer(
            window,
            font,
            colored_texture_shader);
        INPUT_CONTROL::KeyboardInputController input_controller;
        std::random_device random_number_generator;
        std::vector<BIBLE::BibleVerse> bible_verses_left_to_find = BIBLE::BIBLE_VERSES;
        STATES::IntroSequence intro_sequence;
        STATES::TitleScreen title_screen;
        std::unique_ptr<MAPS::Overworld> overworld;
        std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> hud;

#if COMPILE_PROTOTYPE_MUSIC_CODE
        // LOAD MISCELLANEOUS AUDIO RESOURCES.
        /// @todo   Create actual music class?  This background music stuff
        /// is just temporary for now to get something in to see exactly
        /// how music might have to work.  The temporary background music
        /// is pretty horrible.
        sf::Music background_music;
        if (!background_music.openFromFile("res/sounds/temp_background_music.wav"))
        {
            std::cerr << "Failed to load background music." << std::endl;
            /// @todo   More specific exit code.  A specific exit code hasn't been
            /// created yet since this specific loading code is likely temporary.
            return EXIT_FAILURE;
        }
        background_music.setVolume(50.0f);
        background_music.setPitch(0.5f);
        background_music.setLoop(true);
        /// @todo   Start playing when we transition to gameplay state.
        /// Commented out for now while we work on getting intro sequence working.
        /// background_music.play();
#endif

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
                float elapsed_time_in_seconds = elapsed_time.asSeconds();

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
                    {
                        // UPDATE THE HUD IN RESPONSE TO USER INPUT.
                        hud->RespondToInput(input_controller);

                        // CHECK IF THE INVENTORY GUI IS DISPLAYED.
                        // If the inventory GUI is displayed, then the regular controls for the player
                        // in the world shouldn't work.
                        if (!hud->InventoryOpened)
                        {
                            // UPDATE THE TEXT BOX IF IT IS VISIBLE.
                            // If the text box is currently being displayed, then it should capture any user input.
                            if (hud->MainTextBox.IsVisible)
                            {
                                // UPDATE THE TEXT IN THE TEXT BOX.
                                hud->MainTextBox.Update(elapsed_time_in_seconds);
                            }
                            else
                            {
                                // UPDATE THE OVERWORLD.
                                std::string message_for_text_box;
                                overworld->Update(
                                    elapsed_time_in_seconds,
                                    random_number_generator,
                                    input_controller,
                                    bible_verses_left_to_find,
                                    assets,
                                    renderer.Camera,
                                    message_for_text_box);

                                // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
                                bool text_box_message_exists = !message_for_text_box.empty();
                                if (text_box_message_exists)
                                {
                                    hud->MainTextBox.StartDisplayingText(message_for_text_box);
                                }
                            }
                        }
                        break;
                    }
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
                        renderer.Render(elapsed_time_in_seconds, *overworld);
                        hud->Render(renderer);
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
                            /* @todo std::unique_ptr<SavedGameData> saved_game_data = SavedGameData::Load("saved_game.dat");
                            bool saved_game_loaded = (nullptr != saved_game_data);
                            if (saved_game_data_loaded)
                            {
                                // SAVE DATA.
                                // - Starting tile map.
                                // - Starting player position.
                                // - Player inventory.
                            }
                            else*/
                            {
                                // CREATE THE OVERWORLD.
                                bool overworld_exists = (nullptr != overworld);
                                if (!overworld_exists)
                                {
                                    assert(overworld_being_loaded.valid());
                                    overworld = overworld_being_loaded.get();
                                    assert(overworld);

                                    // INITIALIZE THE PLAYER NOAH CHARACTER.
                                    /// @todo   Need to figure out a different way to determine initial position.
                                    const MAPS::TileMap& starting_tile_map = overworld->TileMaps(0, 0);
                                    MATH::Vector2f noah_initial_position = starting_tile_map.GetCenterWorldPosition();
                                    InitializePlayer(noah_initial_position, assets, overworld->NoahPlayer);
                                }

                                // FOCUS THE CAMERA ON THE PLAYER.
                                MATH::Vector2f player_start_world_position = overworld->NoahPlayer->GetWorldPosition();
                                renderer.Camera.SetCenter(player_start_world_position);

                                // INITIALIZE THE HUD.
                                bool hud_exists = (nullptr != hud);
                                if (!hud_exists)
                                {
                                    unsigned int text_box_width_in_pixels = SCREEN_WIDTH_IN_PIXELS;
                                    const unsigned int LINE_COUNT = 2;
                                    unsigned int text_box_height_in_pixels = GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS * LINE_COUNT;
                                    hud = std::make_unique<GRAPHICS::GUI::HeadsUpDisplay>(
                                        overworld->NoahPlayer->Inventory,
                                        text_box_width_in_pixels,
                                        text_box_height_in_pixels,
                                        assets.GetTexture(RESOURCES::AXE_TEXTURE_ID),
                                        assets.GetTexture(RESOURCES::WOOD_LOG_TEXTURE_ID));
                                }
                            }
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
