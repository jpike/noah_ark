#include "Collision/Collisions.h"
#include "Maps/Overworld.h"

namespace MAPS
{
    Overworld::Overworld(
        const unsigned int width_in_tile_maps,
        const unsigned int height_in_tile_maps,
        const unsigned int tile_map_width_in_tiles,
        const unsigned int tile_map_height_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    TileMaps(width_in_tile_maps, height_in_tile_maps),
    TileMapWidthInTiles(tile_map_width_in_tiles),
    TileMapHeightInTiles(tile_map_height_in_tiles),
    TileDimensionInPixels(tile_dimension_in_pixels),
    NoahPlayer(),
    AxeSwings()
    {}

    void Overworld::Update(
        const float elapsed_time_in_seconds,
        std::random_device& random_number_generator,
        INPUT_CONTROL::KeyboardInputController& input_controller,
        std::vector<BIBLE::BibleVerse>& bible_verses_left_to_find,
        RESOURCES::Assets& assets,
        GRAPHICS::Camera& camera,
        std::string& message_for_text_box)
    {
        // INDICATE THAT NO MESSAGE NEEDS TO BE DISPLAYED YET IN THE TEXT BOX.
        message_for_text_box.clear();

        /// @todo   Try and remove this dependency on the camera.
        /// It's here for now to ease the transition from having most
        /// of this update logic in the main loop, but trying to
        /// remove it immediately seemed like it might carry too
        /// high a risk of introducing bugs.
        MATH::FloatRectangle camera_bounds = camera.GetViewBounds();
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

        MAPS::TileMap* current_tile_map = GetTileMap(camera_view_center.X, camera_view_center.Y);
        assert(current_tile_map);

        // CHECK IF THE PRIMARY ACTION BUTTON WAS PRESSED.
        if (input_controller.PrimaryActionButtonPressed())
        {
            // SWING THE PLAYER'S AXE.
            // A new axe swing may not be created if the player's
            // axe is already being swung.
            std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = NoahPlayer.SwingAxe();
            bool axe_swing_occurred = (nullptr != axe_swing);
            if (axe_swing_occurred)
            {
                // Allow the axe to collide with other objects.
                AxeSwings.push_back(axe_swing);
            }
        }

        // CHECK IF THE PLAYER IS ALLOWED TO MOVE.
        // Noah can't move if the text box is capturing input or while the axe is swinging.
        // Movement is prevented to have the axe's position
        // remain attached to Noah.
        // We need to keep track if Noah moved this frame
        // so that we can stop any walking animations for
        // him if he didn't move.
        bool noah_moved_this_frame = false;
        bool axe_is_swinging = (nullptr != NoahPlayer.Inventory->Axe) && NoahPlayer.Inventory->Axe->IsSwinging();
        bool player_movement_allowed = (!axe_is_swinging);
        if (player_movement_allowed)
        {
            // MOVE NOAH IN RESPONSE TO USER INPUT.
            const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
            MATH::Vector2f old_noah_position = NoahPlayer.GetWorldPosition();
            if (input_controller.UpButtonPressed())
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                NoahPlayer.FacingDirection = CORE::Direction::UP;

                // PLAY THE WALKING UP ANIMATION.
                NoahPlayer.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID);
                NoahPlayer.Sprite.Play();

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                    *this,
                    elapsed_time_in_seconds,
                    CORE::Direction::UP,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    NoahPlayer.GetWorldBoundingBox());
                NoahPlayer.SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer.GetWorldBoundingBox();
                float camera_top_y_position = camera_bounds.GetTopYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetTopYPosition() < camera_top_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A TOP TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int top_tile_map_row_index = current_tile_map->OverworldRowIndex - 1;
                    unsigned int top_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                    MAPS::TileMap* top_tile_map = GetTileMap(
                        top_tile_map_row_index,
                        top_tile_map_column_index);
                    bool top_tile_map_exists = (nullptr != top_tile_map);
                    if (top_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS UP SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = NoahPlayer.GetWorldPosition();
                        noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer.SetWorldPosition(noah_world_position);

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

                        NoahPlayer.SetWorldPosition(noah_world_position);
                    }
                } // end if (player_moved_out_of_view)
            } // end if (input_controller.UpButtonPressed())
            if (input_controller.DownButtonPressed())
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                NoahPlayer.FacingDirection = CORE::Direction::DOWN;

                // PLAY THE WALKING DOWN ANIMATION.
                NoahPlayer.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);
                NoahPlayer.Sprite.Play();

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                    *this,
                    elapsed_time_in_seconds,
                    CORE::Direction::DOWN,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    NoahPlayer.GetWorldBoundingBox());
                NoahPlayer.SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer.GetWorldBoundingBox();
                float camera_bottom_y_position = camera_bounds.GetBottomYPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetBottomYPosition() > camera_bottom_y_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A BOTTOM TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int bottom_tile_map_row_index = current_tile_map->OverworldRowIndex + 1;
                    unsigned int bottom_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                    MAPS::TileMap* bottom_tile_map = GetTileMap(
                        bottom_tile_map_row_index,
                        bottom_tile_map_column_index);
                    bool bottom_tile_map_exists = (nullptr != bottom_tile_map);
                    if (bottom_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS DOWN SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = NoahPlayer.GetWorldPosition();
                        noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer.SetWorldPosition(noah_world_position);

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
                        float noah_half_height = NoahPlayer.GetWorldBoundingBox().GetHeight() / 2.0f;
                        noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;

                        NoahPlayer.SetWorldPosition(noah_world_position);
                    }
                } // end if (player_moved_out_of_view)
            } // end if (input_controller.DownButtonPressed())
            if (input_controller.LeftButtonPressed())
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                NoahPlayer.FacingDirection = CORE::Direction::LEFT;

                // PLAY THE WALKING UP ANIMATION.
                NoahPlayer.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID);
                NoahPlayer.Sprite.Play();

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                    *this,
                    elapsed_time_in_seconds,
                    CORE::Direction::LEFT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    NoahPlayer.GetWorldBoundingBox());
                NoahPlayer.SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer.GetWorldBoundingBox();
                float camera_left_x_position = camera_bounds.GetLeftXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetLeftXPosition() < camera_left_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A LEFT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int left_tile_map_row_index = current_tile_map->OverworldRowIndex;
                    unsigned int left_tile_map_column_index = current_tile_map->OverworldColumnIndex - 1;
                    MAPS::TileMap* left_tile_map = GetTileMap(
                        left_tile_map_row_index,
                        left_tile_map_column_index);
                    bool left_tile_map_exists = (nullptr != left_tile_map);
                    if (left_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS LEFT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = NoahPlayer.GetWorldPosition();
                        noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer.SetWorldPosition(noah_world_position);

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
                        float noah_half_width = NoahPlayer.GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_left_boundary + noah_half_width;

                        NoahPlayer.SetWorldPosition(noah_world_position);
                    }
                } // end if (player_moved_out_of_view)
            } // end if (input_controller.LeftButtonPressed())
            if (input_controller.RightButtonPressed())
            {
                // TRACK NOAH AS MOVING THIS FRAME.
                noah_moved_this_frame = true;

                // HAVE NOAH FACE IN THE APPROPRIATE DIRECTION.
                NoahPlayer.FacingDirection = CORE::Direction::RIGHT;

                // PLAY THE WALKING UP ANIMATION.
                NoahPlayer.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID);
                NoahPlayer.Sprite.Play();

                // MOVE NOAH WHILE HANDLING COLLISIONS.
                MATH::Vector2f new_position = COLLISION::MoveWithCollisionDetection(
                    *this,
                    elapsed_time_in_seconds,
                    CORE::Direction::RIGHT,
                    OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                    NoahPlayer.GetWorldBoundingBox());
                NoahPlayer.SetWorldPosition(new_position);

                // CHECK IF NOAH MOVED OUT OF THE CAMERA'S VIEW.
                MATH::FloatRectangle noah_world_bounding_box = NoahPlayer.GetWorldBoundingBox();
                float camera_right_x_position = camera_bounds.GetRightXPosition();
                bool player_moved_out_of_view = (noah_world_bounding_box.GetRightXPosition() > camera_right_x_position);
                if (player_moved_out_of_view)
                {
                    // CHECK IF A RIGHT TILE MAP EXISTS FOR NOAH TO MOVE TO.
                    unsigned int right_tile_map_row_index = current_tile_map->OverworldRowIndex;
                    unsigned int right_tile_map_column_index = current_tile_map->OverworldColumnIndex + 1;
                    MAPS::TileMap* right_tile_map = GetTileMap(
                        right_tile_map_row_index,
                        right_tile_map_column_index);
                    bool right_tile_map_exists = (nullptr != right_tile_map);
                    if (right_tile_map_exists)
                    {
                        // MOVE NOAH A FEW MORE PIXELS RIGHT SO THAT HE WILL BE MORE VISIBLE ON THE NEW MAP.
                        MATH::Vector2f noah_world_position = NoahPlayer.GetWorldPosition();
                        noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                        NoahPlayer.SetWorldPosition(noah_world_position);

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
                        float noah_half_width = NoahPlayer.GetWorldBoundingBox().GetWidth() / 2.0f;
                        noah_world_position.X = tile_map_right_boundary - noah_half_width;

                        NoahPlayer.SetWorldPosition(noah_world_position);
                    }
                } // end if (player_moved_out_of_view)
            } // end if (input_controller.RightButtonPressed())
        } // end if (!axe_is_swinging)

        // STOP NOAH'S ANIMATION FROM PLAYING IF THE PLAYER DIDN'T MOVE THIS FRAME.
        if (!noah_moved_this_frame)
        {
            NoahPlayer.Sprite.ResetAnimation();
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
        COLLISION::HandleAxeSwingCollisions(*this, AxeSwings, assets);
        for (auto wood_logs = current_tile_map->WoodLogs.begin(); wood_logs != current_tile_map->WoodLogs.end();)
        {
            // CHECK IF THE WOOD LOGS INTERSECT WITH NOAH.
            MATH::FloatRectangle wood_log_bounding_box = wood_logs->GetWorldBoundingBox();
            MATH::FloatRectangle noah_bounding_box = NoahPlayer.GetWorldBoundingBox();
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
                NoahPlayer.Inventory->AddWood();

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
                        // PLAY THE SOUND EFFECT FOR COLLECTING A BIBLE VERSE.
                        /// @todo   Perhaps factor this out?  If we can get rid of the dependency
                        /// on the game assets from this method, then this might be feasible.
                        std::shared_ptr<AUDIO::SoundEffect> collected_bible_verse_sound = assets.GetSound(RESOURCES::COLLECT_BIBLE_VERSE_SOUND_ID);
                        bool collect_bible_verse_sound_loaded = (nullptr != collected_bible_verse_sound);
                        if (collect_bible_verse_sound_loaded)
                        {
                            collected_bible_verse_sound->Play();
                        }

                        // SELECT A RANDOM BIBLE VERSE.
                        unsigned int random_bible_verse_index = random_number_generator() % bible_verses_left_to_find.size();
                        auto bible_verse = bible_verses_left_to_find.begin() + random_bible_verse_index;

                        // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                        NoahPlayer.Inventory->BibleVerses.insert(*bible_verse);

                        // POPULATE THE MESSAGE TO DISPLAY IN THE MAIN TEXT BOX.
                        std::stringstream bible_verse_message;
                        bible_verse_message
                            << "You got a Bible verse!\n"
                            << bible_verse->ToString();
                        message_for_text_box = bible_verse_message.str();

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
    }

    MAPS::TileMap* Overworld::GetTileMap(const unsigned int row, const unsigned int column)
    {
        // MAKE SURE THE PROVIDED INDICES ARE IN RANGE.
        bool tile_map_indices_valid = TileMaps.IndicesInRange(column, row);
        if (!tile_map_indices_valid)
        {
            // No tile map exists at an invalid location.
            return nullptr;
        }

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        MAPS::TileMap& tile_map = TileMaps(column, row);
        return &tile_map;
    }
    
    MAPS::TileMap* Overworld::GetTileMap(const float world_x_position, const float world_y_position)
    {
        // CONVERT THE WORLD POSITIONS TO ROW/COLUMN INDICES.
        float tile_map_width_in_pixels = static_cast<float>(TileMapWidthInTiles * TileDimensionInPixels);
        float tile_map_height_in_pixels = static_cast<float>(TileMapHeightInTiles * TileDimensionInPixels);
        unsigned int column_index = static_cast<unsigned int>(world_x_position / tile_map_width_in_pixels);
        unsigned int row_index = static_cast<unsigned int>(world_y_position / tile_map_height_in_pixels);

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        MAPS::TileMap* tile_map = GetTileMap(row_index, column_index);
        return tile_map;
    }

    std::shared_ptr<MAPS::Tile> Overworld::GetTileAtWorldPosition(const float world_x_position, const float world_y_position)
    {
        // GET THE TILE MAP AT THE SPECIFIED WORLD POSITION.
        MAPS::TileMap* tile_map = GetTileMap(world_x_position, world_y_position);
        bool tile_map_exists = (nullptr != tile_map);
        if (!tile_map_exists)
        {
            // No tile map exists that could contain a tile at the specified world position.
            return nullptr;
        }

        // GET THE TILE IN THE TILE MAP AT THE SPECIFIED WORLD POSITION.
        std::shared_ptr<MAPS::Tile> tile = tile_map->GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }
}