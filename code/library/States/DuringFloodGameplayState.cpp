#include "Collision/CollisionDetectionAlgorithms.h"
#include "States/DuringFloodGameplayState.h"

namespace STATES
{
    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output for the update.
    /// @param[in,out]  world - The world to update based on gameplay.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @return The next game state after updating.
    GameState DuringFloodGameplayState::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        STATES::SavedGameData& current_game_data)
    {
        // UPDATE THE HUD.
        // As of now, the HUD is capable of altering the gameplay state.
        GameState next_game_state = Hud.Update(current_game_data, gaming_hardware);

        // CHECK IF A MODAL HUD COMPONENT IS DISPLAYED.
        // If a modal GUI component is displayed, then the regular controls for the player
        // in the world shouldn't work.
        bool modal_hud_components_displayed = Hud.ModalComponentDisplayed();
        if (modal_hud_components_displayed)
        {
            // No further updating is needed.
            return next_game_state;
        }

        // UPDATE THE CURRENT MAP GRID.
        UpdateMapGrid(
            gaming_hardware,
            world,
            *CurrentMapGrid,
            camera,
            current_game_data);

        // START PLAYING THE BACKGROUND MUSIC IF ITS NOT ALREADY PLAYING.
        /// @todo   Background music?

        // RETURN THE NEXT GAME STATE.
        return next_game_state;
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  world - The world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered gameplay state.
    sf::Sprite DuringFloodGameplayState::Render(
        MAPS::World& world,
        STATES::SavedGameData& current_game_data,
        GRAPHICS::Renderer& renderer)
    {
        // RENDER CONTENT SPECIFIC TO THE CURRENT MAP.
        renderer.Render(*CurrentMapGrid);

        // RENDER THE PLAYER.
        renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);

        // RENDER THE HUD.
        // The text color should differ based on where the player is located.
        // White is more readable on-top of the black borders around the ark interior.
        const GRAPHICS::Color HUD_TEXT_COLOR = GRAPHICS::Color::WHITE;
        /// @todo   Remove axe from HUD?
        Hud.Render(current_game_data, HUD_TEXT_COLOR, renderer);

        // RENDER THE FINAL SCREEN WITH TIME-OF-DAY LIGHTING.
        /// @todo   Darker due to flood?
        sf::Sprite screen = renderer.RenderFinalScreenWithTimeOfDayShading(); 
        return screen;
    }

    /// Updates a map grid based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output.
    /// @param[in,out]  world - The world being updated.
    /// @param[in,out]  map_grid - The map grid to update.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @param[in,out]  current_game_data - The current game data to use and update.
    void DuringFloodGameplayState::UpdateMapGrid(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera,
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

        // MOVE OBJECTS IF POSSIBLE.
        // If the main text box is displaying text, then no objects should move
        // to avoid having the player's gameplay hindered.
        bool objects_can_move = !Hud.MainTextBox.IsVisible;
        if (objects_can_move)
        {
            // UPDATE THE PLAYER BASED ON INPUT.
            MAPS::ExitPoint* map_exit_point = UpdatePlayerBasedOnInput(
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
                gaming_hardware.InputController,
                world,
                *current_tile_map,
                map_grid,
                camera,
                *gaming_hardware.Speakers);
            if (map_exit_point)
            {
                // SWITCH OVER TO THE NEW MAP GRID.
                CurrentMapGrid = map_exit_point->NewMapGrid;

                // UPDATE THE CAMERA TO FOCUS ON THE NEW TILE MAP.
                MATH::Vector2f center_world_position = map_exit_point->NewTileMap->GetCenterWorldPosition();
                camera.SetCenter(center_world_position);

                // MOVE THE PLAYER TO THE START POINT OF THE NEW TILE MAP.
                world.NoahPlayer->SetWorldPosition(map_exit_point->NewPlayerWorldPosition);

                // EXIT THIS UPDATE IF THE PLAYER HAS CHANGED MAPS.
                return;
            }
        }

        // UPDATE THE REST OF THE WORLD WITHIN CURRENT TILE MAP.
        current_tile_map->Update(objects_can_move, current_game_data, gaming_hardware);

        // UPDATE THE CAMERA'S WORLD VIEW.
        UpdateCameraWorldView(
            gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
            world,
            camera,
            *gaming_hardware.Speakers,
            gaming_hardware.InputController,
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
    MAPS::ExitPoint* DuringFloodGameplayState::UpdatePlayerBasedOnInput(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::World& world,
        MAPS::TileMap& current_tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers)
    {
        /// @todo
        speakers;

        MATH::FloatRectangle camera_bounds = camera.ViewBounds;

        // GET THE TILE UNDER NOAH.
        // This is needed to help track if Noah moves onto a different type of tile.
        MATH::Vector2f old_noah_position = world.NoahPlayer->GetWorldPosition();
        std::shared_ptr<MAPS::Tile> original_tile_under_noah = current_tile_map.GetTileAtWorldPosition(
            old_noah_position.X,
            old_noah_position.Y);

        // MOVE NOAH IN RESPONSE TO USER INPUT.
        bool noah_moved_this_frame = false;
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

        // CHECK IF NOAH MOVED THIS FRAME.
        if (noah_moved_this_frame)
        {
            // UPDATE NOAH'S ANIMATION.
            world.NoahPlayer->Sprite.Update(elapsed_time);

            // CHECK IF THE PLAYER STEPPED ON AN EXIT POINT.
            // This should only occur if the player has changed to a different tile.
            // Otherwise, the game might quickly flip back and forth between maps.
            MATH::Vector2f noah_world_position = world.NoahPlayer->GetWorldPosition();
            MAPS::TileMap* tile_map_underneath_noah = map_grid.GetTileMap(noah_world_position.X, noah_world_position.Y);
            std::shared_ptr<MAPS::Tile> tile_under_noah = tile_map_underneath_noah->GetTileAtWorldPosition(
                noah_world_position.X,
                noah_world_position.Y);
            bool ground_tile_changed = (
                (original_tile_under_noah && tile_under_noah) &&
                (original_tile_under_noah->Id != tile_under_noah->Id));
            bool noah_stepped_onto_new_tile = ground_tile_changed;
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

    /// Updates the camera, along with the world based on any major changes
    /// in what the camera is viewing.
    /// @param[in]  elapsed_time - The elapsed time by which to update the camera.
    /// @param[in,out]  world - The world being viewed.
    /// @param[in,out]  camera - The camera to update.
    /// @param[in,out]  speakers - The speakers from which to play any audio.
    /// @param[in,out]  input_controller - The input controller that might
    ///     be tweaked based on camera movement.
    /// @param[in,out]  current_tile_map - The current tile map in view by the camera.
    void DuringFloodGameplayState::UpdateCameraWorldView(
        const sf::Time& elapsed_time,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::TileMap& current_tile_map,
        STATES::SavedGameData& current_game_data)
    {
        /// @todo
        world;
        speakers;
        current_game_data;

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
