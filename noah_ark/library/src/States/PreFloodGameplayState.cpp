#include <algorithm>
#include "Collision/CollisionDetectionAlgorithms.h"
#include "Core/NullChecking.h"
#include "Debugging/DebugConsole.h"
#include "Objects/RandomAnimalGenerationAlgorithm.h"
#include "States/PreFloodGameplayState.h"

namespace STATES
{
    /// Constructor.
    /// @param[in,out]  speakers - The speakers for which to play sound effects.
    /// @param[in]  assets - The assets to be used during gameplay.
    /// @throws std::exception - Thrown if the assets are null.
    PreFloodGameplayState::PreFloodGameplayState(
        const std::shared_ptr<AUDIO::Speakers>& speakers,
        const std::shared_ptr<RESOURCES::Assets>& assets) :
        World(),
        NoahPlayer(),
        RandomNumberGenerator(),
        Speakers(speakers),
        BibleVersesLeftToFind(),
        Assets(assets),
        Hud(),
        CurrentMapGrid(nullptr),
        TileMapEditorGui(assets->GetTexture(RESOURCES::MAIN_TILESET_TEXTURE_ID))
    {
        // MAKE SURE PARAMETERS WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Speakers, "Speakers cannot be null for gameplay state.");
        CORE::ThrowInvalidArgumentExceptionIfNull(Assets, "Assets cannot be null for gameplay state.");
    }

    /// Initializes the gameplay state.
    /// @param[in]  screen_width_in_pixels - The width of the screen, in pixels.
    /// @param[in]  saved_game_data - The saved game data to use to initialize the gameplay state.
    /// @param[in,out]  world - The world for the gameplay state.
    /// @return True if initialization succeeded; false otherwise.
    bool PreFloodGameplayState::Initialize(
        const unsigned int screen_width_in_pixels,
        const SavedGameData& saved_game_data,
        const std::shared_ptr<MAPS::World>& world)
    {
        // MAKE SURE A WORLD WAS PROVIDED.
        bool world_exists = (nullptr != world);
        if (!world_exists)
        {
            // The gameplay state requires a world.
            return false;
        }

        // INITIALIZE THE WORLD.
        World = world;
        CurrentMapGrid = &World->Overworld;

        // Built ark pieces need to be initialized.
        for (const auto& built_ark_piece_data : saved_game_data.BuildArkPieces)
        {
            // GET THE TILE MAP FOR THE BUILT ARK PIECES.
            MAPS::TileMap* current_tile_map = World->Overworld.GetTileMap(built_ark_piece_data.TileMapGridYPosition, built_ark_piece_data.TileMapGridXPosition);
            if (!current_tile_map)
            {
                // The game can't be properly initialized if a tile map is missing.
                return false;
            }

            // UPDATE THE BUILT ARK PIECES IN THE CURRENT TILE MAP.
            for (size_t ark_piece_index : built_ark_piece_data.BuiltArkPieceIndices)
            {
                // SET THE CURRENT ARK PIECE AS BUILT.
                auto& ark_piece = current_tile_map->ArkPieces.at(ark_piece_index);
                ark_piece.Built = true;
            }
        }

        // INITIALIZE THE PLAYER.
        NoahPlayer = InitializePlayer(saved_game_data);
        bool player_initialized = (nullptr != NoahPlayer);
        if (!player_initialized)
        {
            // The gameplay state requires a player.
            return false;
        }

        // INITIALIZE THE HUD.
        Hud = InitializeHud(screen_width_in_pixels, World, NoahPlayer);
        bool hud_initialized = (nullptr != Hud);
        if (!hud_initialized)
        {
            // The gameplay state requires a HUD.
            return false;
        }

        // INITIALIZE THE BIBLE VERSES LEFT TO FIND.
        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            saved_game_data.FoundBibleVerses.cbegin(),
            saved_game_data.FoundBibleVerses.cend(),
            std::inserter(BibleVersesLeftToFind, BibleVersesLeftToFind.begin()));

        // START PLAYING THE BACKGROUND MUSIC.
        Speakers->PlayMusic(RESOURCES::OVERWORLD_BACKGROUND_MUSIC_ID);

