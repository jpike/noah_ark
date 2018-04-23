#include <chrono>
#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Input/InputController.h"
#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Math/Number.h"
#include "Resources/AnimalSounds.h"
#include "Resources/AssetPackage.h"
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
    MAPS::MultiTileMapGrid& overworld)
{
    // LOAD THE TILESET TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::AssetId::MAIN_TILESET_TEXTURE);
    if (!tileset_texture)
    {
        return;
    }

    // LOAD THE TREE TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::AssetId::TREE_TEXTURE);
    if (!tree_texture)
    {
        return;
    }

    // CREATE THE TILESET.
    MAPS::Tileset tileset(tileset_texture);

    // CREATE A RANDOM NUMBER GENERATOR FOR CREATING FOOD.
    MATH::RandomNumberGenerator random_number_generator;

    // LOAD TILE MAPS FOR EACH ROW.
    for (unsigned int row = 0; row < MAPS::World::OVERWORLD_HEIGHT_IN_TILE_MAPS; ++row)
    {
        // LOAD TILE MAPS FOR EACH COLUMN.
        for (unsigned int column = 0; column < MAPS::World::OVERWORLD_WIDTH_IN_TILE_MAPS; ++column)
        {
            // GET THE CURRENT TILE MAP FILE.
            const auto& tile_map_data = MAPS::DATA::OVERWORLD_MAP_DATA(column, row);

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
                std::shared_ptr<GRAPHICS::Texture> ark_texture = assets.GetTexture(RESOURCES::AssetId::ARK_TEXTURE);
                if (ark_texture)
                {
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

/// Populates the ark interior based on data read from in-memory assets.
/// @param[in,out]  assets - The assets for the overworld.
/// @param[in,out]  ark_interior - The ark interior to populate.
void PopulateArkInterior(RESOURCES::Assets& assets, MAPS::MultiTileMapGrid& ark_interior)
{
    // LOAD THE TILESET TEXTURE.
    std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::AssetId::MAIN_TILESET_TEXTURE);
    if (!tileset_texture)
    {
        return;
    }

    // CREATE THE TILESET.
    MAPS::Tileset tileset(tileset_texture);

    // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
    MATH::Vector2f map_center_world_position;

    /// @todo   Add an appropriate loop to handle a larger ark interior, rather than hard-coding column/row.
    unsigned int column = 0;
    unsigned int row = 0;
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

    const auto& tile_map_data = MAPS::DATA::ARK_INTERIOR_TILE_MAP_0_0;

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
            MAPS::TileId tile_id = tile_map_data(current_tile_x, current_tile_y);
            std::shared_ptr<MAPS::Tile> tile = tileset.CreateTile(tile_id);
            bool tile_exists_in_tileset = (nullptr != tile);
            if (!tile_exists_in_tileset)
            {
                // Skip to trying to create the next tile.  The layer
                // simply won't have any tile at this location.
                continue;
            }

            // SET THE TILE IN THE GROUND LAYER.
            tile_map.Ground.SetTile(current_tile_x, current_tile_y, tile);
        }
    }

    // STORE THE TILE MAP OF THE ARK INTERIOR.
    ark_interior.TileMaps(column, row) = std::move(tile_map);
}

/// Loads the world.
/// @param[in,out]  assets - The assets to use for the world.
/// @return The world, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::World> LoadWorld(RESOURCES::Assets& assets)
{
    // CREATE THE EMPTY WORLD.
    auto load_start_time = std::chrono::system_clock::now();
    std::shared_ptr<MAPS::World> world = std::make_shared<MAPS::World>();

    // POPULATE THE WORLD.
    PopulateOverworld(assets, world->Overworld);
    PopulateArkInterior(assets, world->ArkInterior);

    // CREATE THE EXIT POINTS BETWEEN MAP GRIDS.
    // The ark interior data used for exit points is currently hard-coded to simplify
    // things since I'm not sure yet exactly how we want to structure this.
    MAPS::TileMap* starting_ark_interior_tile_map = &world->ArkInterior.TileMaps(0, 0);
    // For now, this is hardcoded to an arbitrary place near the bottom of the starting tile map.
    MATH::FloatRectangle ark_interior_bounding_box = starting_ark_interior_tile_map->GetWorldBoundingBox();
    float ark_interior_center_x_position = ark_interior_bounding_box.GetCenterXPosition();
    float ark_interior_bottom_y_position = ark_interior_bounding_box.GetBottomYPosition();
    MATH::Vector2f ark_interior_bottom_center_position(ark_interior_center_x_position, ark_interior_bottom_y_position);
    MATH::Vector2f ark_interior_player_start_position = ark_interior_bottom_center_position;
    ark_interior_player_start_position.Y -= MAPS::Tile::DIMENSION_IN_PIXELS<float> * 3.0f;

    MATH::FloatRectangle ark_interior_exit_point_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
        ark_interior_player_start_position.X,
        ark_interior_player_start_position.Y + MAPS::Tile::DIMENSION_IN_PIXELS<float>,
        MAPS::Tile::DIMENSION_IN_PIXELS<float> * 4.0f,
        MAPS::Tile::DIMENSION_IN_PIXELS<float> * 2.0f);

    // Note that this is intentionally done in a a not-very-efficient way right now.
    // While we could theoretically create the initial exit points when creating
    // the different map grids, that's not currently being done now due to risk in
    // having incomplete exit points and the fact that I'm not sure yet exactly
    // what form we'll want these exit points in.
    unsigned int overworld_height_in_tile_maps = world->Overworld.TileMaps.GetHeight();
    unsigned int overworld_width_in_tile_maps = world->Overworld.TileMaps.GetWidth();
    for (unsigned int tile_map_row = 0; tile_map_row < overworld_height_in_tile_maps; ++tile_map_row)
    {
        for (unsigned int tile_map_column = 0; tile_map_column < overworld_width_in_tile_maps; ++tile_map_column)
        {
            // SEARCH THE CURRENT TILE MAP FOR ANY ARK PIECE EXISTS.
            MAPS::TileMap& overworld_tile_map = world->Overworld.TileMaps(tile_map_column, tile_map_row);
            for (const auto& ark_piece : overworld_tile_map.ArkPieces)
            {
                // CREATE EXIT POINTS FOR ANY DOORWAYS.
                if (ark_piece.IsExternalDoorway)
                {
                    // CREATE AN EXIT POINT FROM THE OVERWORLD INTO THE ARK.
                    MAPS::ExitPoint overworld_to_ark_exit_point;
                    MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
                    overworld_to_ark_exit_point.BoundingBox = ark_piece_bounding_box;
                    overworld_to_ark_exit_point.NewMapGrid = &world->ArkInterior;
                    overworld_to_ark_exit_point.NewTileMap = starting_ark_interior_tile_map;
                    overworld_to_ark_exit_point.NewPlayerWorldPosition = ark_interior_player_start_position;

                    overworld_tile_map.ExitPoints.push_back(overworld_to_ark_exit_point);

                    // CREATE AN EXIT POINT FROM THE ARK INTERIOR TO THE OVERWORLD.
                    MAPS::ExitPoint ark_to_overworld_exit_point;
                    ark_to_overworld_exit_point.BoundingBox = ark_interior_exit_point_bounding_box;
                    ark_to_overworld_exit_point.NewMapGrid = &world->Overworld;
                    ark_to_overworld_exit_point.NewTileMap = &overworld_tile_map;
                    MATH::Vector2f overworld_exit_starting_position = ark_piece_bounding_box.GetCenterPosition();
                    overworld_exit_starting_position.Y = ark_piece_bounding_box.GetBottomYPosition();
                    ark_to_overworld_exit_point.NewPlayerWorldPosition = overworld_exit_starting_position;

                    starting_ark_interior_tile_map->ExitPoints.push_back(ark_to_overworld_exit_point);
                }
            }
        }
    }

    auto load_end_time = std::chrono::system_clock::now();

    // Overworld load time: 82,263,372 (file-based)
    // Overworld load time: 17,972,479 (array-based)
    // World load time (with ark interior): 31,132,753
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("World load time: ", load_time_diff.count());

    return world;
}

