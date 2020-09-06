#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <ctime>
#include <limits>
#include "Bible/BibleVerses.h"
#include "Collision/CollisionDetectionAlgorithms.h"
#include "Debugging/DebugConsole.h"
#include "ErrorHandling/NullChecking.h"
#include "Gameplay/RandomAnimalGenerationAlgorithm.h"
#include "States/GameplayState.h"

namespace STATES
{
    /// Initializes the gameplay state.
    /// @param[in]  saved_game_data - The saved game data to use to initialize the gameplay state.
    /// @param[in,out]  world - The world for the gameplay state.
    /// @param[in,out]  renderer - The renderer used fro some initialization.
    /// @return True if initialization succeeded; false otherwise.
    bool GameplayState::Initialize(
        const SavedGameData& saved_game_data,
        MAPS::World& world,
        GRAPHICS::Renderer& renderer)
    {
        // INITIALIZE THE WORLD.
        CurrentMapGrid = &world.Overworld;

        // Built ark pieces need to be initialized.
        world.InitializeBuiltArkInOverworld(saved_game_data.BuildArkPieces);

        // FOCUS THE CAMERA ON THE PLAYER.
        MATH::Vector2f player_start_world_position = world.NoahPlayer->GetWorldPosition();
        renderer.Camera.SetCenter(player_start_world_position);

        // INITIALIZE THE BIBLE VERSES LEFT TO FIND.
        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            saved_game_data.Player->Inventory.BibleVerses.cbegin(),
            saved_game_data.Player->Inventory.BibleVerses.cend(),
            std::inserter(BibleVersesLeftToFind, BibleVersesLeftToFind.begin()));

