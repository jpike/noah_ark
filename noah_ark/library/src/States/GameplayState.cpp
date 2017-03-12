#include <iostream>
#include <algorithm>
#include "Collision/CollisionDetectionAlgorithms.h"
#include "Core/NullChecking.h"
#include "Objects/RandomAnimalGenerationAlgorithm.h"
#include "States/GameplayState.h"

namespace STATES
{
    /// Constructor.
    /// @param[in]  assets - The assets to be used during gameplay.
    /// @throws std::exception - Thrown if the assets are null.
    GameplayState::GameplayState(const std::shared_ptr<RESOURCES::Assets>& assets) :
        Overworld(),
        RandomNumberGenerator(),
        Speakers(),
        BibleVersesLeftToFind(),
        Assets(assets),
        Hud()
    {
        // MAKE SURE ASSETS WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Assets, "Assets cannot be null for gameplay state.");
    }

    /// Initializes the gameplay state.
    /// @param[in]  screen_width_in_pixels - The width of the screen, in pixels.
    /// @param[in]  saved_game_data - The saved game data to use to initialize the gameplay state.
    /// @param[in,out]  overworld - The overworld for the gameplay state.
    /// @return True if initialization succeeded; false otherwise.
    bool GameplayState::Initialize(
        const unsigned int screen_width_in_pixels,
        const SavedGameData& saved_game_data,
        const std::shared_ptr<MAPS::Overworld>& overworld)
    {
        // MAKE SURE AN OVERWORLD WAS PROVIDED.
        bool overworld_exists = (nullptr != overworld);
        if (!overworld_exists)
        {
            // The gameplay state requires an overworld.
            return false;
        }

        // INITIALIZE THE OVERWORLD.
        Overworld = overworld;

        // Built ark pieces need to be initialized.
        for (const auto& built_ark_piece_data : saved_game_data.BuildArkPieces)
        {
            // GET THE TILE MAP FOR THE BUILT ARK PIECES.
            MAPS::TileMap* current_tile_map = Overworld->GetTileMap(built_ark_piece_data.TileMapGridYPosition, built_ark_piece_data.TileMapGridXPosition);
            assert(current_tile_map);

            // UPDATE THE BUILT ARK PIECES IN THE CURRENT TILE MAP.
            for (unsigned int ark_piece_index : built_ark_piece_data.BuiltArkPieceIndices)
            {
                // SET THE CURRENT ARK PIECE AS BUILT.
                auto& ark_piece = current_tile_map->ArkPieces.at(ark_piece_index);
                ark_piece.Built = true;
            }
        }

        // INITIALIZE THE PLAYER.
        std::unique_ptr<OBJECTS::Noah> noah_player = InitializePlayer(saved_game_data);
        bool player_initialized = (nullptr != noah_player);
        if (player_initialized)
        {
            Overworld->NoahPlayer = std::move(noah_player);
        }
        else
        {
            // The gameplay state requires a player.
            return false;
        }

        // INITIALIZE THE HUD.
        Hud = InitializeHud(screen_width_in_pixels, Overworld);
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
        Overworld->BackgroundMusic->play();
        
        // INDICATE WHETHER OR NOT INITIALIZATION SUCCEEDED.
        bool initialization_succeeded = hud_initialized;
        return initialization_succeeded;
    }

    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in]  elapsed_time - The amount of time by which to update the game state.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    void GameplayState::Update(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::KeyboardInputController& input_controller,
        GRAPHICS::Camera& camera)
    {
        // UPDATE THE HUD IN RESPONSE TO USER INPUT.
        Hud->RespondToInput(input_controller);

        // CHECK IF A MODAL HUD COMPONENT IS DISPLAYED.
        // If a modal GUI component is displayed, then the regular controls for the player
        // in the world shouldn't work.
        bool modal_hud_components_displayed = Hud->ModalComponentDisplayed();
        if (!modal_hud_components_displayed)
        {
            // UPDATE THE TEXT BOX IF IT IS VISIBLE.
            // If the text box is currently being displayed, then it should capture any user input.
            if (Hud->MainTextBox.IsVisible)
            {
                // UPDATE THE TEXT IN THE TEXT BOX.
                Hud->MainTextBox.Update(elapsed_time);
            }
            else
            {
                // UPDATE THE OVERWORLD.
                std::string message_for_text_box;
                UpdateOverworld(elapsed_time, input_controller, camera, message_for_text_box);

                // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
                bool text_box_message_exists = !message_for_text_box.empty();
                if (text_box_message_exists)
                {
                    Hud->MainTextBox.StartDisplayingText(message_for_text_box);
                }
            }

            // GET THE CURRENT TILE MAP.
            /// @todo   Revisit the structure of the logic here.  There probably shouldn't be
            // so much duplication here, and the separation of what's in "UpdateOverworld"
            // and what logic is here isn't clear.
            // This is needed for updating the tree and dust cloud animations, which should still
            // be updated even if the main text box is displayed.
            MATH::FloatRectangle camera_bounds = camera.ViewBounds;
            MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();
            MAPS::TileMap* current_tile_map = Overworld->GetTileMap(camera_view_center.X, camera_view_center.Y);
            assert(current_tile_map);

            // UPDATE THE CURRENT TILE MAP'S ANIMALS>
            for (auto& animal : current_tile_map->Animals)
            {
                animal->Sprite.Update(elapsed_time);
            }

            // UPDATE THE CURRENT TILE MAP'S TREES.
            for (auto tree = current_tile_map->Trees.begin(); tree != current_tile_map->Trees.end(); ++tree)
            {
                tree->Update(elapsed_time);
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
            Overworld->NoahPlayer->Inventory->Axe->Update(elapsed_time);
        }

        // REMOVE ANY SOUNDS THAT ARE NO LONGER PLAYING.
        // This helps prevent memory usage from growing without bounds.
        Speakers.RemoveCompletedSounds();
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  renderer - The renderer to use for rendering.
    void GameplayState::Render(GRAPHICS::Renderer& renderer)
    {
        renderer.Render(*Overworld);
        Hud->Render(renderer);
    }

    /// Attempts to initialize the player character from saved game data.
    /// @param[in]  saved_game_data - The saved game data from which to initialize the player.
    /// @return The initialized player, if successful; null otherwise.
    std::unique_ptr<OBJECTS::Noah> GameplayState::InitializePlayer(const SavedGameData& saved_game_data)
    {
        // GET THE TEXTURE FOR NOAH.
        std::shared_ptr<GRAPHICS::Texture> noah_texture = Assets->GetTexture(RESOURCES::NOAH_TEXTURE_ID);
        assert(noah_texture);

        // GET THE AXE TEXTURE FOR NOAH.
        std::shared_ptr<GRAPHICS::Texture> axe_texture = Assets->GetTexture(RESOURCES::AXE_TEXTURE_ID);
        assert(axe_texture);

        // GET THE AXE'S SOUND EFFECT.
        std::shared_ptr<AUDIO::SoundEffect> axe_hit_sound = Assets->GetSound(RESOURCES::AXE_HIT_SOUND_ID);
        assert(axe_hit_sound);

        // CREATE THE AXE.
        std::shared_ptr<OBJECTS::Axe> axe = std::make_shared<OBJECTS::Axe>(axe_texture, axe_hit_sound);

        // CREATE NOAH.
        auto noah_player = std::make_unique<OBJECTS::Noah>(noah_texture, axe);

        // SET NOAH'S INITIAL POSITION.
        noah_player->SetWorldPosition(saved_game_data.PlayerWorldPosition);

        // POPULATE THE REST OF NOAH'S INVENTORY.
        noah_player->Inventory->WoodCount = saved_game_data.WoodCount;
        noah_player->Inventory->BibleVerses.insert(saved_game_data.FoundBibleVerses.cbegin(), saved_game_data.FoundBibleVerses.cend());

        return noah_player;
    }

    /// Attempts to initialize the HUD for gameplay.
    /// @param[in]  screen_width_in_pixels - The width of the screen, in pixels.
    /// @param[in]  overworld - The overworld for which the HUD is displaying information about.
    /// @return The initialized HUD, if successful; null otherwise.
    std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> GameplayState::InitializeHud(
        const unsigned int screen_width_in_pixels,
        const std::shared_ptr<MAPS::Overworld>& overworld)
    {
        // GET ASSETS NEEDED FOR THE HUD.
        std::shared_ptr<GRAPHICS::Texture> axe_texture = Assets->GetTexture(RESOURCES::AXE_TEXTURE_ID);
        assert(axe_texture);
        std::shared_ptr<GRAPHICS::Texture> wood_log_texture = Assets->GetTexture(RESOURCES::WOOD_LOG_TEXTURE_ID);
        assert(wood_log_texture);

        // CALCULATE THE TEXT BOX DIMENSIONS.
        unsigned int text_box_width_in_pixels = screen_width_in_pixels;
        const unsigned int LINE_COUNT = 2;
        unsigned int text_box_height_in_pixels = GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS * LINE_COUNT;
        
        // CREATE THE HUD.
        auto hud = std::make_unique<GRAPHICS::GUI::HeadsUpDisplay>(
            overworld,
            text_box_width_in_pixels,
            text_box_height_in_pixels,
            axe_texture,
            wood_log_texture);
        return hud;
    }

    /// Updates the entire overworld for the elapsed time.
    /// @param[in]  elapsed_time - The elapsed time for which to update the world.
    /// @param[in,out]  input_controller - The controller supplying player input.
    /// @param[in,out]  camera - The camera defining the viewable region of the overworld.
    /// @param[out] message_for_text_box - The message to display in the main text box,
    ///     if one needs to start being displayed; empty if no new message needs to be displayed.
    void GameplayState::UpdateOverworld(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::KeyboardInputController& input_controller,
        GRAPHICS::Camera& camera,
        std::string& message_for_text_box)
    {
        // INDICATE THAT NO MESSAGE NEEDS TO BE DISPLAYED YET IN THE TEXT BOX.
        message_for_text_box.clear();

        MATH::FloatRectangle camera_bounds = camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

        MAPS::TileMap* current_tile_map = Overworld->GetTileMap(camera_view_center.X, camera_view_center.Y);
        assert(current_tile_map);

        // CHECK IF THE PRIMARY ACTION BUTTON WAS PRESSED.
        if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::PRIMARY_ACTION_KEY))
        {
            // SWING THE PLAYER'S AXE.
            // A new axe swing may not be created if the player's
            // axe is already being swung.
            std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = Overworld->NoahPlayer->SwingAxe();
            bool axe_swing_occurred = (nullptr != axe_swing);
            if (axe_swing_occurred)
            {
                // Allow the axe to collide with other objects.
                Overworld->AxeSwings.push_back(axe_swing);
            }
        }

        // CHECK IF THE PLAYER IS ALLOWED TO MOVE.
        // Noah can't move while the axe is swinging.
        // Movement is prevented to have the axe's position remain attached to Noah.
        // We need to keep track if Noah moved this frame so that we can stop any walking animations for him if he didn't move.
        bool noah_moved_this_frame = false;
        bool axe_is_swinging = (nullptr != Overworld->NoahPlayer->Inventory->Axe) && Overworld->NoahPlayer->Inventory->Axe->IsSwinging();
        bool player_movement_allowed = (!axe_is_swinging);
        if (player_movement_allowed)
        {
            // MOVE NOAH IN RESPONSE TO USER INPUT.
            const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
            MATH::Vector2f old_noah_position = Overworld->NoahPlayer->GetWorldPosition();
            if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::UP_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING UP.
                Overworld->NoahPlayer->BeginWalking(CORE::Direction::UP, OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    Overworld->NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::UP,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    *Overworld);
                Overworld->NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = Overworld->NoahPlayer->GetWorldBoundingBox();
                float camera_top_y_position = camera_bounds.GetTopYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetTopYPosition() < camera_top_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A TOP TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int top_tile_map_row_index = current_tile_map->OverworldRowIndex - 1;
                    unsigned int top_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                    MAPS::TileMap* top_tile_map = Overworld->GetTileMap(
                        top_tile_map_row_index,
                        top_tile_map_column_index);
                    bool top_tile_map_exists = (nullptr != top_tile_map);
                    if (top_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS UP SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = Overworld->NoahPlayer->GetWorldPosition();
                        noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);

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

                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            }
            if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::DOWN_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING DOWN.
                Overworld->NoahPlayer->BeginWalking(CORE::Direction::DOWN, OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    Overworld->NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::DOWN,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    *Overworld);
                Overworld->NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = Overworld->NoahPlayer->GetWorldBoundingBox();
                float camera_bottom_y_position = camera_bounds.GetBottomYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetBottomYPosition() > camera_bottom_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A BOTTOM TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int bottom_tile_map_row_index = current_tile_map->OverworldRowIndex + 1;
                    unsigned int bottom_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                    MAPS::TileMap* bottom_tile_map = Overworld->GetTileMap(
                        bottom_tile_map_row_index,
                        bottom_tile_map_column_index);
                    bool bottom_tile_map_exists = (nullptr != bottom_tile_map);
                    if (bottom_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS DOWN SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = Overworld->NoahPlayer->GetWorldPosition();
                        noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float noah_half_height = Overworld->NoahPlayer->GetWorldBoundingBox().GetHeight() / 2.0f;
                        noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;

                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::LEFT_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING LEFT.
                Overworld->NoahPlayer->BeginWalking(CORE::Direction::LEFT, OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    Overworld->NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::LEFT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    *Overworld);
                Overworld->NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = Overworld->NoahPlayer->GetWorldBoundingBox();
                float camera_left_x_position = camera_bounds.GetLeftXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetLeftXPosition() < camera_left_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A LEFT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int left_tile_map_row_index = current_tile_map->OverworldRowIndex;
                    unsigned int left_tile_map_column_index = current_tile_map->OverworldColumnIndex - 1;
                    MAPS::TileMap* left_tile_map = Overworld->GetTileMap(
                        left_tile_map_row_index,
                        left_tile_map_column_index);
                    bool left_tile_map_exists = (nullptr != left_tile_map);
                    if (left_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS LEFT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = Overworld->NoahPlayer->GetWorldPosition();
                        noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float noah_half_width = Overworld->NoahPlayer->GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_left_boundary + noah_half_width;

                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
            if (input_controller.ButtonDown(INPUT_CONTROL::KeyboardInputController::RIGHT_KEY))
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // START NOAH WALKING RIGHT.
                Overworld->NoahPlayer->BeginWalking(CORE::Direction::RIGHT, OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    Overworld->NoahPlayer->GetWorldBoundingBox(),
                    CORE::Direction::RIGHT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    elapsed_time,
                    *Overworld);
                Overworld->NoahPlayer->SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = Overworld->NoahPlayer->GetWorldBoundingBox();
                float camera_right_x_position = camera_bounds.GetRightXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetRightXPosition() > camera_right_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A RIGHT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int right_tile_map_row_index = current_tile_map->OverworldRowIndex;
                    unsigned int right_tile_map_column_index = current_tile_map->OverworldColumnIndex + 1;
                    MAPS::TileMap* right_tile_map = Overworld->GetTileMap(
                        right_tile_map_row_index,
                        right_tile_map_column_index);
                    bool right_tile_map_exists = (nullptr != right_tile_map);
                    if (right_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS RIGHT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = Overworld->NoahPlayer->GetWorldPosition();
                        noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);

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
                        float noah_half_width = Overworld->NoahPlayer->GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_right_boundary - noah_half_width;

                        Overworld->NoahPlayer->SetWorldPosition(noah_world_position);
                    }
                }
            } 
        }

        // CHECK IF NOAH MOVED THIS FRAME.
        if (noah_moved_this_frame)
        {
            // UPDATE NOAH'S ANIMATION.
            Overworld->NoahPlayer->Sprite.Update(elapsed_time);

            // BUILD A PIECE OF THE ARK IF NOAH STEPPED ONTO AN APPROPRIATE SPOT.
            MATH::Vector2f noah_world_position = Overworld->NoahPlayer->GetWorldPosition();
            MAPS::TileMap* tile_map_underneath_noah = Overworld->GetTileMap(noah_world_position.X, noah_world_position.Y);
            assert(tile_map_underneath_noah);
            
            // An ark piece only needs to be built once and requires wood to be built.
            OBJECTS::ArkPiece* ark_piece = tile_map_underneath_noah->GetArkPieceAtWorldPosition(noah_world_position);
            bool noah_has_wood = (Overworld->NoahPlayer->Inventory->WoodCount > 0);
            bool ark_piece_can_be_built = (ark_piece && !ark_piece->Built && noah_has_wood);
            if (ark_piece_can_be_built)
            {
                // USE THE WOOD FROM NOAH'S INVENTORY.
                --Overworld->NoahPlayer->Inventory->WoodCount;

                // BUILD THE ARK PIECE.
                ark_piece->Built = true;

                // When building an ark piece, a dust cloud should appear.
                std::shared_ptr<GRAPHICS::Texture> dust_cloud_texture = Assets->GetTexture(RESOURCES::DUST_CLOUD_TEXTURE_ID);
                assert(dust_cloud_texture);
                OBJECTS::DustCloud dust_cloud(dust_cloud_texture);

                // The dust cloud should be positioned over the ark piece.
                MATH::Vector2f dust_cloud_center_world_position = ark_piece->Sprite.GetWorldPosition();
                dust_cloud.Sprite.SetWorldPosition(dust_cloud_center_world_position);

                // The dust cloud should start animating immediately.
                dust_cloud.Sprite.Play();

                // The dust cloud needs to be added to the tile map so that it gets updated.
                tile_map_underneath_noah->DustClouds.push_back(dust_cloud);

                // Play a sound to indicate a piece of the ark is being built.
                std::shared_ptr<AUDIO::SoundEffect> axe_building_sound = Assets->GetSound(RESOURCES::ARK_BUILDING_SOUND_ID);
                assert(axe_building_sound);
                Speakers.Play(axe_building_sound);
            }
        }
        else
        {
            // STOP NOAH'S ANIMATION FROM PLAYING SINCE THE PLAYER DIDN'T MOVE THIS FRAME.
            Overworld->NoahPlayer->Sprite.ResetAnimation();
        }

        // UPDATE THE CAMERA BASED ON SCROLLING.
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
                    std::cout << "Generating random animal..." << std::endl;
                    std::cout << "Tile map column: " << current_tile_map->OverworldColumnIndex << " row: " << current_tile_map->OverworldRowIndex << std::endl;
                    auto tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                    std::cout << "Tile map LTRB: "
                        << tile_map_bounding_box.GetLeftXPosition() << " "
                        << tile_map_bounding_box.GetTopYPosition() << " "
                        << tile_map_bounding_box.GetRightXPosition() << " "
                        << tile_map_bounding_box.GetBottomYPosition() << std::endl;
                    // GENERATE A RANDOM ANIMAL IN THE CURRENT TILE MAP.
                    std::shared_ptr<OBJECTS::Animal> animal = OBJECTS::RandomAnimalGenerationAlgorithm::GenerateAnimal(
                        *Overworld->NoahPlayer->Inventory,
                        *current_tile_map,
                        RandomNumberGenerator,
                        *Assets);
                    bool animal_generated = (nullptr != animal);
                    if (animal_generated)
                    {
                        std::cout << "Random animal generated: " << static_cast<unsigned int>(animal->Type.Species) << std::endl;
                        animal->Sprite.Play();
                        current_tile_map->Animals.push_back(animal);
                    }
                }
            }
        }
        else
        {
            // POSITION THE CAMERA TO FOCUS ON THE CENTER OF THE CURRENT TILE MAP.
            MATH::Vector2f center_world_position = current_tile_map->GetCenterWorldPosition();
            camera.SetCenter(center_world_position);
        }

        // HANDLE OTHER COLLISIONS.
        COLLISION::CollisionDetectionAlgorithms::HandleAxeSwings(*Overworld, Overworld->AxeSwings, *Assets);
        for (auto wood_logs = current_tile_map->WoodLogs.begin(); wood_logs != current_tile_map->WoodLogs.end();)
        {
            // CHECK IF THE WOOD LOGS INTERSECT WITH NOAH.
            MATH::FloatRectangle wood_log_bounding_box = wood_logs->GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = Overworld->NoahPlayer->GetWorldBoundingBox();
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
                Overworld->NoahPlayer->Inventory->AddWood(random_wood_count);

                // REMOVE THE WOOD LOGS SINCE THEY'VE BEEN COLLECTED BY NOAH.
                wood_logs = current_tile_map->WoodLogs.erase(wood_logs);

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
                        std::shared_ptr<AUDIO::SoundEffect> collected_bible_verse_sound = Assets->GetSound(RESOURCES::COLLECT_BIBLE_VERSE_SOUND_ID);
                        assert(collected_bible_verse_sound);
                        Speakers.Play(collected_bible_verse_sound);

                        // SELECT A RANDOM BIBLE VERSE.
                        unsigned int remaining_bible_verse_count = BibleVersesLeftToFind.size();
                        unsigned int random_bible_verse_index = RandomNumberGenerator.RandomNumberLessThan(remaining_bible_verse_count);
                        auto bible_verse = BibleVersesLeftToFind.begin() + random_bible_verse_index;

                        // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                        Overworld->NoahPlayer->Inventory->BibleVerses.insert(*bible_verse);

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
}
