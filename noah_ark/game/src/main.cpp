#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Bible/BibleVerses.h"
#include "Collision/Collisions.h"
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Graphics/Camera.h"
#include "Graphics/Rendering.h"
#include "Input/KeyboardInputController.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"
#include "Resources/AudioClips.h"

/// The game exited successfully.
int EXIT_CODE_SUCCESS = 0;
/// A standard exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN = 1;
/// An unknown exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN = 2;
/// Game assets failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_ASSETS = 3;

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
                                std::shared_ptr<AUDIO::SoundEffect> tree_shake_sound = assets.GetSound(&RESOURCES::TREE_SHAKE_AUDIO_CLIP);
                                bool tree_shake_sound_retrieved = (nullptr != tree_shake_sound);
                                if (!tree_shake_sound_retrieved)
                                {
                                    /// @todo   Allow creating tree with no sound effect?
                                    assert(false);
                                    continue;
                                }

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
    noah_player.Axe->Sprite = GRAPHICS::Sprite(axe_texture, axe_texture_sub_rectangle);

    noah_player.Axe->AxeHitSound = assets.GetSound(&RESOURCES::AXE_HIT_AUDIO_CLIP);
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

        // INITIALIZE THE PLAYER NOAH CHARACTER.
        /// @todo   Need to figure out a different way to determine initial position.
        const MAPS::TileMap& starting_tile_map = overworld.TileMaps(0, 0);
        MATH::Vector2f noah_initial_position = starting_tile_map.GetCenterWorldPosition();
        OBJECTS::Noah noah_player;
        InitializePlayer(noah_initial_position, assets, noah_player);

        // CREATE THE CONTROLLER USING THE KEYBOARD.
        INPUT_CONTROL::KeyboardInputController input_controller;

        // CREATE THE WINDOW.
        // The width and height are currently set to match the dimensions
        // of a single tile map in the game.
        const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
        const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // INITIALIZE THE CAMERA.
        GRAPHICS::Camera camera(window);
        MATH::Vector2f center_world_position = starting_tile_map.GetCenterWorldPosition();
        camera.SetCenter(center_world_position);

        // CREATE A COLLECTION OF AXE SWING EVENTS.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > axe_swings;

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

                // SWING THE PLAYER'S AXE IF THE APPROPRIATE BUTTON WAS PRESSED.
                if (input_controller.PrimaryActionButtonPressed())
                {
                    // A new axe swing may not be created if the player's
                    // axe is already being swung.
                    std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = noah_player.SwingAxe();
                    bool axe_swing_occurred = (nullptr != axe_swing);
                    if (axe_swing_occurred)
                    {
                        // Allow the axe to collide with other objects.
                        axe_swings.push_back(axe_swing);
                    }
                }

                // CHECK IF THE AXE IS SWINGING.
                // Noah can't move while the axe is swinging.
                // Movement is prevented to have the axe's position
                // remain attached to Noah.
                // We need to keep track if Noah moved this frame
                // so that we can stop any walking animations for
                // him if he didn't move.
                bool noah_moved_this_frame = false;
                bool axe_is_swinging = (nullptr != noah_player.Axe) && noah_player.Axe->IsSwinging();
                if (!axe_is_swinging)
                {
                    // MOVE NOAH IN RESPONSE TO USER INPUT.
                    const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
                    MATH::Vector2f old_noah_position = noah_player.GetWorldPosition();
                    if (input_controller.UpButtonPressed())
                    {
                        // TRACK NOAH AS MOVING THIS FRAME.
                        noah_moved_this_frame = true;

                        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                        noah_player.FacingDirection = CORE::Direction::UP;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        // MOVE NOAH WHILE HANDLING COLLISIONS.
                        MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::UP,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_top_y_position = camera_bounds.GetTopYPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetTopYPosition() < camera_top_y_position);
                        if (player_moved_out_of_view)
                        {
                            // CHECK IF A TOP TILE MAP EXISTS FOR NOAH TO MOVE TO.
                            unsigned int top_tile_map_row_index = current_tile_map->OverworldRowIndex - 1;
                            unsigned int top_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                            MAPS::TileMap* top_tile_map = overworld.GetTileMap(
                                top_tile_map_row_index,
                                top_tile_map_column_index);
                            bool top_tile_map_exists = (nullptr != top_tile_map);
                            if (top_tile_map_exists)
                            {
                                // MOVE NOAH A FEW MORE PIXELS UP SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                                MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                                noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                // START SCROLLING TO THE TOP TILE MAP.
                                MATH::Vector2f scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                MATH::Vector2f scroll_end_position = top_tile_map->GetCenterWorldPosition();
                                camera.StartScrolling(scroll_start_position, scroll_end_position);

                                // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                                input_controller.DisableInput();
                            }
                            else
                            {
                                // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                                // Since there is no top tile map to scroll to, this will keep Noah on-screen.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_top_boundary = tile_map_bounding_box.GetTopYPosition();

                                // To keep Noah completely on screen, his center position should be half
                                // his height below the top tile map boundary.
                                MATH::Vector2f noah_world_position = old_noah_position;
                                float noah_half_height = noah_world_bounding_box.GetHeight() / 2.0f;
                                noah_world_position.Y = tile_map_top_boundary + noah_half_height;

                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        } // end if (player_moved_out_of_view)
                    } // end if (input_controller.UpButtonPressed())
                    if (input_controller.DownButtonPressed())
                    {
                        // TRACK NOAH AS MOVING THIS FRAME.
                        noah_moved_this_frame = true;

                        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                        noah_player.FacingDirection = CORE::Direction::DOWN;

                        // PLAY THE WALKING DOWN ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        // MOVE NOAH WHILE HANDLING COLLISIONS.
                        MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::DOWN,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_bottom_y_position = camera_bounds.GetBottomYPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetBottomYPosition() > camera_bottom_y_position);
                        if (player_moved_out_of_view)
                        {
                            // CHECK IF A BOTTOM TILE MAP EXISTS FOR NOAH TO MOVE TO.
                            unsigned int bottom_tile_map_row_index = current_tile_map->OverworldRowIndex + 1;
                            unsigned int bottom_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                            MAPS::TileMap* bottom_tile_map = overworld.GetTileMap(
                                bottom_tile_map_row_index,
                                bottom_tile_map_column_index);
                            bool bottom_tile_map_exists = (nullptr != bottom_tile_map);
                            if (bottom_tile_map_exists)
                            {
                                // MOVE NOAH A FEW MORE PIXELS DOWN SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                                MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                                noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                // START SCROLLING TO THE BOTTOM TILE MAP.
                                MATH::Vector2f scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                MATH::Vector2f scroll_end_position = bottom_tile_map->GetCenterWorldPosition();
                                camera.StartScrolling(scroll_start_position, scroll_end_position);

                                // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                                input_controller.DisableInput();
                            }
                            else
                            {
                                // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                                // Since there is no bottom tile map to scroll to, this will keep Noah on-screen.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_bottom_boundary = tile_map_bounding_box.GetBottomYPosition();

                                // To keep Noah completely on screen, his center position should be half
                                // his height above the bottom tile map boundary.
                                MATH::Vector2f noah_world_position = old_noah_position;
                                float noah_half_height = noah_player.GetWorldBoundingBox().GetHeight() / 2.0f;
                                noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;

                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        } // end if (player_moved_out_of_view)
                    } // end if (input_controller.DownButtonPressed())
                    if (input_controller.LeftButtonPressed())
                    {
                        // TRACK NOAH AS MOVING THIS FRAME.
                        noah_moved_this_frame = true;

                        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                        noah_player.FacingDirection = CORE::Direction::LEFT;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        // MOVE NOAH WHILE HANDLING COLLISIONS.
                        MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::LEFT,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_left_x_position = camera_bounds.GetLeftXPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetLeftXPosition() < camera_left_x_position);
                        if (player_moved_out_of_view)
                        {
                            // CHECK IF A LEFT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                            unsigned int left_tile_map_row_index = current_tile_map->OverworldRowIndex;
                            unsigned int left_tile_map_column_index = current_tile_map->OverworldColumnIndex - 1;
                            MAPS::TileMap* left_tile_map = overworld.GetTileMap(
                                left_tile_map_row_index,
                                left_tile_map_column_index);
                            bool left_tile_map_exists = (nullptr != left_tile_map);
                            if (left_tile_map_exists)
                            {
                                // MOVE NOAH A FEW MORE PIXELS LEFT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                                MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                                noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                // START SCROLLING TO THE LEFT TILE MAP.
                                MATH::Vector2f scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                MATH::Vector2f scroll_end_position = left_tile_map->GetCenterWorldPosition();
                                camera.StartScrolling(scroll_start_position, scroll_end_position);

                                // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                                input_controller.DisableInput();
                            }
                            else
                            {
                                // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                                // Since there is no left tile map to scroll to, this will keep Noah on-screen.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_left_boundary = tile_map_bounding_box.GetLeftXPosition();

                                // To keep Noah completely on screen, his center position should be half
                                // his width to the right of the left tile map boundary.
                                MATH::Vector2f noah_world_position = old_noah_position;
                                float noah_half_width = noah_player.GetWorldBoundingBox().GetWidth() / 2.0f;
                                noah_world_position.X = tile_map_left_boundary + noah_half_width;

                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        } // end if (player_moved_out_of_view)
                    } // end if (input_controller.LeftButtonPressed())
                    if (input_controller.RightButtonPressed())
                    {
                        // TRACK NOAH AS MOVING THIS FRAME.
                        noah_moved_this_frame = true;

                        // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                        noah_player.FacingDirection = CORE::Direction::RIGHT;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        // MOVE NOAH WHILE HANDLING COLLISIONS.
                        MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::RIGHT,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_right_x_position = camera_bounds.GetRightXPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetRightXPosition() > camera_right_x_position);
                        if (player_moved_out_of_view)
                        {
                            // CHECK IF A RIGHT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                            unsigned int right_tile_map_row_index = current_tile_map->OverworldRowIndex;
                            unsigned int right_tile_map_column_index = current_tile_map->OverworldColumnIndex + 1;
                            MAPS::TileMap* right_tile_map = overworld.GetTileMap(
                                right_tile_map_row_index,
                                right_tile_map_column_index);
                            bool right_tile_map_exists = (nullptr != right_tile_map);
                            if (right_tile_map_exists)
                            {
                                // MOVE NOAH A FEW MORE PIXELS RIGHT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                                MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                                noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                // START SCROLLING TO THE RIGHT TILE MAP.
                                MATH::Vector2f scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                MATH::Vector2f scroll_end_position = right_tile_map->GetCenterWorldPosition();
                                camera.StartScrolling(scroll_start_position, scroll_end_position);

                                // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                                input_controller.DisableInput();
                            }
                            else
                            {
                                // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                                // Since there is no right tile map to scroll to, this will keep Noah on-screen.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_right_boundary = tile_map_bounding_box.GetRightXPosition();

                                // To keep Noah completely on screen, his center position should be half
                                // his width to the left of the right tile map boundary.
                                MATH::Vector2f noah_world_position = old_noah_position;
                                float noah_half_width = noah_player.GetWorldBoundingBox().GetWidth() / 2.0f;
                                noah_world_position.X = tile_map_right_boundary - noah_half_width;

                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        } // end if (player_moved_out_of_view)
                    } // end if (input_controller.RightButtonPressed())
                } // end if (!axe_is_swinging)

                // STOP NOAH'S ANIMATION FROM PLAYING IF THE PLAYER DIDN'T MOVE THIS FRAME.
                if (!noah_moved_this_frame)
                {
                    noah_player.Sprite.ResetAnimation();
                }

                // UPDATE THE CAMERA BASED ON SCROLLING.
                if (camera.IsScrolling)
                {
                    // SCROLL BASED ON THE ELAPSED FRAME TIME.
                    camera.Scroll(elapsed_time_in_seconds);

                    // CHECK IF SCROLLING HAS FINISHED.
                    bool scrolling_finished = !camera.IsScrolling;
                    if (scrolling_finished)
                    {
                        // RE-ENABLE PLAYER INPUT.
                        input_controller.EnableInput();
                    }
                }
                else
                {
                    // POSITION THE CAMERA TO FOCUS ON THE CENTER OF THE CURRENT TILE MAP.
                    MATH::Vector2f center_world_position = current_tile_map->GetCenterWorldPosition();
                    camera.SetCenter(center_world_position);
                }

                // HANDLE OTHER COLLISIONS.
                COLLISION::HandleAxeSwingCollisions(overworld, axe_swings, assets);
                for (auto wood_logs = current_tile_map->WoodLogs.begin(); wood_logs != current_tile_map->WoodLogs.end();)
                {
                    // CHECK IF THE WOOD LOGS INTERSECT WITH NOAH.
                    MATH::FloatRectangle wood_log_bounding_box = wood_logs->GetWorldBoundingBox();
                    MATH::FloatRectangle noah_bounding_box = noah_player.GetWorldBoundingBox();
                    /// @todo   Re-examine this later.  For now, we are forcing players to run over the center of
                    /// the wood logs to collect them.  This mostly seems fine.  However, it doesn't fully
                    /// solve the problem (it is still possible for the player to collect the wood without
                    /// ever actually seeing it).
                    bool noah_collided_with_wood_logs = noah_bounding_box.Contains(
                        wood_log_bounding_box.GetCenterXPosition(), 
                        wood_log_bounding_box.GetCenterYPosition());
                    if (noah_collided_with_wood_logs)
                    {
                        // ADD THE WOOD TO NOAH'S INVENTORY.
                        /// @todo   Make the wood logs have a random amount of wood?
                        noah_player.Inventory.AddWood();

                        /// @todo   Remove this debug printing.
                        /// Just temporary until time is dedicated to creating a graphical version.
                        std::cout << "Wood count: " << noah_player.Inventory.WoodCount << std::endl;

                        // REMOVE THE WOOD LOGS SINCE THEY'VE BEEN COLLECTED BY NOAH.
                        wood_logs = current_tile_map->WoodLogs.erase(wood_logs);

                        // SEE IF A BIBLE VERSE SHOULD BE COLLECTED ALONG WITH THE WOOD.
                        // There should be a random chance that a Bible verse can be collected.
                        const unsigned int EVENLY_DIVISIBLE = 0;
                        /// @todo   Tweak this value later as needed.  Somewhere between 2 and 3
                        /// seems about right, but the value that feels best will have to be
                        /// determined based on experience within the context of the full
                        /// game.
                        const unsigned int BIBLE_VERSE_EXISTS_IF_DIVISIBLE_BY_THIS = 2;
                        unsigned int random_number = random_number_generator();
                        bool bible_verse_exists_with_wood = ((random_number % BIBLE_VERSE_EXISTS_IF_DIVISIBLE_BY_THIS) == EVENLY_DIVISIBLE);
                        if (bible_verse_exists_with_wood)
                        {
                            // CHECK IF ANY BIBLE VERSES REMAIN.
                            // This check helps protect against a mod by 0 below.
                            bool bible_verses_remain_to_be_found = !bible_verses_left_to_find.empty();
                            if (bible_verses_remain_to_be_found)
                            {
                                // SELECT A RANDOM BIBLE VERSE.
                                unsigned int random_bible_verse_index = random_number_generator() % bible_verses_left_to_find.size();
                                auto bible_verse = bible_verses_left_to_find.begin() + random_bible_verse_index;

                                // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                                noah_player.Inventory.BibleVerses.push_back(*bible_verse);

                                /// @todo   Remove this debug printing.
                                /// Just temporary until graphical inventory is implemented.
                                std::cout
                                    << "You got a Bible verse!\n"
                                    /// @todo   Make Bible book names printable!
                                    << static_cast<int>(bible_verse->Book) << " " << bible_verse->Chapter << ":" << bible_verse->Verse
                                    << " - " << bible_verse->Text
                                    << std::endl;

                                // REMOVE THE VERSE SINCE IT HAS BEEN FOUND.
                                bible_verses_left_to_find.erase(bible_verse);
                            }
                        }
                    }
                    else
                    {
                        // MOVE TO CHECKING COLLISIONS WITH THE NEXT SET OF WOOD LOGS.
                        ++wood_logs;
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
                noah_player.Axe->Update(elapsed_time_in_seconds);
                noah_player.Sprite.Update(elapsed_time_in_seconds);
                GRAPHICS::Render(noah_player.Sprite, *window);

                // The axe should only be rendered if it is swinging.
                if (noah_player.Axe->IsSwinging())
                {
                    GRAPHICS::Render(noah_player.Axe->Sprite, *window);
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
