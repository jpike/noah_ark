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
#include "Graphics/Rendering.h"
#include "Graphics/Screen.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Gui/TextBox.h"
#include "Input/KeyboardInputController.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"

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

void InitializePlayer(const MATH::Vector2f& initial_world_position, RESOURCES::Assets& assets, OBJECTS::Noah& noah_player)
{
    // LOAD THE ANIMATION SEQUENCES.
    std::shared_ptr<GRAPHICS::AnimationSequence> walk_front_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);
    bool walk_front_animation_loaded = (nullptr != walk_front_animation);
    if (!walk_front_animation_loaded)
    {
        assert(walk_front_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_back_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID);
    bool walk_back_animation_loaded = (nullptr != walk_back_animation);
    if (!walk_back_animation_loaded)
    {
        assert(walk_back_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_left_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID);
    bool walk_left_animation_loaded = (nullptr != walk_left_animation);
    if (!walk_left_animation_loaded)
    {
        assert(walk_left_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_right_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID);
    bool walk_right_animation_loaded = (nullptr != walk_right_animation);
    if (!walk_right_animation_loaded)
    {
        assert(walk_right_animation_loaded);
        return;
    }

    // GET THE TEXTURE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> texture = assets.GetTexture(RESOURCES::NOAH_TEXTURE_ID);
    bool texture_loaded = (nullptr != texture);
    if (!texture_loaded)
    {
        assert(texture_loaded);
        return;
    }

    // CREATE THE SPRITE FOR NOAH.
    /// @todo   Better way to determine initial subrect.
    const MATH::FloatRectangle TEXTURE_SUB_RECT = MATH::FloatRectangle::FromTopLeftAndDimensions(0, 0, 16, 16);
    GRAPHICS::Sprite sprite(texture, TEXTURE_SUB_RECT);
    /// @todo better way to set center.
    sprite.SetOrigin(MATH::Vector2f(8.0f, 8.0f));
    GRAPHICS::AnimatedSprite animated_sprite(sprite);

    // SET ANIMATION SEQUENCES.
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID, walk_front_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID, walk_back_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID, walk_left_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID, walk_right_animation);

    // Set the initial animation sequence to have the player facing forward (downward).
    animated_sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);

    // SET NOAH'S INITIAL POSITION.
    animated_sprite.SetWorldPosition(initial_world_position.X, initial_world_position.Y);

    // INITIALIZE NOAH.
    noah_player.Sprite = animated_sprite;
    noah_player.FacingDirection = CORE::Direction::DOWN;

    // LOAD THE AXE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> axe_texture = assets.GetTexture(RESOURCES::AXE_TEXTURE_ID);
    bool axe_resources_loaded = (nullptr != axe_texture);
    if (!axe_resources_loaded)
    {
        // The axe cannot be created without its resources.
        assert(axe_resources_loaded);
        return;
    }

    const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
    const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
    const float AXE_WIDTH_IN_PIXELS = 11.0f;
    const float AXE_HEIGHT_IN_PIXELS = 14.0f;
    MATH::FloatRectangle axe_texture_sub_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
        AXE_SPRITE_X_OFFSET_IN_PIXELS,
        AXE_SPRITE_Y_OFFSET_IN_PIXELS,
        AXE_WIDTH_IN_PIXELS,
        AXE_HEIGHT_IN_PIXELS);
    noah_player.Inventory->Axe->Sprite = GRAPHICS::Sprite(axe_texture, axe_texture_sub_rectangle);

    noah_player.Inventory->Axe->AxeHitSound = assets.GetSound(RESOURCES::AXE_HIT_SOUND_ID);
}