        // INDICATE WHETHER OR NOT INITIALIZATION SUCCEEDED.
        bool initialization_succeeded = hud_initialized;
        return initialization_succeeded;
    }

    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in]  elapsed_time - The amount of time by which to update the game state.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    void PreFloodGameplayState::Update(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        GRAPHICS::Camera& camera)
    {
#ifdef _DEBUG
        // UPDATE THE TILE MAP EDITOR IN RESPONSE TO USER INPUT.
        TileMapEditorGui.RespondToInput(input_controller);
        if (TileMapEditorGui.Visible)
        {
            // MAKE SURE THE TILE MAP EDITOR GUI HAS THE CURRENT TILE MAP.
            MATH::FloatRectangle camera_bounds = camera.ViewBounds;
            MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();
            MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
            TileMapEditorGui.CurrentTileMap = current_tile_map;
            
            // FINISH UPDATING.
            // If the tile map editor is displayed, it should have
            // full control over updating to avoid interference
            // by other components.
            return;
        }
        else
        {
            // CLEAR THE TILE MAP EDITOR GUI'S CURRENT TILE MAP.
            TileMapEditorGui.CurrentTileMap = nullptr;
        }
#endif

        // UPDATE THE HUD IN RESPONSE TO USER INPUT.
        Hud->RespondToInput(input_controller);

        // CHECK IF A MODAL HUD COMPONENT IS DISPLAYED.
        // If a modal GUI component is displayed, then the regular controls for the player
        // in the world shouldn't work.
        bool modal_hud_components_displayed = Hud->ModalComponentDisplayed();
        if (modal_hud_components_displayed)
        {
            // No further updating is needed.
            return;
        }

        // UPDATE THE CURRENT MAP GRID.
        UpdateMapGrid(elapsed_time, input_controller, camera, *CurrentMapGrid);
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  renderer - The renderer to use for rendering.
    void PreFloodGameplayState::Render(GRAPHICS::Renderer& renderer)
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
            // RENDER THE PLAYER.
            NoahPlayer->Sprite.Render(renderer.Screen);

            // The axe should only be rendered if it is swinging.
            if (NoahPlayer->Inventory->Axe->IsSwinging())
            {
                NoahPlayer->Inventory->Axe->Sprite.Render(renderer.Screen);
            }

            // RENDER THE HUD.
            Hud->Render(renderer);
        }
    }

    /// Attempts to initialize the player character from saved game data.
    /// @param[in]  saved_game_data - The saved game data from which to initialize the player.
    /// @return The initialized player, if successful; null otherwise.
    std::shared_ptr<OBJECTS::Noah> PreFloodGameplayState::InitializePlayer(const SavedGameData& saved_game_data)
    {
        // GET THE TEXTURE FOR NOAH.
        std::shared_ptr<GRAPHICS::Texture> noah_texture = Assets->GetTexture(RESOURCES::NOAH_TEXTURE_ID);
        if (!noah_texture)
        {
            return nullptr;
        }

        // GET THE AXE TEXTURE FOR NOAH.
        std::shared_ptr<GRAPHICS::Texture> axe_texture = Assets->GetTexture(RESOURCES::AXE_TEXTURE_ID);
        if (!axe_texture)
        {
            return nullptr;
        }

        // CREATE THE AXE.
        std::shared_ptr<OBJECTS::Axe> axe = std::make_shared<OBJECTS::Axe>(axe_texture);

        // CREATE NOAH.
        auto noah_player = std::make_shared<OBJECTS::Noah>(noah_texture, axe);

        // SET NOAH'S INITIAL POSITION.
        noah_player->SetWorldPosition(saved_game_data.PlayerWorldPosition);

        // POPULATE THE REST OF NOAH'S INVENTORY.
        noah_player->Inventory->WoodCount = saved_game_data.WoodCount;
        noah_player->Inventory->BibleVerses.insert(saved_game_data.FoundBibleVerses.cbegin(), saved_game_data.FoundBibleVerses.cend());
        noah_player->Inventory->CollectedAnimalCounts = saved_game_data.CollectedAnimals;
        noah_player->Inventory->CollectedFoodCounts = saved_game_data.CollectedFood;

        return noah_player;
    }

    /// Attempts to initialize the HUD for gameplay.
    /// @param[in]  screen_width_in_pixels - The width of the screen, in pixels.
    /// @param[in]  world - The world for which the HUD is displaying information about.
    /// @param[in]  noah_player - The player whose information to display in the HUD.
    /// @return The initialized HUD, if successful; null otherwise.
    std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> PreFloodGameplayState::InitializeHud(
        const unsigned int screen_width_in_pixels,
        const std::shared_ptr<MAPS::World>& world,
        const std::shared_ptr<OBJECTS::Noah>& noah_player)
    {
        // GET ASSETS NEEDED FOR THE HUD.
        std::shared_ptr<GRAPHICS::Texture> axe_texture = Assets->GetTexture(RESOURCES::AXE_TEXTURE_ID);
        if (!axe_texture)
        {
            return nullptr;
        }

        std::shared_ptr<GRAPHICS::Texture> wood_log_texture = Assets->GetTexture(RESOURCES::WOOD_LOG_TEXTURE_ID);
        if (!wood_log_texture)
        {
            return nullptr;
        }

        // CALCULATE THE TEXT BOX DIMENSIONS.
        unsigned int text_box_width_in_pixels = screen_width_in_pixels;
        const unsigned int LINE_COUNT = 2;
        unsigned int text_box_height_in_pixels = GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS * LINE_COUNT;

        // CREATE THE HUD.
        auto hud = std::make_unique<GRAPHICS::GUI::HeadsUpDisplay>(
            world,
            noah_player,
            text_box_width_in_pixels,
            text_box_height_in_pixels,
            Assets);
        return hud;
    }

    /// Updates a map grid based on elapsed time and player input.
    /// @param[in]  elapsed_time - The amount of time by which to update the game state.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @param[in,out]  map_grid - The map grid to update.
    void PreFloodGameplayState::UpdateMapGrid(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        GRAPHICS::Camera& camera,
        MAPS::MultiTileMapGrid& map_grid)
    {
        // GET THE CURRENT TILE MAP.
        MATH::FloatRectangle camera_bounds = camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();
        MAPS::TileMap* current_tile_map = map_grid.GetTileMap(camera_view_center.X, camera_view_center.Y);
        if (!current_tile_map)
        {
            // A current tile map is required for updating.
            return;
        }

        // UPDATE THE TEXT BOX IF IT IS VISIBLE.
        // If the text box is currently being displayed, then it should capture any user input.
        if (Hud->MainTextBox.IsVisible)
        {
            // UPDATE THE TEXT IN THE TEXT BOX.
            Hud->MainTextBox.Update(elapsed_time);
        }
        else
        {
            // UPDATE THE PLAYER BASED ON INPUT.
            MAPS::ExitPoint* map_exit_point = UpdatePlayerBasedOnInput(
                elapsed_time, 
                input_controller, 
                *current_tile_map, 
                map_grid, 
                camera);
            if (map_exit_point)
            {
                // SWITCH OVER TO THE NEW MAP GRID.
                CurrentMapGrid = map_exit_point->NewMapGrid;

                // UPDATE THE CAMERA TO FOCUS ON THE NEW TILE MAP.
                MATH::Vector2f center_world_position = map_exit_point->NewTileMap->GetCenterWorldPosition();
                camera.SetCenter(center_world_position);

                // MOVE THE PLAYER TO THE START POINT OF THE NEW TILE MAP.
                NoahPlayer->SetWorldPosition(map_exit_point->NewPlayerWorldPosition);

                // EXIT THIS UPDATE IF THE PLAYER HAS CHANGED MAPS.
                return;
            }

            // MOVE ANIMALS IN THE WORLD.
            MoveAnimals(elapsed_time, *current_tile_map, map_grid);

            // UPDATE FOOD FALLING IN THE WORLD.
            UpdateFallingFood(elapsed_time, *current_tile_map);

            // HANDLE PLAYER COLLISIONS.
            std::string message_for_text_box;
            CollectWoodAndBibleVersesCollidingWithPlayer(*current_tile_map, map_grid, message_for_text_box);
            CollectFoodCollidingWithPlayer(*current_tile_map);
            CollectAnimalsCollidingWithPlayer(*current_tile_map);

            // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
            bool text_box_message_exists = !message_for_text_box.empty();
            if (text_box_message_exists)
            {
                Hud->MainTextBox.StartDisplayingText(message_for_text_box);
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
            if (tree->Shaking)
            {
                // ONLY START PLAYING THE SOUND IF IT ISN'T ALREADY PLAYING.
                // This results in a smoother sound experience.
                bool tree_shake_sound_playing = Speakers->SoundIsPlaying(RESOURCES::TREE_SHAKE_SOUND_ID);
                if (!tree_shake_sound_playing)
                {
                    Speakers->PlaySound(RESOURCES::TREE_SHAKE_SOUND_ID);
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
        NoahPlayer->Inventory->Axe->Update(elapsed_time);

        // UPDATE THE CAMERA'S WORLD VIEW.
        UpdateCameraWorldView(
            elapsed_time,
            camera,
            input_controller,
            *current_tile_map);
    }

    /// Updates the player and related items in the tile map based on input and elapsed time.
    /// @param[in]  elapsed_time - The elapsed time for which to update things.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  current_tile_map - The tile map the player is currently located in.
    /// @param[in,out]  map_grid - The map grid containing the current tile map.
    /// @param[in,out]  camera - The camera defining the viewable region of the map grid.
    /// @return The map exit point, if the player stepped on such a point.
    MAPS::ExitPoint* PreFloodGameplayState::UpdatePlayerBasedOnInput(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::TileMap& current_tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera)
    {
        MATH::FloatRectangle camera_bounds = camera.ViewBounds;

        // CHECK IF THE PRIMARY ACTION BUTTON WAS PRESSED.
        if (input_controller.ButtonDown(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
        {
            // SWING THE PLAYER'S AXE.
            // A new axe swing may not be created if the player's
            // axe is already being swung.
            std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = NoahPlayer->SwingAxe();
            bool axe_swing_occurred = (nullptr != axe_swing);
            if (axe_swing_occurred)
            {
                // Allow the axe to collide with other objects.
                map_grid.AxeSwings.push_back(axe_swing);
            }
        }

        // GET THE TILE UNDER NOAH.
        // This is needed to help track if Noah moves onto a different type of tile.
        MATH::Vector2f old_noah_position = NoahPlayer->GetWorldPosition();
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
        bool axe_is_swinging = (nullptr != NoahPlayer->Inventory->Axe) && NoahPlayer->Inventory->Axe->IsSwinging();
        bool player_movement_allowed = (!axe_is_swinging);
        if (player_movement_allowed)
        {
            // MOVE NOAH IN RESPONSE TO USER INPUT.
            const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
            if (input_controller.ButtonDown(INPUT_CONTROL::InputController::UP_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING UP.
                NoahPlayer->BeginWalking(CORE::Direction::UP, OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::UP,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer->GetWorldBoundingBox();
                float camera_top_y_position = camera_bounds.GetTopYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetTopYPosition() < camera_top_y_position);
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
                        MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
                        noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float tile_map_top_boundary = tile_map_bounding_box.GetTopYPosition();

                        // To keep Noah completely on screen, his center position should be half
                        // his height below the top tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_height = noah_world_bounding_box.GetHeight() / 2.0f;
                        noah_world_position.Y = tile_map_top_boundary + noah_half_height;

                        NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            }
            if (input_controller.ButtonDown(INPUT_CONTROL::InputController::DOWN_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING DOWN.
                NoahPlayer->BeginWalking(CORE::Direction::DOWN, OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::DOWN,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer->GetWorldBoundingBox();
                float camera_bottom_y_position = camera_bounds.GetBottomYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetBottomYPosition() > camera_bottom_y_position);
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
                        MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
                        noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float tile_map_bottom_boundary = tile_map_bounding_box.GetBottomYPosition();

                        // To keep Noah completely on screen, his center position should be half
                        // his height above the bottom tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_height = NoahPlayer->GetWorldBoundingBox().GetHeight() / 2.0f;
                        noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;

                        NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(INPUT_CONTROL::InputController::LEFT_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING LEFT.
                NoahPlayer->BeginWalking(CORE::Direction::LEFT, OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::LEFT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer->GetWorldBoundingBox();
                float camera_left_x_position = camera_bounds.GetLeftXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetLeftXPosition() < camera_left_x_position);
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
                        MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
                        noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float tile_map_left_boundary = tile_map_bounding_box.GetLeftXPosition();

                        // To keep Noah completely on screen, his center position should be half
                        // his width to the right of the left tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_width = NoahPlayer->GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_left_boundary + noah_half_width;

                        NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(INPUT_CONTROL::InputController::RIGHT_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING RIGHT.
                NoahPlayer->BeginWalking(CORE::Direction::RIGHT, OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::RIGHT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    map_grid);
                NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer->GetWorldBoundingBox();
                float camera_right_x_position = camera_bounds.GetRightXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetRightXPosition() > camera_right_x_position);
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
                        MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
                        noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float tile_map_right_boundary = tile_map_bounding_box.GetRightXPosition();

                        // To keep Noah completely on screen, his center position should be half
                        // his width to the left of the right tile map boundary.
                        MATH::Vector2f noah_world_position = old_noah_position;
                        float noah_half_width = NoahPlayer->GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_right_boundary - noah_half_width;

                        NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
        }

        // CHECK IF NOAH MOVED THIS FRAME.
        if (noah_moved_this_frame)
        {
            // UPDATE NOAH'S ANIMATION.
            NoahPlayer->Sprite.Update(elapsed_time);

            // BUILD A PIECE OF THE ARK IF NOAH STEPPED ONTO AN APPROPRIATE SPOT.
            MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
            MAPS::TileMap* tile_map_underneath_noah = map_grid.GetTileMap(noah_world_position.X, noah_world_position.Y);
            if (!tile_map_underneath_noah)
            {
                // If no tile map exists, then Noah couldn't have stepped on an exit point.
                return nullptr;
            }
            
            // An ark piece only needs to be built once and requires wood to be built.
            OBJECTS::ArkPiece* ark_piece = tile_map_underneath_noah->GetArkPieceAtWorldPosition(noah_world_position);
            bool noah_has_wood = (NoahPlayer->Inventory->WoodCount > 0);
            bool ark_piece_can_be_built = (ark_piece && !ark_piece->Built && noah_has_wood);
            if (ark_piece_can_be_built)
            {
                // USE THE WOOD FROM NOAH'S INVENTORY.
                --NoahPlayer->Inventory->WoodCount;

                // BUILD THE ARK PIECE.
                ark_piece->Built = true;

                // When building an ark piece, a dust cloud should appear.
                std::shared_ptr<GRAPHICS::Texture> dust_cloud_texture = Assets->GetTexture(RESOURCES::DUST_CLOUD_TEXTURE_ID);
                if (dust_cloud_texture)
                {
                    OBJECTS::DustCloud dust_cloud(dust_cloud_texture);

                    // The dust cloud should be positioned over the ark piece.
                    MATH::Vector2f dust_cloud_center_world_position = ark_piece->Sprite.GetWorldPosition();
                    dust_cloud.Sprite.SetWorldPosition(dust_cloud_center_world_position);

                    // The dust cloud should start animating immediately.
                    dust_cloud.Sprite.Play();

                    // The dust cloud needs to be added to the tile map so that it gets updated.
                    tile_map_underneath_noah->DustClouds.push_back(dust_cloud);
                }

                // Play a sound to indicate a piece of the ark is being built.
                Speakers->PlaySound(RESOURCES::ARK_BUILDING_SOUND_ID);
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
            NoahPlayer->Sprite.ResetAnimation();
        }

        // INDICATE THAT THE PLAYER DIDN'T STEP ON AN EXIT POINT.
        return nullptr;
    }

    /// Moves animals in the provided tile map based on the elapsed time.
    /// @param[in]  elapsed_time - The elapsed time for which to move the animals.
    /// @param[in,out]  tile_map - The tile map in which to move animals.
    /// @param[in,out]  map_grid - The map grid containing the tile map.
    void PreFloodGameplayState::MoveAnimals(const sf::Time& elapsed_time, MAPS::TileMap& tile_map, MAPS::MultiTileMapGrid& map_grid)
    {
        // MOVE EACH ANIMAL IN THE TILE MAP CLOSER TO NOAH.
        for (auto& animal : tile_map.Animals)
        {
            // DETERMINE THE DIRECTION FROM THE ANIMAL TO THE PLAYER.
            // The animal should move closer to Noah based on Genesis 6:20.
            MATH::Vector2f noah_world_position = NoahPlayer->GetWorldPosition();
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
                tile_types_allowed_to_move_over.emplace(MAPS::TileType::WATER);
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

    /// Updates any falling food in the tile map.
    /// @param[in]  elapsed_time - The elapsed time for which to update the food.
    /// @param[in,out]  tile_map - The tile map whose falling food to update.
    void PreFloodGameplayState::UpdateFallingFood(const sf::Time& elapsed_time, MAPS::TileMap& tile_map)
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
    /// @param[out] message_for_text_box - A message for the HUD's main text box, if
    ///     a Bible verse was collected.
    void PreFloodGameplayState::CollectWoodAndBibleVersesCollidingWithPlayer(
        MAPS::TileMap& tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        std::string& message_for_text_box)
    {
        // INDICATE THAT NO MESSAGE EXISTS FOR THE TEXT BOX YET.
        message_for_text_box.clear();

        // HANDLE PLAYER COLLISIONS WITH WOOD LOGS.
        COLLISION::CollisionDetectionAlgorithms::HandleAxeSwings(map_grid, map_grid.AxeSwings, *Speakers, *Assets);
        for (auto wood_logs = tile_map.WoodLogs.begin(); wood_logs != tile_map.WoodLogs.end();)
        {
            // CHECK IF THE WOOD LOGS INTERSECT WITH NOAH.
            MATH::FloatRectangle wood_log_bounding_box = wood_logs->GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = NoahPlayer->GetWorldBoundingBox();
            bool noah_collided_with_wood_logs = noah_bounding_box.Contains(
                wood_log_bounding_box.GetCenterXPosition(),
                wood_log_bounding_box.GetCenterYPosition());
            if (noah_collided_with_wood_logs)
            {
                // ADD THE WOOD TO NOAH'S INVENTORY.
                // The logs can have a random amount of wood.
                unsigned int MIN_WOOD_COUNT = 1;
                unsigned int MAX_WOOD_COUNT = 3;
                unsigned int random_wood_count = RandomNumberGenerator.RandomInRange<unsigned int>(MIN_WOOD_COUNT, MAX_WOOD_COUNT);
                NoahPlayer->Inventory->AddWood(random_wood_count);

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
                        Speakers->PlaySound(RESOURCES::COLLECT_BIBLE_VERSE_SOUND_ID);

                        // SELECT A RANDOM BIBLE VERSE.
                        size_t remaining_bible_verse_count = BibleVersesLeftToFind.size();
                        size_t random_bible_verse_index = RandomNumberGenerator.RandomNumberLessThan(remaining_bible_verse_count);
                        auto bible_verse = BibleVersesLeftToFind.begin() + random_bible_verse_index;

                        // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                        NoahPlayer->Inventory->BibleVerses.insert(*bible_verse);

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
    /// @param[in,out]  tile_map - The tile map to examine food in.
    void PreFloodGameplayState::CollectFoodCollidingWithPlayer(MAPS::TileMap& tile_map)
    {
        // HANDLE PLAYER COLLISIONS WITH FOOD.
        for (auto food = tile_map.FoodOnGround.cbegin();
            food != tile_map.FoodOnGround.cend();)
        {
            // CHECK IF THE CURRENT FOOD ITEM INTERSECTS WITH THE PLAYER.
            MATH::FloatRectangle food_bounding_box = food->Sprite.GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = NoahPlayer->GetWorldBoundingBox();
            bool food_intersects_with_noah = food_bounding_box.Intersects(noah_bounding_box);
            if (food_intersects_with_noah)
            {
                // PLAY THE SOUND EFFECT FOR COLLECTING FOOD.
                Speakers->PlaySound(RESOURCES::FOOD_PICKUP_SOUND_ID);

                // ADD THE FOOD TO THE PLAYER'S INVENTORY.
                DEBUGGING::DebugConsole::WriteLine("Collected food: ", static_cast<int>(food->Type));
                NoahPlayer->Inventory->AddFood(*food);

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
    /// @param[in,out]  tile_map - The tile map to examine animals in.
    void PreFloodGameplayState::CollectAnimalsCollidingWithPlayer(MAPS::TileMap& tile_map)
    {
        // HANDLE PLAYER COLLISIONS WITH ANIMALS.
        for (auto animal = tile_map.Animals.cbegin();
            animal != tile_map.Animals.cend();)
        {
            // CHECK IF THE CURRENT ANIMAL INTERSECTS WITH THE PLAYER.
            MATH::FloatRectangle animal_bounding_box = (*animal)->Sprite.GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = NoahPlayer->GetWorldBoundingBox();
            bool animal_intersects_with_noah = animal_bounding_box.Intersects(noah_bounding_box);
            if (animal_intersects_with_noah)
            {
                // PLAY THE ANIMAL'S SOUND EFFECT, IF ONE EXISTS.
                Speakers->PlaySound((*animal)->SoundId);

                // ADD THE ANIMAL TO THE PLAYER'S INVENTORY.
                DEBUGGING::DebugConsole::WriteLine("Collected animal.");
                NoahPlayer->Inventory->AddAnimal(*animal);

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
    /// @param[in,out]  camera - The camera to update.
    /// @param[in,out]  input_controller - The input controller that might
    ///     be tweaked based on camera movement.
    /// @param[in,out]  current_tile_map - The current tile map in view by the camera.
    void PreFloodGameplayState::UpdateCameraWorldView(
        const sf::Time& elapsed_time,
        GRAPHICS::Camera& camera,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::TileMap& current_tile_map)
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
                const unsigned int EVENLY_DIVISIBLE = 0;
                const unsigned int GENERATE_RANDOM_ANIMAL_IF_DIVISIBLE_BY_THIS = 2;
                unsigned int random_number_for_animal_generation = RandomNumberGenerator.RandomNumber<unsigned int>();
                bool random_animal_should_be_generated = (random_number_for_animal_generation % GENERATE_RANDOM_ANIMAL_IF_DIVISIBLE_BY_THIS) == EVENLY_DIVISIBLE;
                if (random_animal_should_be_generated)
                {
                    DEBUGGING::DebugConsole::WriteLine("Generating random animal...");
                    DEBUGGING::DebugConsole::WriteLine("Tile map column, row: ", MATH::Vector2ui(current_tile_map.GridColumnIndex, current_tile_map.GridRowIndex));
                    auto tile_map_bounding_box = current_tile_map.GetWorldBoundingBox();
                    DEBUGGING::DebugConsole::WriteLine("Tile map LTRB: ", tile_map_bounding_box);

                    // GENERATE A RANDOM ANIMAL IN THE CURRENT TILE MAP.
                    std::shared_ptr<OBJECTS::Animal> animal = OBJECTS::RandomAnimalGenerationAlgorithm::GenerateAnimal(
                        *NoahPlayer,
                        current_tile_map,
                        RandomNumberGenerator,
                        *Assets);
                    bool animal_generated = (nullptr != animal);
                    if (animal_generated)
                    {
                        DEBUGGING::DebugConsole::WriteLine("Random animal generated: ", static_cast<unsigned int>(animal->Type.Species));

                        // PLAY THE ANIMAL'S SOUND EFFECT, IF ONE EXISTS.
                        Speakers->PlaySound(animal->SoundId);

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