/// Loads the sounds/music for the game, adding them to the speakers.
/// @todo   Rethink this function since it doesn't load the intro music.
/// @param[in,out]  assets - The assets from which to load the sounds/music.
/// @param[in,out]  speakers - The speakers to add sounds to.
void LoadSounds(RESOURCES::Assets& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();

    // ADD EACH GENERIC SOUND EFFECT TO THE SPEAKERS.
    const std::array<RESOURCES::AssetId, 5> GENERIC_SOUND_EFFECT_IDS =
    {
        RESOURCES::AssetId::ARK_BUILDING_SOUND,
        RESOURCES::AssetId::AXE_HIT_SOUND,
        RESOURCES::AssetId::COLLECT_BIBLE_VERSE_SOUND,
        RESOURCES::AssetId::FOOD_PICKUP_SOUND,
        RESOURCES::AssetId::TREE_SHAKE_SOUND

    };
    for (const RESOURCES::AssetId sound_id : GENERIC_SOUND_EFFECT_IDS)
    {
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(sound_id);
        if (audio_samples)
        {
            speakers.AddSound(sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE ANIMAL SOUND EFFECTS TO THE SPEAKERS.
    for (int species_id = 0; species_id < static_cast<int>(OBJECTS::AnimalSpecies::COUNT); ++species_id)
    {
        OBJECTS::AnimalSpecies species = static_cast<OBJECTS::AnimalSpecies>(species_id);
        RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(species);
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(animal_sound_id);
        if (audio_samples)
        {
            speakers.AddSound(animal_sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE MUSIC TO THE SPEAKERS.
    std::shared_ptr<sf::Music> overworld_music = assets.GetMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
    if (overworld_music)
    {
        overworld_music->setLoop(true);
        speakers.AddMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC, overworld_music);
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Sound load time: ", load_time_diff.count());
}

bool LoadIntroSequenceAssets(RESOURCES::Assets& assets)
{
    auto load_start_time = std::chrono::system_clock::now();

    // DEFINE THE INTRO SEQUENCE ASSETS.
    const RESOURCES::AssetPackage INTRO_SEQUENCE_ASSETS(
    {
        RESOURCES::AssetDefinition(RESOURCES::AssetType::FONT, RESOURCES::AssetId::FONT_TEXTURE),
        RESOURCES::AssetDefinition(RESOURCES::AssetType::MUSIC, RESOURCES::AssetId::INTRO_MUSIC),
        RESOURCES::AssetDefinition(RESOURCES::AssetType::SHADER, RESOURCES::AssetId::COLORED_TEXTURE_SHADER),
    });

    // LOAD THE ASSETS.
    bool assets_loaded = assets.Load(INTRO_SEQUENCE_ASSETS);

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Intro sequence load time: ", load_time_diff.count());

    return assets_loaded;
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

        // LOAD ASSETS NEEDED FOR THE INTRO SEQUENCE.
        // Some assets are explicitly loaded first as they are needed for initial
        // rendering of the intro sequence and title screen.  By loading them first,
        // quick startup time for the game can be maintained, showing the intro
        // sequence while other assets are being loaded.
        std::shared_ptr<RESOURCES::Assets> assets = std::make_shared<RESOURCES::Assets>();
        bool intro_sequence_assets_loaded = LoadIntroSequenceAssets(*assets);
        if (!intro_sequence_assets_loaded)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Failed to load intro sequence assets.");
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        auto font = assets->GetFont(RESOURCES::AssetId::FONT_TEXTURE);
        if (!font)
        {
            return EXIT_CODE_FAILURE_LOADING_FONT;
        }

        auto colored_texture_shader = assets->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
        if (!colored_texture_shader)
        {
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        std::shared_ptr<AUDIO::Speakers> speakers = std::make_shared<AUDIO::Speakers>();
        auto intro_music = assets->GetMusic(RESOURCES::AssetId::INTRO_MUSIC);
        if (!intro_music)
        {
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }
        speakers->AddMusic(RESOURCES::AssetId::INTRO_MUSIC, intro_music);

        // LOAD REMAINING SOUND EFFECTS/MUSIC.
        // This is loaded in a background thread in order to let the rest of the game continue
        // while assets not needed for the intro sequence are loaded.
        std::future<void> audio_being_loaded = std::async(LoadSounds, std::ref(*assets), std::ref(*speakers));
        /// @todo   When to wait for this to finish?

        // LOAD THE MAPS.
        // The world is loaded in the background in separate threads to avoid having
        // map loading slow the startup time of the rest of the game.
        std::future< std::shared_ptr<MAPS::World> > world_being_loaded = std::async(LoadWorld, std::ref(*assets));

        // INITIALIZE REMAINING SUBSYSTEMS.
        GRAPHICS::Renderer renderer(
            SCREEN_WIDTH_IN_PIXELS,
            SCREEN_HEIGHT_IN_PIXELS,
            font,
            colored_texture_shader);
        INPUT_CONTROL::InputController input_controller;
        STATES::IntroSequence intro_sequence;
        speakers->PlayMusic(RESOURCES::AssetId::INTRO_MUSIC);
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
                // HANDLE THE CURRENT EVENT BASED ON ITS TYPE.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        // CLOSE THE WINDOW.
                        // The game will end later.
                        window->close();
                        break;
                    case sf::Event::LostFocus:
                        // DISABLE INPUT.
                        // This will prevent the user from accidentally moving
                        // the player until the window regains focus.
                        DEBUGGING::DebugConsole::WriteLine("Disabling input.");
                        input_controller.DisableInput();
                        break;
                    case sf::Event::GainedFocus:
                        // ENABLE INPUT.
                        // This will allow the user to resume moving the player
                        // now that the window has focus.
                        DEBUGGING::DebugConsole::WriteLine("Enabling input.");
                        input_controller.EnableInput();
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
                renderer.Screen.RenderTarget.display();
                sf::Sprite screen(renderer.Screen.RenderTarget.getTexture());
                window->draw(screen);
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
                            auto world = world_being_loaded.get();
                            if (!world)
                            {
                                return EXIT_FAILURE;
                            }

                            bool gameplay_state_initialized = pre_flood_gameplay_state.Initialize(
                                SCREEN_WIDTH_IN_PIXELS,
                                *saved_game_data,
                                world);
                            if (!gameplay_state_initialized)
                            {
                                return EXIT_FAILURE;
                            }

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
        DEBUGGING::DebugConsole::WriteErrorLine("Standard exception in main: " + std::string(exception.what()));
        return EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN;
    }
    catch (...)
    {   
        DEBUGGING::DebugConsole::WriteErrorLine("Unknown exception in main.");
        return EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN;
    }
}