/// @todo   Document this function.  It looks like it can probably stay in this file for now.
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
            MAPS::TileMap tile_map(
                row,
                column,
                map_center_world_position,
                map_dimensions_in_tiles,
                overworld_map_file.TileDimensionInPixels);

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
                                    MATH::FloatRectangle SMALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(0.0f, 0.0f, 16.0f, 16.0f);
                                    tree_texture_sub_rectangle = SMALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (TALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    MATH::FloatRectangle TALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(32.0f, 0.0f, 16.0f, 32.0f);
                                    tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (LARGE_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    MATH::FloatRectangle LARGE_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(0.0f, 16.0f, 32.0f, 32.0f);
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

    // INITIALIZE THE PLAYER NOAH CHARACTER.
    /// @todo   Need to figure out a different way to determine initial position.
    const MAPS::TileMap& starting_tile_map = overworld.TileMaps(0, 0);
    MATH::Vector2f noah_initial_position = starting_tile_map.GetCenterWorldPosition();
    InitializePlayer(noah_initial_position, assets, overworld.NoahPlayer);
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
        std::chrono::time_point<std::chrono::system_clock> asset_load_start_time = std::chrono::system_clock::now();
        bool assets_loaded = assets.LoadAll();
        std::chrono::time_point<std::chrono::system_clock> asset_load_end_time = std::chrono::system_clock::now();
        std::chrono::duration<double> asset_load_time_in_seconds = asset_load_end_time - asset_load_start_time;
        std::cout << "Asset load time (seconds): " << asset_load_time_in_seconds.count() << std::endl;
        if (!assets_loaded)
        {
            /// @todo   Provide a more specific error from Assets class for loading failures?
            std::cerr << "Failed to load assets." << std::endl;
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        // INITIALIZE THE BIBLE VERSES LEFT TO BE FOUND.
        std::vector<BIBLE::BibleVerse> bible_verses_left_to_find = BIBLE::BIBLE_VERSES;

        // CREATE THE OVERWORLD.
        const std::unique_ptr<MAPS::OverworldMapFile>& overworld_map_file = assets.OverworldMapFile;
        MAPS::Overworld overworld(
            overworld_map_file->OverworldWidthInTileMaps, 
            overworld_map_file->OverworldHeightInTileMaps,
            overworld_map_file->TileMapWidthInTiles,
            overworld_map_file->TileMapHeightInTiles,
            overworld_map_file->TileDimensionInPixels);
        PopulateOverworld(*overworld_map_file, assets, overworld);

        // CREATE THE CONTROLLER USING THE KEYBOARD.
        INPUT_CONTROL::KeyboardInputController input_controller;

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
        background_music.play();

        // CREATE THE WINDOW.
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(GRAPHICS::Screen::WIDTH_IN_PIXELS, GRAPHICS::Screen::HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // Ensure that only one key event is generated for each key press.
        window->setKeyRepeatEnabled(false);

        // CREATE THE MAIN TEXT BOX FOR DISPLAYING MESSAGES TO THE PLAYER.
        std::shared_ptr<GRAPHICS::GUI::Font> font = assets.GetFont(RESOURCES::FONT_TEXTURE_ID);
        bool font_loaded = (nullptr != font);
        if (!font_loaded)
        {
            std::cerr << "Failed to load font." << std::endl;
            return EXIT_CODE_FAILURE_LOADING_FONT;
        }

        // INITIALIZE THE HUD.
        /// @todo   Re-examine how we get resources to display in the HUD.
        GRAPHICS::GUI::HeadsUpDisplay hud(
            overworld.NoahPlayer.Inventory,
            font,
            assets.GetTexture(RESOURCES::AXE_TEXTURE_ID),
            assets.GetTexture(RESOURCES::WOOD_LOG_TEXTURE_ID));

        // INITIALIZE THE CAMERA.
        GRAPHICS::Camera camera(window);
        MATH::Vector2f player_start_world_position = overworld.NoahPlayer.GetWorldPosition();
        camera.SetCenter(player_start_world_position);

        // CREATE THE RANDOM NUMBER GENERATOR.
        std::random_device random_number_generator;

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
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
                    case sf::Event::KeyPressed:
                    {
                        // CHECK IF THE KEY FOR THE INVENTORY WAS PRESSED.
                        // This is done here as a simple way to easily check if a key has
                        // been pressed and released (as opposed to whether it is down
                        // or not).  This prevents a scenario where the inventory could
                        // repeatedly open or close if the key was held down.
                        /// @todo   I'm not sure I like separating these key presses
                        /// from the rest of the keyboard controls.
                        bool inventory_key_pressed = (sf::Keyboard::X == event.key.code);
                        if (inventory_key_pressed)
                        {
                            // OPEN OR CLOSE THE INVENTORY.
                            // The inventory key acts like a toggle button for the inventory.
                            hud.InventoryOpened = !hud.InventoryOpened;
                            break;
                        }

                        // CHECK IF THE INVENTORY IS OPENED.
                        if (hud.InventoryOpened)
                        {
                            // FORWARD THE KEY PRESS TO THE HUD.
                            hud.RespondToInput(event.key.code);
                        }
                        break;
                    }
                    case sf::Event::Closed:
                        window->close();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window->isOpen())
            {
                /// @todo   Keep in mind possible ways to factor out the bulk of this logic
                /// to a separate function (and hopefully make this main loop easier to understand).
                /// Right now, most of this code is interconnected enough where factoring it out
                /// wouldn't be that feasible (and could make things harder to understand or
                /// change later).

                // UPDATE THE GAME FOR THE NEW FRAME.
                sf::Time elapsed_time = game_loop_clock.restart();
                float elapsed_time_in_seconds = elapsed_time.asSeconds();

                MATH::FloatRectangle camera_bounds = camera.GetViewBounds();
                MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

                MAPS::TileMap* current_tile_map = overworld.GetTileMap(camera_view_center.X, camera_view_center.Y);
                assert(current_tile_map);

                // CHECK IF THE INVENTORY GUI IS DISPLAYED.
                // If the inventory GUI is displayed, then the regular controls for the player
                // in the world shouldn't work.
                if (!hud.InventoryOpened)
                {
                    // UPDATE THE TEXT BOX IF IT IS VISIBLE.
                    // If the text box is currently being displayed, then it should capture any user input.
                    if (hud.MainTextBox.IsVisible)
                    {
                        /// @todo   Maybe this should be moved to after the MoveToNextPage() below
                        /// to prevent the last character from not being seen?
                        hud.MainTextBox.Update(elapsed_time_in_seconds);

                        // HAVE THE MAIN TEXT BOX RESPOND TO USER INPUT.
                        if (input_controller.PrimaryActionButtonPressed())
                        {
                            // CHECK IF THE TEXT BOX IS FINISHED DISPLAYING ITS CURRENT PAGE OF TEXT.
                            // If the current page of text has not yet all been displayed, the next
                            // page of text should not be moved to so that the user can finish
                            // seeing the complete message.
                            bool current_text_finished_being_displayed = hud.MainTextBox.CurrentPageOfTextFinishedBeingDisplayed();
                            if (current_text_finished_being_displayed)
                            {
                                // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
                                hud.MainTextBox.MoveToNextPage();
                            }
                        }
                    }
                    else
                    {
                        // UPDATE THE OVERWORLD.
                        std::string message_for_text_box;
                        overworld.Update(
                            elapsed_time_in_seconds,
                            random_number_generator,
                            input_controller,
                            bible_verses_left_to_find,
                            assets,
                            camera,
                            message_for_text_box);

                        // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
                        bool text_box_message_exists = !message_for_text_box.empty();
                        if (text_box_message_exists)
                        {
                            hud.MainTextBox.StartDisplayingText(message_for_text_box);
                        }
                    }
                }

                // RENDER THE CURRENT STATE OF THE GAME.
                window->clear();

                // RENDER THE CURRENT TILE MAP.
                /// @todo   Consider signed ints.  Right now, we're just
                /// rendering a few surrounding tile maps.
                unsigned int min_tile_map_row = current_tile_map->OverworldRowIndex;
                bool top_tile_map_exists = (min_tile_map_row > 0);
                if (top_tile_map_exists)
                {
                    --min_tile_map_row;
                }
                unsigned int min_tile_map_column = current_tile_map->OverworldColumnIndex;
                bool left_tile_map_exists = (min_tile_map_column > 0);
                if (left_tile_map_exists)
                {
                    --min_tile_map_column;
                }
                unsigned int max_tile_map_row = min_tile_map_row + 2;
                unsigned int max_tile_map_column = min_tile_map_column + 2;
                for (unsigned int tile_map_row = min_tile_map_row; tile_map_row <= max_tile_map_row; ++tile_map_row)
                {
                    for (unsigned int tile_map_column = min_tile_map_column; tile_map_column <= max_tile_map_column; ++tile_map_column)
                    {
                        // GET THE CURRENT TILE MAP.
                        MAPS::TileMap* tile_map = overworld.GetTileMap(tile_map_row, tile_map_column);
                        bool tile_map_exists = (nullptr != tile_map);
                        if (!tile_map_exists)
                        {
                            // Continue trying to render other tile maps.
                            continue;
                        }

                        // UPDATE THE CURRENT TILE MAP'S TREES.
                        /// @todo   Figure out if this maybe should be handled elsewhere.
                        for (auto tree = tile_map->Trees.begin(); tree != tile_map->Trees.end(); ++tree)
                        {
                            tree->Update(elapsed_time_in_seconds);
                        }

                        // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
                        /// @todo   Maybe we only need a single dust cloud?  At least in theory, it shouldn't be
                        /// possible for more than one tree to in this state at a given time.
                        for (auto dust_cloud = tile_map->TreeDustClouds.begin(); dust_cloud != tile_map->TreeDustClouds.end();)
                        {
                            // UPDATE THE CURRENT DUST CLOUD.
                            dust_cloud->Update(elapsed_time_in_seconds);

                            // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
                            bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
                            if (dust_cloud_disappeared)
                            {
                                // REMOVE THE DUST CLOUD.
                                dust_cloud = tile_map->TreeDustClouds.erase(dust_cloud);
                            }
                            else
                            {
                                // MOVE TO UPDATING THE NEXT DUST CLOUD.
                                ++dust_cloud;
                            }
                        }

                        // RENDER THE TILE MAP.
                        GRAPHICS::Render(*tile_map, *window);
                    }
                }
                
                // RENDER THE PLAYER.
                // Make sure his axe/sprite are updated.
                overworld.NoahPlayer.Inventory->Axe->Update(elapsed_time_in_seconds);
                overworld.NoahPlayer.Sprite.Update(elapsed_time_in_seconds);
                GRAPHICS::Render(overworld.NoahPlayer.Sprite, *window);

                // The axe should only be rendered if it is swinging.
                if (overworld.NoahPlayer.Inventory->Axe->IsSwinging())
                {
                    GRAPHICS::Render(overworld.NoahPlayer.Inventory->Axe->Sprite, *window);
                }

                // RENDER THE HUD.
                hud.Render(*window);

                // RENDER THE TEXT BOX IF IT IS VISIBLE.
                if (hud.MainTextBox.IsVisible)
                {
                    hud.MainTextBox.Render(*window);
                }

                // DISPLAY THE RENDERED FRAME IN THE WINDOW.
                window->display();
            } // end if (window->isOpen())
        } // end while (window->isOpen())

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