        // INDICATE THAT INITIALIZATION SUCCEEDED.
        return true;
    }

    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output for the update.
    /// @param[in,out]  world - The world to update based on gameplay.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @return The next game state after updating.
    GameState GameplayState::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        GRAPHICS::GUI::HeadsUpDisplay& hud,
        STATES::SavedGameData& current_game_data)
    {
#ifdef _DEBUG
        // UPDATE THE TILE MAP EDITOR IN RESPONSE TO USER INPUT.
        TileMapEditorGui.RespondToInput(gaming_hardware.InputController);
        if (TileMapEditorGui.Visible)
        {
            // MAKE SURE THE TILE MAP EDITOR GUI HAS THE CURRENT TILE MAP.
            MATH::FloatRectangle camera_bounds = camera.ViewBounds;
            MATH::Vector2f camera_view_center = camera_bounds.Center();
            MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
            TileMapEditorGui.CurrentTileMap = current_tile_map;

            // FINISH UPDATING.
            // If the tile map editor is displayed, it should have
            // full control over updating to avoid interference
            // by other components.
            return GameState::GAMEPLAY;
        }
        else
        {
            // CLEAR THE TILE MAP EDITOR GUI'S CURRENT TILE MAP.
            TileMapEditorGui.CurrentTileMap = nullptr;
        }
#endif

        // UPDATE THE HUD.
        // As of now, only the HUD is capable of altering the gameplay state.
        GameState next_game_state = hud.Update(gaming_hardware, world, current_game_data);

        // CHECK IF A MODAL HUD COMPONENT IS DISPLAYED.
        // If a modal GUI component is displayed, then the regular controls for the player
        // in the world shouldn't work.
        bool modal_hud_components_displayed = hud.ModalComponentDisplayed();
        if (modal_hud_components_displayed)
        {
            // No further updating is needed.
            return next_game_state;
        }

        // UPDATE THE CURRENT MAP GRID.
        UpdateMapGrid(
            gaming_hardware.Clock.ElapsedTimeSinceLastFrame, 
            world,
            gaming_hardware.InputController, 
            camera, 
            *gaming_hardware.Speakers,
            *CurrentMapGrid,
            hud,
            current_game_data);

        // START PLAYING THE BACKGROUND MUSIC IF ITS NOT ALREADY PLAYING.
        gaming_hardware.Speakers->PlayMusicIfNotAlready(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
        
        // RETURN THE NEXT GAME STATE.
        return next_game_state;
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  world - The world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered gameplay state.
    sf::Sprite GameplayState::Render(
        MAPS::World& world, 
        GRAPHICS::GUI::HeadsUpDisplay& hud,
        STATES::SavedGameData& current_game_data,
        GRAPHICS::Renderer& renderer)
    {
        // RENDER CONTENT SPECIFIC TO THE CURRENT MAP.
        renderer.Render(*CurrentMapGrid);

        // CHECK IF THE TILE MAP EDITOR GUI IS VISIBLE.
        if (TileMapEditorGui.Visible)
        {
            // RENDER THE TILE MAP EDITOR GUI.
            // Other components like the player and HUD aren't rendered
            // because they would distract from the editor.
            TileMapEditorGui.Render(renderer);
        }
        else
        {
            // RENDER ANY ANIMALS ENTERING INTO THE ARK.
            for (const auto& animal : AnimalsGoingIntoArk)
            {
                renderer.Render(animal->Sprite.CurrentFrameSprite);
            }

            // RENDER ANY ANIMALS FOLLOWING NOAH.
            for (const std::shared_ptr<OBJECTS::Animal>& animal : world.NoahPlayer->Inventory.FollowingAnimals.Animals)
            {
                renderer.Render(animal->Sprite.CurrentFrameSprite);
            }

            // CHECK WHICH DIRECTION NOAH IS FACING.
            // If he's facing up, the axe needs to be rendered first
            // so that it appears in-front of him (behind him from
            // the player's view).
            bool noah_facing_up = (GAMEPLAY::Direction::UP == world.NoahPlayer->FacingDirection);
            if (noah_facing_up)
            {
                // RENDER THE AXE.
                // The axe should only be rendered if it is swinging.
                if (world.NoahPlayer->Inventory.Axe->IsSwinging())
                {
                    renderer.Render(world.NoahPlayer->Inventory.Axe->Sprite);
                }

                // RENDER THE PLAYER.
                renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);
            }
            else
            {
                // RENDER THE PLAYER.
                renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);

                // RENDER THE AXE.
                // The axe should only be rendered if it is swinging.
                if (world.NoahPlayer->Inventory.Axe->IsSwinging())
                {
                    renderer.Render(world.NoahPlayer->Inventory.Axe->Sprite);
                }
            }

            // RENDER THE HUD.
            hud.Render(current_game_data, renderer);
        }

        // COMPUTE THE LIGHTING FOR THE CURRENT GAMEPLAY.
        // For main gameplay, the world should be tinted based on the time of day most of the time.
        sf::RenderStates lighting = sf::RenderStates::Default;
        std::shared_ptr<sf::Shader> time_of_day_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::TIME_OF_DAY_SHADER);
        if (time_of_day_shader)
        {
            // GET THE CURRENT COLOR SCALE BASED ON TIME-OF-DAY.
            // If an error occurs getting the current time of day,
            // the normal (maximum) color values will be used
            // (the time-of-day shading feature just won't exist for those users).
            float time_of_day_color_scale = 1.0f;
            std::time_t* const JUST_GET_RETURNED_TIME = nullptr;
            std::time_t current_posix_time = std::time(JUST_GET_RETURNED_TIME);
            std::tm current_time;
            errno_t get_local_time_return_code = localtime_s(&current_time, &current_posix_time);
            const errno_t GET_LOCAL_TIME_SUCCESS_RETURN_CODE = 0;
            bool current_time_retrieved_successfully = (GET_LOCAL_TIME_SUCCESS_RETURN_CODE == get_local_time_return_code);
            if (current_time_retrieved_successfully)
            {
                // 0.4f is the darkest we can go and still have the screen
                // remain reasonably visible.
                const std::size_t HOUR_COUNT_PER_DAY = 24;
                std::array<float, HOUR_COUNT_PER_DAY> HOUR_TO_COLOR_SCALE_LOOKUP =
                {
                    0.45f, // 12am
                    0.4f, // 1am (darkest time)
                    0.45f, // 2am
                    0.50f, // 3am
                    0.55f, // 4am
                    0.60f, // 5am
                    0.65f, // 6am
                    0.70f, // 7am
                    0.75f, // 8am
                    0.80f, // 9am
                    0.85f, // 10am
                    0.90f, // 11am
                    0.95f, // 12pm
                    1.0f, // 1pm (brightest time)
                    0.95f, // 2pm
                    0.90f, // 3pm
                    0.85f, // 4pm
                    0.80f, // 5pm
                    0.75f, // 6pm
                    0.70f, // 7pm
                    0.65f, // 8pm
                    0.60f, // 9pm
                    0.55f, // 10pm
                    0.50f, // 11pm
                };

                time_of_day_color_scale = HOUR_TO_COLOR_SCALE_LOOKUP[current_time.tm_hour];
            }

            // CONFIGURE THE SHADER.
            time_of_day_shader->setUniform("color_scale", time_of_day_color_scale);
            time_of_day_shader->setUniform("texture", sf::Shader::CurrentTexture);

            // RENDER USING THE SHADER.
            lighting.shader = time_of_day_shader.get();
        }

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen(lighting);
        return screen;
    }

    /// Updates a map grid based on elapsed time and player input.
    /// @param[in]  elapsed_time - The amount of time by which to update the game state.
    /// @param[in,out]  world - The world being updated.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @param[in,out]  speakers - The speakers in which to play audio.
    /// @param[in,out]  map_grid - The map grid to update.
    void GameplayState::UpdateMapGrid(
        const sf::Time& elapsed_time,
        MAPS::World& world,
        INPUT_CONTROL::InputController& input_controller,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::GUI::HeadsUpDisplay& hud,
        STATES::SavedGameData& current_game_data)
    {
        // GET THE CURRENT TILE MAP.
        MATH::FloatRectangle camera_bounds = camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.Center();
        MAPS::TileMap* current_tile_map = map_grid.GetTileMap(camera_view_center.X, camera_view_center.Y);
        if (!current_tile_map)
        {
            // A current tile map is required for updating.
            return;
        }

        // UPDATE THE TEXT BOX IF IT IS VISIBLE.
        // If the text box is currently being displayed, then it should capture any user input.
        if (hud.MainTextBox.IsVisible)
        {
            // UPDATE THE TEXT IN THE TEXT BOX.
            hud.MainTextBox.Update(elapsed_time);
        }
        else
        {
            // UPDATE THE PLAYER BASED ON INPUT.
            MAPS::ExitPoint* map_exit_point = UpdatePlayerBasedOnInput(
                elapsed_time, 
                input_controller, 
                world,
                *current_tile_map, 
                map_grid, 
                camera,
                speakers);
            if (map_exit_point)
            {
                // SWITCH OVER TO THE NEW MAP GRID.
                CurrentMapGrid = map_exit_point->NewMapGrid;

                // UPDATE THE CAMERA TO FOCUS ON THE NEW TILE MAP.
                MATH::Vector2f center_world_position = map_exit_point->NewTileMap->GetCenterWorldPosition();
                camera.SetCenter(center_world_position);

                // MOVE THE PLAYER TO THE START POINT OF THE NEW TILE MAP.
                world.NoahPlayer->SetWorldPosition(map_exit_point->NewPlayerWorldPosition);

                // UPDATE THE HUD'S TEXT COLOR BASED THE MAP.
                bool entered_ark = world.Ark.Interior.Contains(CurrentMapGrid);
                if (entered_ark)
                {
                    // White is more readable on-top of the black borders around the ark interior.
                    hud.TextColor = GRAPHICS::Color::WHITE;
                }
                else
                {
                    // The default color of black is more readable in the overworld.
                    hud.TextColor = GRAPHICS::Color::BLACK;
                }

                // EXIT THIS UPDATE IF THE PLAYER HAS CHANGED MAPS.
                return;
            }

            // CLOSE THE ARK DOOR'S IF THE PLAYER IS IN THE ARK AFTER COLLECTION ALL ITEMS.
            bool inside_ark = world.Ark.Interior.Contains(&map_grid);
            if (inside_ark)
            {
                /// @todo   Add non-debug logic for this.
                bool player_collected_all_items = false;
#ifdef _DEBUG
                player_collected_all_items = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_CLOSE_ARK_DOORS_KEY);
#endif
                if (player_collected_all_items)
                {
                    // GET THE TILESET.
                    // It's needed for switching tiles.
                    MAPS::Tileset tileset;
                        
                    // CHANGE ANY ARK EXIT DOORS TO BE CLOSED.
                    for (unsigned int tile_row = 0; tile_row < MAPS::TileMap::HEIGHT_IN_TILES; ++tile_row)
                    {
                        for (unsigned int tile_column = 0; tile_column < MAPS::TileMap::WIDTH_IN_TILES; ++tile_column)
                        {
                            // GET THE CURRENT TILE.
                            std::shared_ptr<MAPS::Tile> current_tile = current_tile_map->Ground.Tiles(tile_column, tile_row);
                            if (!current_tile)
                            {
                                continue;
                            }

                            // CHANGE THE TILE IF IT IS FOR AN ARK EXIT DOOR.
                            switch (current_tile->Type)
                            {
                                case MAPS::TileType::ARK_INTERIOR_CENTER_EXIT:
                                {
                                    std::shared_ptr<MAPS::Tile> center_closed_tile = tileset.CreateTile(MAPS::TileType::ARK_INTERIOR_CENTER_EXIT_CLOSED);
                                    if (center_closed_tile)
                                    {
                                        current_tile_map->Ground.SetTile(tile_column, tile_row, center_closed_tile);
                                    }
                                    break;
                                }
                                case MAPS::TileType::ARK_INTERIOR_LEFT_EXIT:
                                {
                                    std::shared_ptr<MAPS::Tile> left_closed_tile = tileset.CreateTile(MAPS::TileType::ARK_INTERIOR_LEFT_EXIT_CLOSED);
                                    if (left_closed_tile)
                                    {
                                        current_tile_map->Ground.SetTile(tile_column, tile_row, left_closed_tile);
                                    }
                                    break;
                                }
                                case MAPS::TileType::ARK_INTERIOR_RIGHT_EXIT:
                                {
                                    std::shared_ptr<MAPS::Tile> right_closed_tile = tileset.CreateTile(MAPS::TileType::ARK_INTERIOR_RIGHT_EXIT_CLOSED);
                                    if (right_closed_tile)
                                    {
                                        current_tile_map->Ground.SetTile(tile_column, tile_row, right_closed_tile);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // MOVE ANIMALS IN THE WORLD.
            MoveAnimals(elapsed_time, *current_tile_map, map_grid, world);

            // UPDATE FOOD FALLING IN THE WORLD.
            UpdateFallingFood(elapsed_time, *current_tile_map);

            // HANDLE PLAYER COLLISIONS WITH OUTSIDE OBJECTS.
            // These collisions aren't applicable for inside the ark and should be prohibited to prevent
            // things like recollecting of animals that have just entered into the ark.
            if (!inside_ark)
            {
                std::string message_for_text_box;
                CollectWoodAndBibleVersesCollidingWithPlayer(*current_tile_map, map_grid, world, speakers, message_for_text_box);
                CollectFoodCollidingWithPlayer(world, *current_tile_map, speakers);
                CollectAnimalsCollidingWithPlayer(world, *current_tile_map, speakers);

                // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
                bool text_box_message_exists = !message_for_text_box.empty();
                if (text_box_message_exists)
                {
                    hud.MainTextBox.StartDisplayingText(message_for_text_box);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S TILES.
        unsigned int map_height_in_tiles = current_tile_map->Ground.Tiles.GetHeight();
        unsigned int map_width_in_tiles = current_tile_map->Ground.Tiles.GetWidth();
        for (unsigned int tile_y = 0; tile_y < map_height_in_tiles; ++tile_y)
        {
            // UPDATE TILES ACROSS THE CURRENT ROW.
            for (unsigned int tile_x = 0; tile_x < map_width_in_tiles; ++tile_x)
            {
                // UPDATE THE CURRENT TILE.
                auto current_tile = current_tile_map->Ground.Tiles(tile_x, tile_y);
                if (current_tile)
                {
                    current_tile->Sprite.Play();
                    current_tile->Sprite.Update(elapsed_time);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S ANIMALS.
        for (auto& animal : current_tile_map->Animals)
        {
            animal->Sprite.Update(elapsed_time);
        }

        // UPDATE THE CURRENT TILE MAP'S TREES.
        for (auto tree = current_tile_map->Trees.begin(); tree != current_tile_map->Trees.end(); ++tree)
        {
            // UPDATE THE TREE.
            tree->Update(elapsed_time);

            // START PLAYING THE TREE SHAKING SOUND EFFECT IF APPROPRIATE.
            bool is_shaking = tree->IsShaking();
            if (is_shaking)
            {
                // ONLY START PLAYING THE SOUND IF IT ISN'T ALREADY PLAYING.
                // This results in a smoother sound experience.
                bool tree_shake_sound_playing = speakers.SoundIsPlaying(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                if (!tree_shake_sound_playing)
                {
                    speakers.PlaySoundEffect(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
        for (auto dust_cloud = current_tile_map->DustClouds.begin(); dust_cloud != current_tile_map->DustClouds.end();)
        {
            // UPDATE THE CURRENT DUST CLOUD.
            dust_cloud->Update(elapsed_time);

            // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
            bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
            if (dust_cloud_disappeared)
            {
                // REMOVE THE DUST CLOUD.
                dust_cloud = current_tile_map->DustClouds.erase(dust_cloud);
            }
            else
            {
                // MOVE TO UPDATING THE NEXT DUST CLOUD.
                ++dust_cloud;
            }
        }

        // UPDATE NOAH'S AXE AND SPRITE.
        world.NoahPlayer->Inventory.Axe->Update(elapsed_time);

        // UPDATE THE CAMERA'S WORLD VIEW.
        UpdateCameraWorldView(
            elapsed_time,
            world,
            camera,
            speakers,
            input_controller,
            *current_tile_map,
            current_game_data);
    }

    /// Updates the player and related items in the tile map based on input and elapsed time.
    /// @param[in]  elapsed_time - The elapsed time for which to update things.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  world - The world in which the player is being updated.
    /// @param[in,out]  current_tile_map - The tile map the player is currently located in.
    /// @param[in,out]  map_grid - The map grid containing the current tile map.
    /// @param[in,out]  camera - The camera defining the viewable region of the map grid.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    /// @return The map exit point, if the player stepped on such a point.
    MAPS::ExitPoint* GameplayState::UpdatePlayerBasedOnInput(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::World& world,
        MAPS::TileMap& current_tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers)
    {
        MATH::FloatRectangle camera_bounds = camera.ViewBounds;

        // CHECK IF THE PRIMARY ACTION BUTTON WAS PRESSED.
        if (input_controller.ButtonDown(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
        {
            // SWING THE PLAYER'S AXE.
            // A new axe swing may not be created if the player's
            // axe is already being swung.
            std::shared_ptr<GAMEPLAY::AxeSwingEvent> axe_swing = world.NoahPlayer->SwingAxe();
            bool axe_swing_occurred = (nullptr != axe_swing);
            if (axe_swing_occurred)
            {
                // Allow the axe to collide with other objects.
                map_grid.AxeSwings.push_back(axe_swing);
            }
        }

        // GET THE TILE UNDER NOAH.
        // This is needed to help track if Noah moves onto a different type of tile.
        MATH::Vector2f old_noah_position = world.NoahPlayer->GetWorldPosition();
        std::shared_ptr<MAPS::Tile> original_tile_under_noah = current_tile_map.GetTileAtWorldPosition(
            old_noah_position.X,
            old_noah_position.Y);

        /// @todo   Because ark piece tiles are separate from regular tiles, they need to be handled separately.
        /// This likely should be fixed.
        OBJECTS::ArkPiece* original_ark_piece_under_noah = current_tile_map.GetArkPieceAtWorldPosition(old_noah_position);

        // CHECK IF THE PLAYER IS ALLOWED TO MOVE.
        // Noah can't move while the axe is swinging.
        // Movement is prevented to have the axe's position remain attached to Noah.
        // We need to keep track if Noah moved this frame so that we can stop any walking animations for him if he didn't move.
        bool noah_moved_this_frame = false;
        bool axe_is_swinging = world.NoahPlayer->Inventory.Axe->IsSwinging();
        bool player_movement_allowed = (!axe_is_swinging);
        if (player_movement_allowed)
        {
            // MOVE NOAH IN RESPONSE TO USER INPUT.
            const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
            if (input_controller.ButtonDown(sf::Keyboard::Up))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING UP.
                world.NoahPlayer->BeginWalking(GAMEPLAY::Direction::UP, OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    world.NoahPlayer->GetWorldBoundingBox(),
                    GAMEPLAY::Direction::UP,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                world.NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                float camera_top_y_position = camera_bounds.LeftTop.Y;
                bool player_moved_out_of_view = (noah_world_bounding_box.LeftTop.Y < camera_top_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A TOP TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int top_tile_map_row_index = current_tile_map.GridRowIndex - 1;
                    unsigned int top_tile_map_column_index = current_tile_map.GridColumnIndex;
                    MAPS::TileMap* top_tile_map = map_grid.GetTileMap(
                        top_tile_map_row_index,
                        top_tile_map_column_index);
                    bool top_tile_map_exists = (nullptr != top_tile_map);
                    if (top_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS UP SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
                        noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        world.NoahPlayer->SetWorldPosition(noah_world_position);

                        // START SCROLLING TO THE TOP TILE MAP.
                        MATH::Vector2f scroll_start_position = current_tile_map.GetCenterWorldPosition();
                        MATH::Vector2f scroll_end_position = top_tile_map->GetCenterWorldPosition();
                        camera.StartScrolling(scroll_start_position, scroll_end_position);

                        // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                        input_controller.DisableInput();
                    }
                    else
                    {
                        // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                        // Since there is no top tile map to scroll to, this will keep Noah on-screen.
                        MATH::FloatRectangle tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                        float tile_map_top_boundary = tile_map_bounding_box.LeftTop.Y;

                        // To keep Noah completely on screen, his center position should be half
                        // his height below the top tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_height = noah_world_bounding_box.Height() / 2.0f;
                        noah_world_position.Y = tile_map_top_boundary + noah_half_height;

                        world.NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            }
            if (input_controller.ButtonDown(sf::Keyboard::Down))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING DOWN.
                world.NoahPlayer->BeginWalking(GAMEPLAY::Direction::DOWN, OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    world.NoahPlayer->GetWorldBoundingBox(),
                    GAMEPLAY::Direction::DOWN,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                world.NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                float camera_bottom_y_position = camera_bounds.RightBottom.Y;
                bool player_moved_out_of_view = (noah_world_bounding_box.RightBottom.Y > camera_bottom_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A BOTTOM TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int bottom_tile_map_row_index = current_tile_map.GridRowIndex + 1;
                    unsigned int bottom_tile_map_column_index = current_tile_map.GridColumnIndex;
                    MAPS::TileMap* bottom_tile_map = map_grid.GetTileMap(
                        bottom_tile_map_row_index,
                        bottom_tile_map_column_index);
                    bool bottom_tile_map_exists = (nullptr != bottom_tile_map);
                    if (bottom_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS DOWN SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
                        noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        world.NoahPlayer->SetWorldPosition(noah_world_position);

                        // START SCROLLING TO THE BOTTOM TILE MAP.
                        MATH::Vector2f scroll_start_position = current_tile_map.GetCenterWorldPosition();
                        MATH::Vector2f scroll_end_position = bottom_tile_map->GetCenterWorldPosition();
                        camera.StartScrolling(scroll_start_position, scroll_end_position);

                        // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                        input_controller.DisableInput();
                    }
                    else
                    {
                        // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                        // Since there is no bottom tile map to scroll to, this will keep Noah on-screen.
                        MATH::FloatRectangle tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                        float tile_map_bottom_boundary = tile_map_bounding_box.RightBottom.Y;

                        // To keep Noah completely on screen, his center position should be half
                        // his height above the bottom tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_height = world.NoahPlayer->GetWorldBoundingBox().Height() / 2.0f;
                        noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;

                        world.NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(sf::Keyboard::Left))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING LEFT.
                world.NoahPlayer->BeginWalking(GAMEPLAY::Direction::LEFT, OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    world.NoahPlayer->GetWorldBoundingBox(),
                    GAMEPLAY::Direction::LEFT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                world.NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                float camera_left_x_position = camera_bounds.LeftTop.X;
                bool player_moved_out_of_view = (noah_world_bounding_box.LeftTop.X < camera_left_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A LEFT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int left_tile_map_row_index = current_tile_map.GridRowIndex;
                    unsigned int left_tile_map_column_index = current_tile_map.GridColumnIndex - 1;
                    MAPS::TileMap* left_tile_map = map_grid.GetTileMap(
                        left_tile_map_row_index,
                        left_tile_map_column_index);
                    bool left_tile_map_exists = (nullptr != left_tile_map);
                    if (left_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS LEFT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
                        noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        world.NoahPlayer->SetWorldPosition(noah_world_position);

                        // START SCROLLING TO THE LEFT TILE MAP.
                        MATH::Vector2f scroll_start_position = current_tile_map.GetCenterWorldPosition();
                        MATH::Vector2f scroll_end_position = left_tile_map->GetCenterWorldPosition();
                        camera.StartScrolling(scroll_start_position, scroll_end_position);

                        // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                        input_controller.DisableInput();
                    }
                    else
                    {
                        // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                        // Since there is no left tile map to scroll to, this will keep Noah on-screen.
                        MATH::FloatRectangle tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                        float tile_map_left_boundary = tile_map_bounding_box.LeftTop.X;

                        // To keep Noah completely on screen, his center position should be half
                        // his width to the right of the left tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_width = world.NoahPlayer->GetWorldBoundingBox().Width() / 2.0f;
                        noah_world_position.X = tile_map_left_boundary + noah_half_width;

                        world.NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(sf::Keyboard::Right))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING RIGHT.
                world.NoahPlayer->BeginWalking(GAMEPLAY::Direction::RIGHT, OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    world.NoahPlayer->GetWorldBoundingBox(),
                    GAMEPLAY::Direction::RIGHT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                world.NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                float camera_right_x_position = camera_bounds.RightBottom.X;
                bool player_moved_out_of_view = (noah_world_bounding_box.RightBottom.X > camera_right_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A RIGHT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int right_tile_map_row_index = current_tile_map.GridRowIndex;
                    unsigned int right_tile_map_column_index = current_tile_map.GridColumnIndex + 1;
                    MAPS::TileMap* right_tile_map = map_grid.GetTileMap(
                        right_tile_map_row_index,
                        right_tile_map_column_index);
                    bool right_tile_map_exists = (nullptr != right_tile_map);
                    if (right_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS RIGHT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
                        noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        world.NoahPlayer->SetWorldPosition(noah_world_position);

                        // START SCROLLING TO THE RIGHT TILE MAP.
                        MATH::Vector2f scroll_start_position = current_tile_map.GetCenterWorldPosition();
                        MATH::Vector2f scroll_end_position = right_tile_map->GetCenterWorldPosition();
                        camera.StartScrolling(scroll_start_position, scroll_end_position);

                        // DISABLE PLAYER MOVEMENT WHILE SCROLLING IS OCCURRING.
                        input_controller.DisableInput();
                    }
                    else
                    {
                        // KEEP NOAH WITHIN THE BOUNDS OF THE CURRENT TILE MAP.
                        // Since there is no right tile map to scroll to, this will keep Noah on-screen.
                        MATH::FloatRectangle tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                        float tile_map_right_boundary = tile_map_bounding_box.RightBottom.X;

                        // To keep Noah completely on screen, his center position should be half
                        // his width to the left of the right tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_width = world.NoahPlayer->GetWorldBoundingBox().Width() / 2.0f;
                        noah_world_position.X = tile_map_right_boundary - noah_half_width;

                        world.NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
        }

        // CHECK IF NOAH MOVED THIS FRAME.
        if (noah_moved_this_frame)
        {
            // MOVE ANY FOLLOWING ANIMALS ALONG WITH NOAH.
            /// @todo   Clean this up...I don't like how non-encapsulated this is, but this might be easier
            /// to clean-up after some more changes later.  Maybe we should have a simpler "just stay this
            /// close to Noah" logic, which could add more dynamism?
            MATH::Vector2f new_noah_position = world.NoahPlayer->GetWorldPosition();
            MATH::Vector2f noah_movement = new_noah_position - old_noah_position;
            world.NoahPlayer->Inventory.FollowingAnimals.CurrentCenterWorldPosition += noah_movement;
            for (const std::shared_ptr<OBJECTS::Animal>& animal : world.NoahPlayer->Inventory.FollowingAnimals.Animals)
            {
                MATH::Vector2f old_animal_world_position = animal->Sprite.GetWorldPosition();
                MATH::Vector2f new_animal_world_position = old_animal_world_position + noah_movement;
                animal->Sprite.SetWorldPosition(new_animal_world_position);
            }

            // UPDATE NOAH'S ANIMATION.
            world.NoahPlayer->Sprite.Update(elapsed_time);

            // BUILD A PIECE OF THE ARK IF NOAH STEPPED ONTO AN APPROPRIATE SPOT.
            MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
            MAPS::TileMap* tile_map_underneath_noah = map_grid.GetTileMap(noah_world_position.X, noah_world_position.Y);
            if (!tile_map_underneath_noah)
            {
                // If no tile map exists, then Noah couldn't have stepped on an exit point.
                return nullptr;
            }
            
            // An ark piece only needs to be built once and requires wood to be built.
            OBJECTS::ArkPiece* ark_piece = tile_map_underneath_noah->GetArkPieceAtWorldPosition(noah_world_position);
            bool noah_has_wood = (world.NoahPlayer->Inventory.WoodCount > 0);
            bool ark_piece_can_be_built = (ark_piece && !ark_piece->Built && noah_has_wood);
            if (ark_piece_can_be_built)
            {
                // USE THE WOOD FROM NOAH'S INVENTORY.
                --world.NoahPlayer->Inventory.WoodCount;

                // BUILD THE ARK PIECE.
                ark_piece->Built = true;

                // When building an ark piece, a dust cloud should appear.
                OBJECTS::DustCloud dust_cloud(RESOURCES::AssetId::DUST_CLOUD_TEXTURE);

                // The dust cloud should be positioned over the ark piece.
                MATH::Vector2f dust_cloud_center_world_position = ark_piece->Sprite.WorldPosition;
                dust_cloud.Sprite.SetWorldPosition(dust_cloud_center_world_position);

                // The dust cloud should start animating immediately.
                dust_cloud.Sprite.Play();

                // The dust cloud needs to be added to the tile map so that it gets updated.
                tile_map_underneath_noah->DustClouds.push_back(dust_cloud);

                // Play a sound to indicate a piece of the ark is being built.
                speakers.PlaySoundEffect(RESOURCES::AssetId::ARK_BUILDING_SOUND);
            }
            
            // CHECK IF THE PLAYER STEPPED ON AN EXIT POINT.
            // This should only occur if the player has changed to a different tile.
            // Otherwise, the game might quickly flip back and forth between maps.
            std::shared_ptr<MAPS::Tile> tile_under_noah = tile_map_underneath_noah->GetTileAtWorldPosition(
                noah_world_position.X,
                noah_world_position.Y);
            bool ground_tile_changed = (
                (original_tile_under_noah && tile_under_noah) &&
                (original_tile_under_noah->Id != tile_under_noah->Id));
            bool ark_piece_changed = (
                (original_ark_piece_under_noah && ark_piece) &&
                (original_ark_piece_under_noah->IsExternalDoorway != ark_piece->IsExternalDoorway));
            bool noah_stepped_onto_new_tile = (ground_tile_changed || ark_piece_changed);
            if (noah_stepped_onto_new_tile)
            {
                MAPS::ExitPoint* exit_point = tile_map_underneath_noah->GetExitPointAtWorldPosition(noah_world_position);
                return exit_point;
            }
        }
        else
        {
            // STOP NOAH'S ANIMATION FROM PLAYING SINCE THE PLAYER DIDN'T MOVE THIS FRAME.
            world.NoahPlayer->Sprite.ResetAnimation();
        }

        // INDICATE THAT THE PLAYER DIDN'T STEP ON AN EXIT POINT.
        return nullptr;
    }

    /// Moves animals in the provided tile map based on the elapsed time.
    /// @param[in]  elapsed_time - The elapsed time for which to move the animals.
    /// @param[in,out]  tile_map - The tile map in which to move animals.
    /// @param[in,out]  map_grid - The map grid containing the tile map.
    /// @param[in,out]  world - The world containing the map.
    void GameplayState::MoveAnimals(
        const sf::Time& elapsed_time, 
        MAPS::TileMap& tile_map, 
        MAPS::MultiTileMapGrid& map_grid,
        MAPS::World& world)
    {
        // UPDATE ANIMALS FOLLOWING NOAH.
        // They should appear right behind Noah.
        MATH::FloatRectangle noah_world_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
        // Defaults to appearing in the same location as Noah for easier visibility into problems.
        MATH::Vector2f noah_center_world_position = noah_world_bounding_box.Center();
        MATH::Vector2f following_animal_group_world_position = noah_center_world_position;
        switch (world.NoahPlayer->FacingDirection)
        {
            case GAMEPLAY::Direction::UP:
                // The animals should appear below Noah.
                following_animal_group_world_position.Y = noah_world_bounding_box.RightBottom.Y + GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::DOWN:
                // The animals should appear above Noah.
                following_animal_group_world_position.Y = noah_world_bounding_box.LeftTop.Y - GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::LEFT:
                // The animals should appear to the right of Noah.
                following_animal_group_world_position.X = noah_world_bounding_box.RightBottom.X + GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
            case GAMEPLAY::Direction::RIGHT:
                // The animals should appear to the left of Noah.
                following_animal_group_world_position.X = noah_world_bounding_box.LeftTop.X - GAMEPLAY::FollowingAnimalGroup::DIMENSION_IN_PIXELS;
                break;
        }
        world.NoahPlayer->Inventory.FollowingAnimals.Update(elapsed_time, following_animal_group_world_position);

        // CHECK IF THE CURRENT TILE MAP HAS A VISIBLE EXTERNAL ARK DOORWAY.
        // This is how animals following Noah get transferred into the ark.
        bool inside_ark = world.Ark.Interior.Contains(&map_grid);
        const OBJECTS::ArkPiece* doorway_into_ark = nullptr;
        for (const OBJECTS::ArkPiece& ark_piece : tile_map.ArkPieces)
        {
            bool is_doorway_into_ark = !inside_ark && ark_piece.Built && ark_piece.IsExternalDoorway;
            if (is_doorway_into_ark)
            {
                doorway_into_ark = &ark_piece;
                break;
            }
        }
        if (doorway_into_ark)
        {
            // TRANSFER THE ANIMALS CURRENTLY FOLLOWING NOAH OVER TO MOVING INTO THE ARK.
            AnimalsGoingIntoArk.insert(
                AnimalsGoingIntoArk.cend(),
                world.NoahPlayer->Inventory.FollowingAnimals.Animals.cbegin(),
                world.NoahPlayer->Inventory.FollowingAnimals.Animals.cend());
            world.NoahPlayer->Inventory.FollowingAnimals.Animals.clear();

            // MOVE THE ANIMALS GOING INTO THE ARK CLOSER INTO THE ARK.
            MATH::FloatRectangle ark_doorway_bounding_box = doorway_into_ark->Sprite.GetWorldBoundingBox();
            MATH::Vector2f ark_doorway_world_position = doorway_into_ark->Sprite.WorldPosition;
            MAPS::ExitPoint* entry_point_into_ark = tile_map.GetExitPointAtWorldPosition(ark_doorway_world_position);
            for (auto animal = AnimalsGoingIntoArk.begin(); animal != AnimalsGoingIntoArk.end(); )
            {
                // UPDATE THE ANIMAL'S ANIMATION.
                (*animal)->Sprite.Update(elapsed_time);

                // DETERMINE THE DIRECTION FROM THE ANIMAL TO THE DOORWAY.
                MATH::Vector2f animal_world_position = (*animal)->Sprite.GetWorldPosition();
                MATH::Vector2f animal_to_ark_doorway_vector = ark_doorway_world_position - animal_world_position;
                MATH::Vector2f animal_to_ark_doorway_direction = MATH::Vector2f::Normalize(animal_to_ark_doorway_vector);

                // CALCULATE THE DISTANCE THE ANIMAL NEEDS TO MOVE.
                float elapsed_time_in_seconds = elapsed_time.asSeconds();
                float animal_move_distance_in_pixels = (*animal)->Type.MoveSpeedInPixelsPerSecond * elapsed_time_in_seconds;
                MATH::Vector2f animal_move_vector = MATH::Vector2f::Scale(animal_move_distance_in_pixels, animal_to_ark_doorway_direction);

                // MOVE THE ANIMAL.
                /// @todo   Should we take tile types into account?
                MATH::Vector2f new_animal_world_position = animal_world_position + animal_move_vector;
                (*animal)->Sprite.SetWorldPosition(new_animal_world_position);

                // TRANSFER THE ANIMAL INTO THE ARK IF IT HAS REACHED THE DOORWAY.
                MATH::FloatRectangle animal_bounding_box = (*animal)->Sprite.GetWorldBoundingBox();
                bool animal_reached_doorway = ark_doorway_bounding_box.Intersects(animal_bounding_box);
                if (animal_reached_doorway)
                {
                    // ADD THE ANIMAL INTO THE APPROPRIATE TILE MAP OF THE ARK.
                    // This check is a precaution.  There should always be an entry point into the ark for the doorway.
                    assert(entry_point_into_ark);
                    if (entry_point_into_ark)
                    {
                        MAPS::TileMap* entry_room_inside_ark = entry_point_into_ark->NewTileMap;
                        assert(entry_room_inside_ark);
                        if (entry_room_inside_ark)
                        {
                            (*animal)->Sprite.SetWorldPosition(entry_point_into_ark->NewPlayerWorldPosition);
                            entry_room_inside_ark->Animals.push_back(*animal);
                        }
                    }

                    // REMOVE THE ANIMAL FROM ITS STATE OF TRYING TO ENTER THE ARK.
                    animal = AnimalsGoingIntoArk.erase(animal);
                }
                else
                {
                    // MOVE TO UPDATING THE NEXT ANIMAL.
                    ++animal;
                }
            }
        }

        // MOVE EACH ANIMAL IN THE TILE MAP CLOSER TO NOAH IF THEY'RE OUTSIDE.
        if (!inside_ark)
        {
            for (auto& animal : tile_map.Animals)
            {
                // DETERMINE THE DIRECTION FROM THE ANIMAL TO THE PLAYER.
                // The animal should move closer to Noah based on Genesis 6:20.
                MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
                MATH::Vector2f animal_world_position = animal->Sprite.GetWorldPosition();
                MATH::Vector2f animal_to_noah_vector = noah_world_position - animal_world_position;
                MATH::Vector2f animal_to_noah_direction = MATH::Vector2f::Normalize(animal_to_noah_vector);

                // CALCULATE THE DISTANCE THE ANIMAL NEEDS TO MOVE.
                float elapsed_time_in_seconds = elapsed_time.asSeconds();
                float animal_move_distance_in_pixels = animal->Type.MoveSpeedInPixelsPerSecond * elapsed_time_in_seconds;
                MATH::Vector2f animal_move_vector = MATH::Vector2f::Scale(animal_move_distance_in_pixels, animal_to_noah_direction);

                // DETERMINE THE TYPES OF TILES THE ANIMAL IS ALLOWED TO MOVE OVER.
                std::unordered_set<MAPS::TileType::Id> tile_types_allowed_to_move_over =
                {
                    MAPS::TileType::SAND,
                    MAPS::TileType::GRASS,
                    MAPS::TileType::BROWN_DIRT,
                    MAPS::TileType::GRAY_STONE
                };
                bool animal_can_fly = animal->Type.CanFly();
                bool animal_can_swim = animal->Type.CanSwim();
                bool animal_move_move_over_water = (animal_can_fly || animal_can_swim);
                if (animal_move_move_over_water)
                {
                    // LET THE ANIMAL MOVE OVER WATER.
                    tile_types_allowed_to_move_over.insert(MAPS::TileType::WATER_TYPES.cbegin(), MAPS::TileType::WATER_TYPES.cend());
                }

                // MOVE THE ANIMAL.
                MATH::FloatRectangle animal_world_bounding_box = animal->Sprite.GetWorldBoundingBox();
                bool allow_movement_over_solid_objects = animal_can_fly;
                MATH::Vector2f new_animal_world_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    animal_world_bounding_box,
                    animal_move_vector,
                    tile_types_allowed_to_move_over,
                    allow_movement_over_solid_objects,
                    map_grid);
                animal->Sprite.SetWorldPosition(new_animal_world_position);
            }
        }

        // MOVE ANIMALS INSIDE THE ARK.
        /// @todo   Come up with better movement!
        if (inside_ark)
        {
            for (auto& animal : tile_map.Animals)
            {
                // ADD A BIT OF A DYNAMIC OFFSET TO THE ANIMAL TO MAKE IT APPEAR TO JUMP AROUND A BIT.
                // Jumping is based on the position of the animal is used to give a bit more dynamism
                // for animals by avoiding having all animals jump at the same frequency.
                MATH::Vector2f old_animal_world_position = animal->Sprite.GetWorldPosition();
                MATH::Vector2f new_animal_world_position = old_animal_world_position;

                // A sine wave is used to control vertical jumping.
                constexpr float MAX_VERTICAL_JUMP_AMOUNT_IN_PIXELS = 3.0f;
                float sine_of_y_position = std::sinf(old_animal_world_position.Y);
                float vertical_jump_amount_in_pixels = MAX_VERTICAL_JUMP_AMOUNT_IN_PIXELS * sine_of_y_position;
                new_animal_world_position.Y += vertical_jump_amount_in_pixels;

                // A cosine wave is used to control horizontal jumping.
                constexpr float MAX_HORIZONTAL_JUMP_AMOUNT_IN_PIXELS = 3.0f;
                float cosine_of_x_position = std::cosf(old_animal_world_position.X);
                float horizontal_jump_amount_in_pixels = MAX_HORIZONTAL_JUMP_AMOUNT_IN_PIXELS * cosine_of_x_position;
                new_animal_world_position.X += horizontal_jump_amount_in_pixels;

                // UPDATE THE ANIMAL'S WORLD POSITION.
                animal->Sprite.SetWorldPosition(new_animal_world_position);

                // UPDATE THE ANIMAL'S ANIMATION.
                animal->Sprite.Update(elapsed_time);
            }
        }
    }

    /// Updates any falling food in the tile map.
    /// @param[in]  elapsed_time - The elapsed time for which to update the food.
    /// @param[in,out]  tile_map - The tile map whose falling food to update.
    void GameplayState::UpdateFallingFood(const sf::Time& elapsed_time, MAPS::TileMap& tile_map)
    {
        // UPDATE ANY FALLING FOOD.
        for (auto food = tile_map.FallingFood.begin();
            food != tile_map.FallingFood.end();)
        {
            // UPDATE THE CURRENT FOOD ITEM.
            food->Fall(elapsed_time);

            // TRANSFER THE FOOD OVER IF IT HAS FINISHED FALLING.
            bool food_finished_falling = food->FinishedFalling();
            if (food_finished_falling)
            {
                tile_map.FoodOnGround.push_back(food->FoodItem);
                food = tile_map.FallingFood.erase(food);
            }
            else
            {
                // MOVE TO THE NEXT FALLING FOOD ITEM.
                ++food;
            }
        }
    }

    /// Determines if the player is colliding with any wood logs in the tile map.
    /// If so, the wood logs, and potentially Bible verses, are added to the player's inventory.
    /// @param[in,out]  tile_map - The tile map to examine wood logs in.
    /// @param[in,out]  map_grid - The grid containing the tile map.
    /// @param[in,out]  world - The world containing the wood.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    /// @param[out] message_for_text_box - A message for the HUD's main text box, if
    ///     a Bible verse was collected.
    void GameplayState::CollectWoodAndBibleVersesCollidingWithPlayer(
        MAPS::TileMap& tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        MAPS::World& world,
        AUDIO::Speakers& speakers,
        std::string& message_for_text_box)
    {
        // INDICATE THAT NO MESSAGE EXISTS FOR THE TEXT BOX YET.
        message_for_text_box.clear();

        // HANDLE PLAYER COLLISIONS WITH WOOD LOGS.
        COLLISION::CollisionDetectionAlgorithms::HandleAxeSwings(map_grid, map_grid.AxeSwings, speakers);
        for (auto wood_logs = tile_map.WoodLogs.begin(); wood_logs != tile_map.WoodLogs.end();)
        {
            // CHECK IF THE WOOD LOGS INTERSECT WITH NOAH.
            MATH::FloatRectangle wood_log_bounding_box = wood_logs->GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
            bool noah_collided_with_wood_logs = noah_bounding_box.Contains(
                wood_log_bounding_box.CenterX(),
                wood_log_bounding_box.CenterY());
            if (noah_collided_with_wood_logs)
            {
                // ADD THE WOOD TO NOAH'S INVENTORY.
                // The logs can have a random amount of wood.
                unsigned int MIN_WOOD_COUNT = 1;
                unsigned int MAX_WOOD_COUNT = 3;
                unsigned int random_wood_count = RandomNumberGenerator.RandomInRange<unsigned int>(MIN_WOOD_COUNT, MAX_WOOD_COUNT);
                world.NoahPlayer->Inventory.AddWood(random_wood_count);

                // REMOVE THE WOOD LOGS SINCE THEY'VE BEEN COLLECTED BY NOAH.
                wood_logs = tile_map.WoodLogs.erase(wood_logs);

                // SEE IF A BIBLE VERSE SHOULD BE COLLECTED ALONG WITH THE WOOD.
                // There should be a random chance that a Bible verse can be collected.
                const unsigned int EVENLY_DIVISIBLE = 0;
                const unsigned int BIBLE_VERSE_EXISTS_IF_DIVISIBLE_BY_THIS = 2;
                unsigned int random_number_for_bible_verse = RandomNumberGenerator.RandomNumber<unsigned int>();
                bool bible_verse_exists_with_wood = ((random_number_for_bible_verse % BIBLE_VERSE_EXISTS_IF_DIVISIBLE_BY_THIS) == EVENLY_DIVISIBLE);
                if (bible_verse_exists_with_wood)
                {
                    // CHECK IF ANY BIBLE VERSES REMAIN.
                    // This check helps protect against a mod by 0 below.
                    bool bible_verses_remain_to_be_found = !BibleVersesLeftToFind.empty();
                    if (bible_verses_remain_to_be_found)
                    {
                        // PLAY THE SOUND EFFECT FOR COLLECTING A BIBLE VERSE.
                        speakers.PlaySoundEffect(RESOURCES::AssetId::COLLECT_BIBLE_VERSE_SOUND);

                        // SELECT A RANDOM BIBLE VERSE.
                        size_t remaining_bible_verse_count = BibleVersesLeftToFind.size();
                        size_t random_bible_verse_index = RandomNumberGenerator.RandomNumberLessThan(remaining_bible_verse_count);
                        auto bible_verse = BibleVersesLeftToFind.begin() + random_bible_verse_index;

                        // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                        world.NoahPlayer->Inventory.BibleVerses.insert(*bible_verse);

                        // POPULATE THE MESSAGE TO DISPLAY IN THE MAIN TEXT BOX.
                        message_for_text_box = "You got a Bible verse!\n" + bible_verse->ToString();

                        // REMOVE THE VERSE SINCE IT HAS BEEN FOUND.
                        BibleVersesLeftToFind.erase(bible_verse);
                    }
                }
            }
            else
            {
                // MOVE TO CHECKING COLLISIONS WITH THE NEXT SET OF WOOD LOGS.
                ++wood_logs;
            }
        }
    }
    
    /// Determines if the player is colliding with any food in the tile map.
    /// If so, the food is added to the player's inventory.
    /// @param[in,out]  world - The world containing the tile map.
    /// @param[in,out]  tile_map - The tile map to examine food in.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    void GameplayState::CollectFoodCollidingWithPlayer(
        MAPS::World& world,
        MAPS::TileMap& tile_map, 
        AUDIO::Speakers& speakers)
    {
        // HANDLE PLAYER COLLISIONS WITH FOOD.
        for (auto food = tile_map.FoodOnGround.cbegin();
            food != tile_map.FoodOnGround.cend();)
        {
            // CHECK IF THE CURRENT FOOD ITEM INTERSECTS WITH THE PLAYER.
            MATH::FloatRectangle food_bounding_box = food->Sprite.GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
            bool food_intersects_with_noah = food_bounding_box.Intersects(noah_bounding_box);
            if (food_intersects_with_noah)
            {
                // PLAY THE SOUND EFFECT FOR COLLECTING FOOD.
                speakers.PlaySoundEffect(RESOURCES::AssetId::FOOD_PICKUP_SOUND);

                // ADD THE FOOD TO THE PLAYER'S INVENTORY.
                DEBUGGING::DebugConsole::WriteLine("Collected food: ", static_cast<int>(food->Type));
                world.NoahPlayer->Inventory.AddFood(*food);

                // REMOVE THE FOOD ITEM FROM THOSE IN THE CURRENT TILE MAP.
                // This should move to the next food ITEM.
                food = tile_map.FoodOnGround.erase(food);
            }
            else
            {
                // MOVE TO CHECKING COLLISIONS FOR THE NEXT FOOD ITEM.
                ++food;
            }
        }
    }

    /// Determines if the player is colliding with any animals in the tile map.
    /// If so, the animals are added to the player's inventory.
    /// @param[in,out]  world - The world containing the tile map.
    /// @param[in,out]  tile_map - The tile map to examine animals in.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    void GameplayState::CollectAnimalsCollidingWithPlayer(
        MAPS::World& world, 
        MAPS::TileMap& tile_map, 
        AUDIO::Speakers& speakers)
    {
        // HANDLE PLAYER COLLISIONS WITH ANIMALS.
        for (auto animal = tile_map.Animals.cbegin();
            animal != tile_map.Animals.cend();)
        {
            // CHECK IF THE CURRENT ANIMAL INTERSECTS WITH THE PLAYER.
            MATH::FloatRectangle animal_bounding_box = (*animal)->Sprite.GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
            bool animal_intersects_with_noah = animal_bounding_box.Intersects(noah_bounding_box);
            if (animal_intersects_with_noah)
            {
                // PLAY THE ANIMAL'S SOUND EFFECT, IF ONE EXISTS.
                speakers.PlaySoundEffect((*animal)->SoundId);

                // ADD THE ANIMAL TO THE PLAYER'S INVENTORY.
                DEBUGGING::DebugConsole::WriteLine("Collected animal.");
                world.NoahPlayer->Inventory.AddAnimal(*animal);

                // REMOVE THE ANIMAL FROM THOSE IN THE CURRENT TILE MAP.
                // This should move to the next animal.
                animal = tile_map.Animals.erase(animal);
            }
            else
            {
                // MOVE TO CHECKING COLLISIONS FOR THE NEXT ANIMAL.
                ++animal;
            }
        }
    }

    /// Updates the camera, along with the world based on any major changes
    /// in what the camera is viewing.
    /// @param[in]  elapsed_time - The elapsed time by which to update the camera.
    /// @param[in,out]  world - The world being viewed.
    /// @param[in,out]  camera - The camera to update.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    /// @param[in,out]  input_controller - The input controller that might
    ///     be tweaked based on camera movement.
    /// @param[in,out]  current_tile_map - The current tile map in view by the camera.
    void GameplayState::UpdateCameraWorldView(
        const sf::Time& elapsed_time,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::TileMap& current_tile_map,
        STATES::SavedGameData& current_game_data)
    {
        if (camera.IsScrolling)
        {
            // SCROLL BASED ON THE ELAPSED FRAME TIME.
            camera.Scroll(elapsed_time);

            // CHECK IF SCROLLING HAS FINISHED.
            bool scrolling_finished = !camera.IsScrolling;
            if (scrolling_finished)
            {
                // RE-ENABLE PLAYER INPUT.
                input_controller.EnableInput();

                // SEE IF AN ANIMAL SHOULD RANDOMLY APPEAR IN THE NEW TILE MAP.
                // This is always set to "true" for debug builds to help speed up collection process.
#if _DEBUG
                bool random_animal_should_be_generated = true;
#else
                constexpr unsigned int EVENLY_DIVISIBLE = 0;
                constexpr unsigned int GENERATE_RANDOM_ANIMAL_IF_DIVISIBLE_BY_THIS = 2;
                unsigned int random_number_for_animal_generation = RandomNumberGenerator.RandomNumber<unsigned int>();
                bool random_animal_should_be_generated = (random_number_for_animal_generation % GENERATE_RANDOM_ANIMAL_IF_DIVISIBLE_BY_THIS) == EVENLY_DIVISIBLE;
#endif
                if (random_animal_should_be_generated)
                {
                    DEBUGGING::DebugConsole::WriteLine("Generating random animal...");
                    DEBUGGING::DebugConsole::WriteLine("Tile map column, row: ", MATH::Vector2ui(current_tile_map.GridColumnIndex, current_tile_map.GridRowIndex));
                    auto tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                    DEBUGGING::DebugConsole::WriteLine("Tile map LTRB: ", tile_map_bounding_box);

                    // GENERATE A RANDOM ANIMAL IN THE CURRENT TILE MAP.
                    std::shared_ptr<OBJECTS::Animal> animal = GAMEPLAY::RandomAnimalGenerationAlgorithm::GenerateAnimal(
                        current_game_data,
                        *world.NoahPlayer,
                        current_tile_map,
                        RandomNumberGenerator);
                    bool animal_generated = (nullptr != animal);
                    if (animal_generated)
                    {
                        DEBUGGING::DebugConsole::WriteLine("Random animal generated: ", static_cast<unsigned int>(animal->Type.Species));

                        // PLAY THE ANIMAL'S SOUND EFFECT, IF ONE EXISTS.
                        speakers.PlaySoundEffect(animal->SoundId);

                        // START ANIMATING THE ANIMAL.
                        animal->Sprite.Play();

                        // STORE THE ANIMAL IN THE CURRENT TILE MAP.
                        current_tile_map.Animals.push_back(animal);
                    }
                }
            }
        }
        else
        {
            // POSITION THE CAMERA TO FOCUS ON THE CENTER OF THE CURRENT TILE MAP.
            MATH::Vector2f center_world_position = current_tile_map.GetCenterWorldPosition();
            camera.SetCenter(center_world_position);
        }
    }
}
