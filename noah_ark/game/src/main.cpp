#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Input/InputController.h"
#include "Maps/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Math/Number.h"
#include "Resources/AnimalSounds.h"
#include "Resources/Assets.h"
#include "Resources/FoodGraphics.h"
#include "States/CreditsScreen.h"
#include "States/PreFloodGameplayState.h"
#include "States/GameState.h"
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

/// Populates the overworld based on data read from in-memory assets.
/// @param[in,out]  assets - The assets for the overworld.
/// @param[in,out]  overworld - The overworld to populate.
void PopulateOverworld(
    RESOURCES::Assets& assets,
    MAPS::Overworld& overworld)
{
    // LOAD THE TILESET TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::MAIN_TILESET_TEXTURE_ID);
    assert(tileset_texture);

    // LOAD THE TREE TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::TREE_TEXTURE_ID);
    assert(tree_texture);

    // CREATE THE TILESET.
    MAPS::Tileset tileset(tileset_texture);

    // CREATE A RANDOM NUMBER GENERATOR FOR CREATING FOOD.
    MATH::RandomNumberGenerator random_number_generator;

    // LOAD TILE MAPS FOR EACH ROW.
    for (unsigned int row = 0; row < MAPS::Overworld::HEIGHT_IN_TILE_MAPS; ++row)
    {
        // LOAD TILE MAPS FOR EACH COLUMN.
        for (unsigned int column = 0; column < MAPS::Overworld::WIDTH_IN_TILE_MAPS; ++column)
        {
            // GET THE CURRENT TILE MAP FILE.
            const auto& tile_map_data = MAPS::OVERWORLD_MAP_DATA(column, row);

            // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
            MATH::Vector2f map_center_world_position;

            float map_width_in_pixels = static_cast<float>(MAPS::TileMap::WIDTH_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
            float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
            float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
            map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

            float map_height_in_pixels = static_cast<float>(MAPS::TileMap::HEIGHT_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
            float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
            float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
            map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

            // CREATE AN EMPTY TILE MAP.
            MATH::Vector2ui map_dimensions_in_tiles(
                MAPS::TileMap::WIDTH_IN_TILES,
                MAPS::TileMap::HEIGHT_IN_TILES);
            MAPS::TileMap tile_map(
                row,
                column,
                map_center_world_position,
                map_dimensions_in_tiles,
                MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);

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
                    MAPS::TileId tile_id = (*tile_map_data.GroundLayer)(current_tile_x, current_tile_y);
                    std::shared_ptr<MAPS::Tile> tile = tileset.CreateTile(tile_id);
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

            // POPULATE THE ARK LAYER IF ONE EXISTS.
            if (tile_map_data.ArkLayer)
            {
                // GET THE ARK TEXTURE.
                std::shared_ptr<GRAPHICS::Texture> ark_texture = assets.GetTexture(RESOURCES::ARK_TEXTURE_ID);
                assert(ark_texture);

                // CREATE PIECES IN THE ARK LAYER.
                for (unsigned int current_tile_y = 0;
                    current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                    ++current_tile_y)
                {
                    // CREATE ARK PIECES FOR THIS ROW.
                    for (unsigned int current_tile_x = 0;
                        current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                        ++current_tile_x)
                    {
                        // CHECK IF THE TILE ID IS VALID.
                        // Some tiles in this layer may not be for valid ark pieces.
                        MAPS::TileId tile_id = (*tile_map_data.ArkLayer)(current_tile_x, current_tile_y);
                        bool tild_id_valid = (tile_id > 0);
                        if (!tild_id_valid)
                        {
                            continue;
                        }

                        // CREATE THE ARK PIECE.
                        OBJECTS::ArkPiece ark_piece(tile_id, ark_texture);
                        MATH::Vector2f ark_piece_local_center = ark_piece.Sprite.GetOrigin();
                        float tile_left_x_position = static_cast<float>(current_tile_x * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                        float ark_piece_world_x_position = map_left_world_position + tile_left_x_position + ark_piece_local_center.X;
                        float tile_top_y_position = static_cast<float>(current_tile_y * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                        float ark_piece_world_y_position = map_top_world_position + tile_top_y_position + ark_piece_local_center.Y;
                        ark_piece.Sprite.SetWorldPosition(ark_piece_world_x_position, ark_piece_world_y_position);

                        // ADD THE ARK PIECE TO THE TILE MAP.
                        tile_map.ArkPieces.push_back(ark_piece);
                    }
                }
            }

            // POPULATE THE OBJECT LAYER IF ONE EXISTS.
            if (tile_map_data.ObjectLayer)
            {
                // CREATE ANY TREES IN THE LAYER.
                for (unsigned int current_tile_y = 0;
                    current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                    ++current_tile_y)
                {
                    // CREATE ARK PIECES FOR THIS ROW.
                    for (unsigned int current_tile_x = 0;
                        current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                        ++current_tile_x)
                    {
                        // CHECK IF THE TILE ID IS VALID.
                        // Some tiles in this layer may not be for valid ark pieces.
                        uint8_t object_id = (*tile_map_data.ObjectLayer)(current_tile_x, current_tile_y);
                        const uint8_t TREE_OBJECT_ID = 1;
                        bool is_tree = (TREE_OBJECT_ID == object_id);
                        if (is_tree)
                        {
                            // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                            MATH::FloatRectangle TALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(32.0f, 0.0f, 16.0f, 32.0f);
                            MATH::FloatRectangle tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;

                            // CREATE THE TREE'S SPRITE.
                            GRAPHICS::Sprite tree_sprite(tree_texture, tree_texture_sub_rectangle);
                            MATH::Vector2f tree_local_center = tree_sprite.GetOrigin();
                            auto tree_left_x = current_tile_x * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>;
                            auto tree_top_y = current_tile_y * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>;
                            float tree_world_x_position = map_left_world_position + static_cast<float>(tree_left_x) + tree_local_center.X;
                            float tree_world_y_position = map_top_world_position + static_cast<float>(tree_top_y) + tree_local_center.Y;
                            tree_sprite.SetWorldPosition(tree_world_x_position, tree_world_y_position);

                            // CREATE ANY FOOD ON THE TREE.
                            // Food will be randomly generated.
                            std::optional<OBJECTS::Food> food = std::nullopt;
                            unsigned int random_number_for_food_existing = random_number_generator.RandomNumber<unsigned int>();
                            bool food_on_tree = MATH::Number::IsEven(random_number_for_food_existing);
                            if (food_on_tree)
                            {
                                // DETERMINE THE TYPE OF FOOD.
                                // It will be randomly determined.
                                OBJECTS::FoodType food_type = random_number_generator.RandomEnum<OBJECTS::FoodType>();

                                // GET THE SPRITE FOR THE FOOD.
                                // The food can only be created if the sprite was retrieved.
                                std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type, assets);
                                if (food_sprite)
                                {
                                    // CREATE THE FOOD.
                                    food = OBJECTS::Food();
                                    food->Type = food_type;
                                    food->Sprite = GRAPHICS::Sprite(*food_sprite);

                                    // The food should be positioned on the tree.
                                    food->Sprite.SetWorldPosition(tree_sprite.GetWorldPosition());
                                }
                            }

                            // CREATE THE TREE.
                            OBJECTS::Tree tree;
                            tree.Sprite = tree_sprite;
                            tree.Food = food;
                            tile_map.Trees.push_back(tree);
                        }
                    }
                }
            }

            // SET THE TILE MAP IN THE OVERWORLD.
            overworld.TileMaps(column, row) = std::move(tile_map);
        }
    }
}

/// Loads the overworld.
/// @param[in]  assets - The assets to use for the overworld.
/// @return The overworld, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::Overworld> LoadOverworld(RESOURCES::Assets& assets)
{
    // CREATE THE OVERWORLD.
    auto load_start_time = std::chrono::system_clock::now();
    std::shared_ptr<MAPS::Overworld> overworld = std::make_shared<MAPS::Overworld>();
    PopulateOverworld(assets, *overworld);

    auto load_end_time = std::chrono::system_clock::now();

    // Overworld load time: 82263372 (file-based)
    // Overworld load time: 17972479 (array-based)
    auto load_time_diff = load_end_time - load_start_time;
    std::cout << "Overworld load time: " << load_time_diff.count() << std::endl;

    return overworld;
}

/// Loads the sounds/music for the game, adding them to the speakers.
/// @param[in,out]  assets - The assets from which to load the sounds/music.
/// @return The speakers with the sounds/music; never null.
std::shared_ptr<AUDIO::Speakers> LoadSounds(RESOURCES::Assets& assets)
{
    std::shared_ptr<AUDIO::Speakers> speakers = std::make_shared<AUDIO::Speakers>();

    // ADD EACH GENERIC SOUND EFFECT TO THE SPEAKERS.
    const std::array<std::string, 5> GENERIC_SOUND_EFFECT_IDS =
    {
        RESOURCES::ARK_BUILDING_SOUND_ID,
        RESOURCES::AXE_HIT_SOUND_ID,
        RESOURCES::COLLECT_BIBLE_VERSE_SOUND_ID,
        RESOURCES::FOOD_PICKUP_SOUND_ID,
        RESOURCES::TREE_SHAKE_SOUND_ID

    };
    for (const std::string& sound_id : GENERIC_SOUND_EFFECT_IDS)
    {
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(sound_id);
        if (audio_samples)
        {
            speakers->AddSound(sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE ANIMAL SOUND EFFECTS TO THE SPEAKERS.
    for (int species_id = 0; species_id < static_cast<int>(OBJECTS::AnimalSpecies::COUNT); ++species_id)
    {
        OBJECTS::AnimalSpecies species = static_cast<OBJECTS::AnimalSpecies>(species_id);
        std::string animal_sound_id = RESOURCES::AnimalSounds::GetSound(species);
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(animal_sound_id);
        if (audio_samples)
        {
            speakers->AddSound(animal_sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE MUSIC TO THE SPEAKERS.    
    std::shared_ptr<sf::Music> intro_music = assets.GetMusic(RESOURCES::INTRO_MUSIC_ID);
    if (intro_music)
    {
        speakers->AddMusic(RESOURCES::INTRO_MUSIC_ID, intro_music);
    }
    std::shared_ptr<sf::Music> overworld_music = assets.GetMusic(RESOURCES::OVERWORLD_BACKGROUND_MUSIC_ID);
    if (overworld_music)
    {
        overworld_music->setLoop(true);
        speakers->AddMusic(RESOURCES::OVERWORLD_BACKGROUND_MUSIC_ID, overworld_music);
    }
    
    return speakers;
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

        // LOAD THE SOUND EFFECTS/MUSIC.
        std::future< std::shared_ptr<AUDIO::Speakers> > speakers_being_loaded = std::async(LoadSounds, std::ref(*assets));

        // The overworld is loaded in the background in separate threads to avoid having
        // their loading slow the startup time of the rest of the game.
        std::future< std::shared_ptr<MAPS::Overworld> > overworld_being_loaded = std::async(LoadOverworld, std::ref(*assets));

        // INITIALIZE REMAINING SUBSYSTEMS.
        GRAPHICS::Renderer renderer(
            window,
            font,
            colored_texture_shader);
        INPUT_CONTROL::InputController input_controller;
        STATES::IntroSequence intro_sequence;
        auto speakers = speakers_being_loaded.get();
        assert(speakers);
        speakers->PlayMusic(RESOURCES::INTRO_MUSIC_ID);
        STATES::TitleScreen title_screen;
        STATES::CreditsScreen credits_screen;
        STATES::PreFloodGameplayState pre_flood_gameplay_state(speakers, assets);

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        STATES::GameState game_state = STATES::GameState::INTRO_SEQUENCE;
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
                sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(*window);
                input_controller.Mouse.ScreenPosition = MATH::Vector2f(
                    static_cast<float>(mouse_screen_position.x),
                    static_cast<float>(mouse_screen_position.y));
                input_controller.ReadInput();

                // GET THE ELAPSED TIME FOR THE NEW FRAME.
                sf::Time elapsed_time = game_loop_clock.restart();

                // UPDATE THE GAME'S CURRENT STATE.
                STATES::GameState next_game_state = game_state;
                switch (game_state)
                {
                    case STATES::GameState::INTRO_SEQUENCE:
                    {
                        // UPDATE THE INTRO SEQUENCE.
                        intro_sequence.Update(elapsed_time);

                        // MOVE TO THE TITLE SCREEN IF THE INTRO SEQUENCE HAS FINISHED.
                        bool intro_sequence_finished = intro_sequence.Completed();
                        if (intro_sequence_finished)
                        {
                            // The intro music isn't stopped before going to the next state
                            // to avoid a hard cutoff.  It is timed such that it should end
                            // shortly.
                            next_game_state = STATES::GameState::TITLE_SCREEN;
                        }
                        break;
                    }
                    case STATES::GameState::TITLE_SCREEN:
                        next_game_state = title_screen.RespondToInput(input_controller);
                        break;
                    case STATES::GameState::CREDITS_SCREEN:
                        next_game_state = credits_screen.Update(elapsed_time, input_controller);
                        break;
                    case STATES::GameState::PRE_FLOOD_GAMEPLAY:
                        pre_flood_gameplay_state.Update(elapsed_time, input_controller, renderer.Camera);
                }

                // CLEAR THE SCREEN OF THE PREVIOUSLY RENDERED FRAME.
                renderer.Screen.Clear();

                // RENDER THE CURRENT STATE OF THE GAME.
                switch (game_state)
                {
                    case STATES::GameState::INTRO_SEQUENCE:
                        intro_sequence.Render(renderer);
                        break;
                    case STATES::GameState::TITLE_SCREEN:
                        title_screen.Render(renderer);
                        break;
                    case STATES::GameState::CREDITS_SCREEN:
                        credits_screen.Render(renderer);
                        break;
                    case STATES::GameState::PRE_FLOOD_GAMEPLAY:
                        pre_flood_gameplay_state.Render(renderer);
                        break;
                }

                // DISPLAY THE RENDERED FRAME IN THE WINDOW.
                window->display();

                // PERFORM ADDITIONAL STEPS NEEDED TO TRANSITION TO CERTAIN NEW GAME STATES.
                bool game_state_changed = (next_game_state != game_state);
                if (game_state_changed)
                {
                    // CHANGE THE GAME'S STATE.
                    game_state = next_game_state;
                    switch (game_state)
                    {
                        case STATES::GameState::CREDITS_SCREEN:
                            // RESET THE ELAPSED TIME FOR THE CREDITS SCREEN.
                            credits_screen.ElapsedTime = sf::Time::Zero;
                            break;
                        case STATES::GameState::PRE_FLOOD_GAMEPLAY:
                        {
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

                            bool gameplay_state_initialized = pre_flood_gameplay_state.Initialize(
                                SCREEN_WIDTH_IN_PIXELS,
                                *saved_game_data,
                                overworld);
                            assert(gameplay_state_initialized);

                            // FOCUS THE CAMERA ON THE PLAYER.
                            MATH::Vector2f player_start_world_position = pre_flood_gameplay_state.NoahPlayer->GetWorldPosition();
                            renderer.Camera.SetCenter(player_start_world_position);

                            break;
                        }
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
