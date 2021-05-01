#include "Collision/CollisionDetectionAlgorithms.h"
#include "Debugging/DebugConsole.h"
#include "ErrorHandling/Asserts.h"
#include "Gameplay/FloodElapsedTime.h"
#include "Graphics/TimeOfDayLighting.h"
#include "Maps/Tileset.h"
#include "Resources/FoodGraphics.h"
#include "States/DuringFloodGameplayState.h"

namespace STATES
{
    /// Loads the game state into its initial state.
    /// @param[in]  saved_game_data - The saved game data to use to initialize the gameplay state.
    /// @param[in,out]  world - The world for the gameplay state.
    /// @param[in,out]  renderer - The renderer used for some initialization.
    /// @param[in,out]  gaming_hardware - The hardware used to run the game.
    void DuringFloodGameplayState::Load(
        const SavedGameData& saved_game_data,
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // RESET THE WORLD.
        // Many items in the world need to be reset.
        world.ResetToInitialState();

        // Animal pens need to be initialized.
        world.Ark.InitializeAnimalPens(saved_game_data.CollectedAnimalsBySpeciesThenGender);

        // TRACK THE CURRENT MAP GRID.
        CurrentMapGrid = &world.Ark.Interior.LayersFromBottomToTop[MAPS::Ark::LOWEST_LAYER_INDEX];

        // HAVE THE FAMILY MEMBERS RANDOMLY POSITIONED WITHIN THE ARK.
        world.FamilyMembers.clear();
        for (std::size_t family_member_index = 0; family_member_index < OBJECTS::FamilyMember::COUNT; ++family_member_index)
        {
            // RANDOMLY PLACE THE FAMILY MEMBER IN THE ARK.
            // The dimensions of the family members are taken into account to ensure they'll be completely on-screen.
            float min_family_member_world_x_position = OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
            float max_family_member_world_x_position = (
                MAPS::Ark::INTERIOR_WIDTH_IN_TILE_MAPS * MAPS::TileMap::WIDTH_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<float>)
                - OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
            float min_family_member_world_y_position = OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
            float max_family_member_world_y_position = (
                MAPS::Ark::INTERIOR_HEIGHT_IN_TILE_MAPS * MAPS::TileMap::HEIGHT_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<float>)
                - OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
            // This may have to be repeated multiple times in case the first picked tile isn't walkable.
            // But this isn't expected to result in an infinite or even long-running loop.
            bool family_member_placed_in_world = false;
            while (!family_member_placed_in_world)
            {
                // COMPUTE A RANDOM POSITION FOR THE FAMILY MEMBER.
                unsigned int ark_layer_index = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan<unsigned int>(
                    MAPS::Ark::LAYER_COUNT);
                float family_member_x_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                    min_family_member_world_x_position,
                    max_family_member_world_x_position);
                float family_member_y_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                    min_family_member_world_y_position,
                    max_family_member_world_y_position);
                        
                // CHECK IF A WALKABLE TILE EXISTS AT THAT RANDOM LOCATION.
                // If a walkable tile doesn't exist, we'll just continue trying a different location.
                MAPS::MultiTileMapGrid& ark_layer = world.Ark.Interior.LayersFromBottomToTop[ark_layer_index];
                std::shared_ptr<MAPS::Tile> tile = ark_layer.GetTileAtWorldPosition(
                    family_member_x_position,
                    family_member_y_position);
                bool tile_is_walkable = (tile && tile->IsWalkable());
                if (tile_is_walkable)
                {
                    // PLACE THE FAMILY MEMBER AT THAT WORLD POSITION.
                    family_member_placed_in_world = true;
                    MATH::Vector2f family_member_world_position(family_member_x_position, family_member_y_position);
                    DEBUGGING::DebugConsole::WriteLine("Placing family member at: ", family_member_world_position);
                    world.FamilyMembers.emplace_back(
                        static_cast<OBJECTS::FamilyMember::Type>(family_member_index),
                        family_member_world_position,
                        &ark_layer);
                }
            }
        }

        // POSITION FOOD ON THE THIRD FLOOR OF THE ARK.
        // This floor is empty due to all animals being on the first two floors,
        // so it is useful to add visual variety and place food on this third floor.
        //
        // To avoid any leftover food, the relevant tile maps must be cleared of previous food items.
        // The foods are placed in fixed locations based on the type of the food for simplicity.
        // The foods are approximately evenly distributed across the left-most and right-most
        // tile maps on the top floor in order to leave room for the middle tile map to be
        // used for other things (i.e. "personal space").
        constexpr unsigned int SINGLE_ARK_TILE_MAP_ROW_INDEX = 0;
        constexpr unsigned int ARK_LEFT_TILE_MAP_COLUMN_INDEX = 0;
        MAPS::TileMap* left_tile_map_for_food = world.Ark.Interior.LayersFromBottomToTop[MAPS::Ark::HIGHEST_LAYER_INDEX].GetTileMap(SINGLE_ARK_TILE_MAP_ROW_INDEX, ARK_LEFT_TILE_MAP_COLUMN_INDEX);
        left_tile_map_for_food->FoodOnGround.clear();
        constexpr unsigned int ARK_RIGHT_TILE_MAP_COLUMN_INDEX = 2;
        MAPS::TileMap* right_tile_map_for_food = world.Ark.Interior.LayersFromBottomToTop[MAPS::Ark::HIGHEST_LAYER_INDEX].GetTileMap(SINGLE_ARK_TILE_MAP_ROW_INDEX, ARK_RIGHT_TILE_MAP_COLUMN_INDEX);
        right_tile_map_for_food->FoodOnGround.clear();

        constexpr unsigned int FIRST_VALID_FOOD_ID = OBJECTS::Food::TypeId::NONE + 1;
        for (unsigned int food_id = FIRST_VALID_FOOD_ID; food_id < OBJECTS::Food::COUNT; ++food_id)
        {
            // CHECK IF THE CURRENT FOOD HAS BEEN COLLECTED AT ALL.
            unsigned int current_food_collected_count = world.NoahPlayer->Inventory.FoodCounts[food_id];
            bool current_food_collected = (current_food_collected_count > 0);
            if (!current_food_collected)
            {
                continue;
            }

            // DETERMINE THE TILE MAP THE FOOD SHOULD BE PLACED WITHIN.
            MAPS::TileMap* tile_map_for_food = nullptr;
            constexpr unsigned int FOOD_COUNT_PER_TILE_MAP = OBJECTS::Food::COUNT / 2;
            bool on_left_tile_map = (food_id <= FOOD_COUNT_PER_TILE_MAP);
            if (on_left_tile_map)
            {
                // GET THE LEFT TILE MAP OF THE ARK'S THIRD FLOOR.
                tile_map_for_food = left_tile_map_for_food;
            }
            else
            {
                // GET THE RIGHT TILE MAP OF THE ARK'S THIRD FLOOR.
                tile_map_for_food = right_tile_map_for_food;
            }

            // PLACE FOOD WITHIN THE TILE MAP.
            ASSERT_THEN_IF(tile_map_for_food)
            {
                // DETERMINE THE BOUNDARIES IN WHICH TO POSITION FOOD WITHIN THE TILE MAP.
                // An apron of 3 tiles from the edges of the map is used to avoid having the food appear
                // outside of the actual map (on black tiles) or on top of things like stairs.
                constexpr float FOOD_TILE_OFFSET_FROM_BORDERS = 3.0f;
                constexpr float FOOD_PIXEL_OFFSET_FROM_BORDERS = FOOD_TILE_OFFSET_FROM_BORDERS * MAPS::Tile::DIMENSION_IN_PIXELS<float>;
                MATH::FloatRectangle tile_map_world_boundaries = tile_map_for_food->GetWorldBoundingBox();
                MATH::FloatRectangle all_food_boundaries = tile_map_world_boundaries.Shrink(FOOD_PIXEL_OFFSET_FROM_BORDERS);

                // The spacing here is chosen to try and make the most of space on each tile map
                // and result in a nice even distribution in a rectangular pattern.
                constexpr float SPACE_FOR_FOOD_ITEM_IN_PIXELS = 80.0f;
                constexpr float FOOD_HALF_WIDTH_IN_PIXELS = SPACE_FOR_FOOD_ITEM_IN_PIXELS / 2.0f;
                float starting_food_center_x_position = all_food_boundaries.LeftTop.X + FOOD_HALF_WIDTH_IN_PIXELS;
                float starting_food_center_y_position = all_food_boundaries.LeftTop.Y + FOOD_HALF_WIDTH_IN_PIXELS;

                // CALCULATE HOW MANY FOOD BOXES CAN APPEAR PER ROW AND COLUMN.
                float food_space_total_width_in_pixels = all_food_boundaries.Width();
                unsigned int boxes_per_row = static_cast<unsigned int>(food_space_total_width_in_pixels / SPACE_FOR_FOOD_ITEM_IN_PIXELS);

                // DETERMINE THE POSITION FOR THIS CURRENT FOOD ITEM.
                // Since the first food enum isn't valid, the ID must be adjusted to
                // make things zero-based for calculating the box indices.
                unsigned int zero_based_food_id = food_id - 1;
                unsigned int normalized_food_index_within_tile_map = zero_based_food_id % FOOD_COUNT_PER_TILE_MAP;
                unsigned int box_row_index = normalized_food_index_within_tile_map / boxes_per_row;
                unsigned int box_column_index = normalized_food_index_within_tile_map % boxes_per_row;

                float current_box_x_offset = static_cast<float>(box_column_index * SPACE_FOR_FOOD_ITEM_IN_PIXELS);
                float current_food_center_x_position = starting_food_center_x_position + current_box_x_offset;

                float current_box_y_offset = static_cast<float>(box_row_index * SPACE_FOR_FOOD_ITEM_IN_PIXELS);
                float current_food_center_y_position = starting_food_center_y_position + current_box_y_offset;

                // PLACE THIS FOOD ITEM AT THE APPROPRIATE SPOT.
                OBJECTS::Food::TypeId food_type = static_cast<OBJECTS::Food::TypeId>(food_id);
                std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type);
                food_sprite->WorldPosition.X = current_food_center_x_position;
                food_sprite->WorldPosition.Y = current_food_center_y_position;
                OBJECTS::Food food =
                {
                    .Type = food_type,
                    .Count = current_food_collected_count,
                    .Sprite = *food_sprite
                };
                tile_map_for_food->FoodOnGround.push_back(food);

                // REMOVE THE FOOD FROM THE PLAYER'S INVENTORY SINCE IT IS NOW ON THE GROUND.
                world.NoahPlayer->Inventory.FoodCounts[food_id] = 0;

                /// @todo   How to store this information in saved game data?
            }
        }

        // MOVE THE PLAYER INTO THE ENTRANCE.
        std::shared_ptr<MAPS::TileMap> entrance_map = world.Ark.GetEntranceMap();
        MATH::Vector2f entrance_map_center_position = entrance_map->GetCenterWorldPosition();
        world.NoahPlayer->SetWorldPosition(entrance_map_center_position);
        renderer.Camera.SetCenter(entrance_map_center_position);

        // CLOSE THE EXITWAY FROM THE ARK.                        
        // The tileset is needed for switching tiles.
        MAPS::Tileset tileset;
        for (unsigned int tile_row = 0; tile_row < MAPS::TileMap::HEIGHT_IN_TILES; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < MAPS::TileMap::WIDTH_IN_TILES; ++tile_column)
            {
                // GET THE CURRENT TILE.
                std::shared_ptr<MAPS::Tile> current_tile = entrance_map->Ground.Tiles(tile_column, tile_row);
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
                            entrance_map->Ground.SetTile(tile_column, tile_row, center_closed_tile);
                        }
                        break;
                    }
                    case MAPS::TileType::ARK_INTERIOR_LEFT_EXIT:
                    {
                        std::shared_ptr<MAPS::Tile> left_closed_tile = tileset.CreateTile(MAPS::TileType::ARK_INTERIOR_LEFT_EXIT_CLOSED);
                        if (left_closed_tile)
                        {
                            entrance_map->Ground.SetTile(tile_column, tile_row, left_closed_tile);
                        }
                        break;
                    }
                    case MAPS::TileType::ARK_INTERIOR_RIGHT_EXIT:
                    {
                        std::shared_ptr<MAPS::Tile> right_closed_tile = tileset.CreateTile(MAPS::TileType::ARK_INTERIOR_RIGHT_EXIT_CLOSED);
                        if (right_closed_tile)
                        {
                            entrance_map->Ground.SetTile(tile_column, tile_row, right_closed_tile);
                        }
                        break;
                    }
                }
            }
        }

        // INITIALIZE THE HUD.
        unsigned int main_text_box_width_in_pixels = renderer.Screen->WidthInPixels<unsigned int>();
        const unsigned int LINE_COUNT = 2;
        unsigned int main_text_box_height_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS * LINE_COUNT;
        Hud = GRAPHICS::GUI::DuringFloodHeadsUpDisplay(
            renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE],
            main_text_box_width_in_pixels,
            main_text_box_height_in_pixels);
    }

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
        // UPDATE THE ELAPSED TIME FOR THE FLOOD.
        // This is done even if the game is paused since it can take a while to go through the full flood day count anyway,
        // so it's fine if this is sped up a bit.
        current_game_data.FloodElapsedGameplayTime += gaming_hardware.Clock.ElapsedTimeSinceLastFrame;

        // UPDATE THE HUD.
        // As of now, the HUD is capable of altering the gameplay state.
        auto [next_game_state, dropped_food_type] = Hud.Update(current_game_data, gaming_hardware);

        // CHECK IF A TYPE OF FOOD WAS SELECTED FOR DROPPING.
        bool food_selected_for_dropping = (OBJECTS::Food::NONE != dropped_food_type);
        if (food_selected_for_dropping)
        {
            // CLOSE THE INVENTORY SINCE THE PLAYER HAS SELECTED TO DROP SOME FOOD.
            Hud.InventoryOpened = false;

            // DROP THE FOOD IN FRONT OF THE PLAYER'S POSITION.
            // Dropping in front is important to ensure that the player does not automatically pick it up.
            std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(dropped_food_type);
            food_sprite->WorldPosition = world.NoahPlayer->GetWorldPosition();

            MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
            switch (world.NoahPlayer->FacingDirection)
            {
                case GAMEPLAY::Direction::UP:
                    food_sprite->WorldPosition.Y -= noah_bounding_box.Height();
                    break;
                case GAMEPLAY::Direction::DOWN:
                    food_sprite->WorldPosition.Y += noah_bounding_box.Height();
                    break;
                case GAMEPLAY::Direction::LEFT:
                    food_sprite->WorldPosition.X -= noah_bounding_box.Width();
                    break;
                case GAMEPLAY::Direction::RIGHT:
                    food_sprite->WorldPosition.X += noah_bounding_box.Width();
                    break;
            }

            OBJECTS::Food food =
            {
                .Type = dropped_food_type,
                // Only a single item of food is dropped at a time.
                .Count = 1,
                .Sprite = *food_sprite
            };
            MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(food_sprite->WorldPosition.X, food_sprite->WorldPosition.Y);
            ASSERT_THEN_IF(current_tile_map)
            {
                current_tile_map->FoodOnGround.push_back(food);
            }

            // REMOVE THE FOOD FROM THE PLAYER'S INVENTORY SINCE IT IS NOW ON THE GROUND.
            --world.NoahPlayer->Inventory.FoodCounts[dropped_food_type];
        }

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

        /// @todo   Check for days when sending out raven/dove!
        /// - +40 days - Noah sends raven + dove out (Genesis 8:6-7)
        /// - +7 days - Noah sends dove out again (Genesis 8:10)
        /// - +7 days - Noah sends dove out again; it doesn't return (Genesis 8:12)
        ///
        /// 375 total days - 27 days for 2nd month - 30 days for 1st month? = 318 for final dove sending
        /// 318 - 7 = 311 for first dove-only sending
        /// 311 - 40 = 271 for raven/dove sending
        /// Could possibly be as early as about day 223 (assuming 30 day months)...so maybe we should provide a range?
        /// Also should probably look at Hebrew calendar.
        /// @todo   Double-check some of the above calculations!
        /// Also see:
        /// - https://www.esv.org/resources/esv-global-study-bible/chart-01-03/
        /// - https://www.understandchristianity.com/timelines/chronology-flood/
        /// - https://answersingenesis.org/bible-timeline/biblical-overview-of-the-flood-timeline/
        /// - https://answersingenesis.org/bible-timeline/avian-flood-chronology-noah-window/

        // CHECK FOR SPECIAL DAY-BASED EVENTS.
        // 30 month day is simple approximation for this game.
        constexpr unsigned int DAYS_PER_MONTH = 30;
        // 2nd month, 17th day - Flood starts (Genesis 7:11)
        // +150 days (Genesis 7:24, Genesis 8:3)
        // = 
        // 7th month, 17th day (5 months later) - Ark comes to rest on mountains of Ararat (Genesis 8:4)
        constexpr unsigned int DAY_COUNT_FOR_FLOODS_PREVAILING_ON_EARTH = 150;
        // 10th month, 1st day - Waters recede to point where mountaintops seen (Genesis 8:5)
        constexpr unsigned int MONTH_COUNT_BETWEEN_FLOODS_PREVAILING_AND_RECEDING = 2;
        // 30 days per month - 17 days from 7th month = 13 remaining days in 7th month.
        constexpr unsigned int DAY_COUNT_IN_7TH_MONTH_FOR_FLOOD_RECEDING = 13;
        constexpr unsigned int DAY_COUNT_FOR_FLOODS_PREVAILING_AND_RECEDING = (
            DAY_COUNT_FOR_FLOODS_PREVAILING_ON_EARTH + 
            DAY_COUNT_IN_7TH_MONTH_FOR_FLOOD_RECEDING +
            (DAYS_PER_MONTH * MONTH_COUNT_BETWEEN_FLOODS_PREVAILING_AND_RECEDING));
        // Genesis 8:6-8.
        constexpr unsigned int DAY_COUNT_AFTER_INITIAL_FLOOD_RECEDING_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING = 40;
        constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING = (
            DAY_COUNT_FOR_FLOODS_PREVAILING_AND_RECEDING + DAY_COUNT_AFTER_INITIAL_FLOOD_RECEDING_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING);
        // Genesis 8:10.
        constexpr unsigned int DAY_COUNT_BETWEEN_FIRST_AND_SECOND_DOVE_SENDINGS = 7;
        constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING = FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING + DAY_COUNT_BETWEEN_FIRST_AND_SECOND_DOVE_SENDINGS;
        // Genesis 8:12.
        constexpr unsigned int DAY_COUNT_BETWEEN_SECOND_AND_FINAL_DOVE_SENDINGS = 7;
        constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_FINAL_DOVE_SENDING = FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING + DAY_COUNT_BETWEEN_SECOND_AND_FINAL_DOVE_SENDINGS;
        
        unsigned int current_day_of_flood = 0;
        unsigned int current_hour_of_day = 0;
        GAMEPLAY::FloodElapsedTime::GetCurrentDayAndHour(current_game_data.FloodElapsedGameplayTime, current_day_of_flood, current_hour_of_day);

        switch (current_day_of_flood)
        {
            case FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING:
            {
                DEBUGGING::DebugConsole::WriteLine("FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING = ", FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING);
                break;
            }
            case FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING:
            {
                DEBUGGING::DebugConsole::WriteLine("FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING = ", FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING);
                break;
            }
            case FLOOD_DAY_COUNT_UNTIL_FINAL_DOVE_SENDING:
            {
                DEBUGGING::DebugConsole::WriteLine("FLOOD_DAY_COUNT_UNTIL_FINAL_DOVE_SENDING = ", FLOOD_DAY_COUNT_UNTIL_FINAL_DOVE_SENDING);
                break;
            }
        }

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

        // RENDER EACH FAMILY MEMBER IF IN VIEW.
        for (const OBJECTS::FamilyMember& family_member : world.FamilyMembers)
        {
            // ONLY RENDER THE FAMILY MEMBER IF THEY'RE IN VIEW.
            bool family_member_in_current_map_grid = (CurrentMapGrid == family_member.MapGrid);

            MATH::Vector2f family_member_world_position = family_member.Sprite.GetWorldPosition();
            bool family_member_in_view_of_camera = renderer.Camera.ViewBounds.Contains(
                family_member_world_position.X, 
                family_member_world_position.Y);

            bool family_member_in_view = (family_member_in_current_map_grid && family_member_in_view_of_camera);
            if (family_member_in_view)
            {
                renderer.Render(family_member.Sprite.CurrentFrameSprite);
            }
        }

        // RENDER THE PLAYER.
        renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);

        // RENDER A TOOLTIP FOR COLLECTING FOOD IF THE PLAYER IS ON TOP OF IT.
        MATH::FloatRectangle camera_bounds = renderer.Camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.Center();
        MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
        ASSERT_THEN_IF(current_tile_map)
        {
            for (auto food = current_tile_map->FoodOnGround.cbegin();
                food != current_tile_map->FoodOnGround.cend();
                ++food)
            {
                // CHECK IF THE CURRENT FOOD ITEM INTERSECTS WITH THE PLAYER.
                MATH::FloatRectangle food_bounding_box = food->Sprite.GetWorldBoundingBox();
                MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                bool food_intersects_with_noah = food_bounding_box.Intersects(noah_bounding_box);
                if (food_intersects_with_noah)
                {
                    // RENDER A TOOLTIP ICON FOR COLLECTING THE FOOD.
                    MATH::FloatRectangle current_tile_map_world_bounding_box = current_tile_map->GetWorldBoundingBox();

                    MATH::Vector2f food_center_position = food_bounding_box.Center();
                    MATH::Vector2ui collect_food_icon_top_left_position(
                        static_cast<unsigned int>(food_center_position.X - current_tile_map_world_bounding_box.LeftTop.X),
                        static_cast<unsigned int>(food_center_position.Y - current_tile_map_world_bounding_box.LeftTop.Y));
                    // The colors are adjusted to be light text on a dark background since that seems easier to see.
                    const GRAPHICS::Color& BORDER_COLOR = GRAPHICS::Color::WHITE;
                    const GRAPHICS::Color& BACKGROUND_COLOR = GRAPHICS::Color::BLACK;
                    const GRAPHICS::Color& TEXT_COLOR = GRAPHICS::Color::WHITE;
                    renderer.RenderKeyIcon(
                        INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY_TEXT, 
                        collect_food_icon_top_left_position,
                        GRAPHICS::ShapeType::CIRCLE,
                        BORDER_COLOR,
                        BACKGROUND_COLOR,
                        TEXT_COLOR);

                    MATH::Vector2f tooltip_text_top_left_screen_position(
                        food_bounding_box.RightBottom.X - current_tile_map_world_bounding_box.LeftTop.X,
                        food_center_position.Y - current_tile_map_world_bounding_box.LeftTop.Y);
                    tooltip_text_top_left_screen_position.X += GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS;
                    renderer.RenderText(
                        // Short text is used to have it more easily fit on screen.
                        "Pick Up 1",
                        RESOURCES::AssetId::FONT_TEXTURE,
                        tooltip_text_top_left_screen_position,
                        TEXT_COLOR);
                    
                    // There is no need to continue looping if the player is already on top of a food item.
                    break;
                }
            }
        }

        // RENDER THE HUD.
        Hud.Render(current_game_data, renderer);

        // RENDER THE FINAL SCREEN WITH TIME-OF-DAY LIGHTING.
        // It is tinted based on the current hour.
        sf::RenderStates lighting = sf::RenderStates::Default;
        std::shared_ptr<sf::Shader> time_of_day_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::TIME_OF_DAY_SHADER);
        if (time_of_day_shader)
        {
            // COMPUTE THE LIGHTING FOR THE SHADER.
            unsigned int flood_day_count = 0;
            unsigned int current_day_hour = 0;
            GAMEPLAY::FloodElapsedTime::GetCurrentDayAndHour(current_game_data.FloodElapsedGameplayTime, flood_day_count, current_day_hour);
            GRAPHICS::TimeOfDayLighting::Compute(current_day_hour, *time_of_day_shader);
            lighting.shader = time_of_day_shader.get();
        }

        sf::Sprite screen = renderer.RenderFinalScreen(lighting);
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
        // If the HUD is displaying a modal component, then objects shouldn't move.
        // If the main text box is displaying text, then no objects should move
        // to avoid having the player's gameplay hindered.
        bool objects_can_move = (!Hud.ModalComponentDisplayed() && !Hud.MainTextBox.IsVisible);
        if (objects_can_move)
        {
            // UPDATE THE PLAYER BASED ON INPUT.
            std::string message_for_text_box;
            MAPS::ExitPoint* map_exit_point = UpdatePlayerBasedOnInput(
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
                gaming_hardware.InputController,
                world,
                *current_tile_map,
                map_grid,
                camera,
                *gaming_hardware.Speakers,
                message_for_text_box);
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

            // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
            bool text_box_message_exists = !message_for_text_box.empty();
            if (text_box_message_exists)
            {
                Hud.MainTextBox.StartDisplayingText(message_for_text_box);
            }

            // MOVE EACH FAMILY MEMBER IN VIEW.
            for (OBJECTS::FamilyMember& family_member : world.FamilyMembers)
            {
                // MOVE THE FAMILY MEMBER IF IN VIEW.
                bool family_member_in_current_map_grid = (CurrentMapGrid == family_member.MapGrid);

                MATH::Vector2f family_member_world_position = family_member.Sprite.GetWorldPosition();
                bool family_member_in_view_of_camera = camera.ViewBounds.Contains(
                    family_member_world_position.X, 
                    family_member_world_position.Y);

                bool family_member_in_view = (family_member_in_current_map_grid && family_member_in_view_of_camera);
                if (family_member_in_view)
                {
                    family_member.MoveWithin(*current_tile_map, gaming_hardware);
                }
            }

            // CHECK FOR COLLISIONS WITH FOOD.
            for (auto food = current_tile_map->FoodOnGround.begin();
                food != current_tile_map->FoodOnGround.end();)
            {
                // CHECK IF THE CURRENT FOOD ITEM INTERSECTS WITH THE PLAYER.
                MATH::FloatRectangle food_bounding_box = food->Sprite.GetWorldBoundingBox();
                // Food for the player only needs to be collected if the player is pressing the appropriate button,
                // so there is no need to check for this otherwise.
                bool collect_food_button_pressed = gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY);
                if (collect_food_button_pressed)
                {
                    MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
                    bool food_intersects_with_noah = food_bounding_box.Intersects(noah_bounding_box);
                    if (food_intersects_with_noah)
                    {
                        // PLAY THE SOUND EFFECT FOR COLLECTING FOOD.
                        gaming_hardware.Speakers->PlaySoundEffect(RESOURCES::AssetId::FOOD_PICKUP_SOUND);

                        // ADD ONE ITEM OF FOOD TO THE PLAYER'S INVENTORY.
                        // This is done one at a time for a more interesting experience
                        DEBUGGING::DebugConsole::WriteLine("Collected 1 food: ", static_cast<int>(food->Type));
                        ++world.NoahPlayer->Inventory.FoodCounts[food->Type];
                        --food->Count;

                        // REMOVE THE FOOD ITEM FROM THOSE IN THE CURRENT TILE MAP IF IT HAS BEEN COMPLETELY COLLECTED.
                        // This should move to the next food item.
                        bool food_completely_collected = (food->Count <= 0);
                        if (food_completely_collected)
                        {
                            food = current_tile_map->FoodOnGround.erase(food);
                            continue;
                        }
                        else
                        {
                            // MOVE TO CHECKING COLLISIONS FOR THE NEXT FOOD ITEM.
                            ++food;
                            continue;
                        }
                    }
                    else
                    {
                        // MOVE TO CHECKING COLLISIONS FOR THE NEXT FOOD ITEM.
                        ++food;
                        continue;
                    }
                }

                // CHECK FOR COLLISIONS OF FOOD WITH ANIMALS.
                bool current_food_eaten_by_animal = false;
                for (const MAPS::AnimalPen& animal_pen : current_tile_map->AnimalPens)
                {
                    // CHECK IF THE CURRENT FOOD IS WITHIN THE ANIMAL PEN.
                    bool food_in_animal_pen = food_bounding_box.Intersects(animal_pen.InteriorBoundingBox);
                    if (!food_in_animal_pen)
                    {
                        // CONTINUE CHECKING OTHER ANIMAL PENS.
                        continue;
                    }

                    // CHECK IF THE FOOD INTERSECTS WITH ANY ANIMAL.
                    for (const auto& animal : animal_pen.Animals)
                    {
                        MATH::FloatRectangle animal_bounding_box = animal->Sprite.GetWorldBoundingBox();
                        /// @todo   More complex logic for animals sometimes eating food?
                        bool food_intersects_animal = food_bounding_box.Intersects(animal_bounding_box);
                        if (food_intersects_animal)
                        {
                            /// @todo   Play eating sound?
                            current_food_eaten_by_animal = true;

                            // ADD A PRESENT WITH A BIBLE VERSE IF SOME BIBLE VERSES STILL NEED TO BE COLLECTED.
                            /// @todo   Only add presents sometimes? (randomly)
                            /// @todo   Should we have a delay for the presents appearing?  Some kind of timer?
                            bool all_bible_verses_collected = current_game_data.BibleVersesLeftToFind.empty();
                            if (!all_bible_verses_collected)
                            {
                                size_t remaining_bible_verse_count = current_game_data.BibleVersesLeftToFind.size();
                                size_t random_bible_verse_index = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan(remaining_bible_verse_count);
                                auto random_bible_verse = current_game_data.BibleVersesLeftToFind.begin() + random_bible_verse_index;

                                MATH::Vector2f present_center_position = food_bounding_box.Center();
                                current_tile_map->Presents.emplace_back(present_center_position, *random_bible_verse);

                                // The Bible verse is pre-emptively removed so that it does not have to be refound for removing later.
                                /// @todo   How to handle this with saving game data?  Saving presents should save verse for reloading?
                                current_game_data.BibleVersesLeftToFind.erase(random_bible_verse);
                            }

                            // No need to continue looping if food was eaten.
                            break;
                        }
                    }

                    // CHECK IF THE FOOD WAS EATEN WITHIN THE CURRENT ANIMAL PEN.
                    if (current_food_eaten_by_animal)
                    {
                        // If the current food was eaten, no need to continue checking other animal pens.
                        break;
                    }
                }

                // CHECK IF THE CURRENT FOOD WAS EATEN BY AN ANIMAL.
                if (current_food_eaten_by_animal)
                {
                    // ERASE THE CURRENT FOOD FROM THE GROUND.
                    food = current_tile_map->FoodOnGround.erase(food);
                    continue;
                }
                else
                {
                    // MOVE TO CHECKING COLLISIONS FOR THE NEXT FOOD ITEM.
                    ++food;
                    continue;
                }
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
    /// @param[out] message_for_text_box - A message for the HUD's main text box, if
    ///     a Bible verse was collected.
    /// @return The map exit point, if the player stepped on such a point.
    MAPS::ExitPoint* DuringFloodGameplayState::UpdatePlayerBasedOnInput(
        const sf::Time& elapsed_time,
        INPUT_CONTROL::InputController& input_controller,
        MAPS::World& world,
        MAPS::TileMap& current_tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera,
        AUDIO::Speakers& speakers,
        std::string& message_for_text_box)
    {
        // INDICATE THAT NO MESSAGE EXISTS FOR THE TEXT BOX YET.
        message_for_text_box.clear();

        // GET THE CAMERA'S VIEWING BOUNDS.
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

            // CHECK IF THE PLAYER STEPPED ON A PRESENT.
            MATH::FloatRectangle noah_bounding_box = world.NoahPlayer->GetWorldBoundingBox();
            for (auto present = current_tile_map.Presents.begin();
                present != current_tile_map.Presents.end();)
            {
                // CHECK IF THE PLAYER STEPPED ON THE CURRENT PRESENT.
                MATH::FloatRectangle present_bounding_box = present->Sprite.GetWorldBoundingBox();
                bool noah_stepped_on_present = noah_bounding_box.Intersects(present_bounding_box);
                if (noah_stepped_on_present)
                {
                    // PLAY THE SOUND EFFECT FOR COLLECTING A BIBLE VERSE.
                    speakers.PlaySoundEffect(RESOURCES::AssetId::COLLECT_BIBLE_VERSE_SOUND);

                    // ADD THE BIBLE VERSE TO THE PLAYER'S INVENTORY.
                    world.NoahPlayer->Inventory.BibleVerses.insert(present->BibleVerse);

                    // POPULATE THE MESSAGE TO DISPLAY IN THE MAIN TEXT BOX.
                    message_for_text_box = "You got a Bible verse!\n" + present->BibleVerse.ToString();

                    // REMOVE THE PRESENT FROM THE TILE MAP.
                    present = current_tile_map.Presents.erase(present);
                }
                else
                {
                    // MOVE TO THE NEXT PRESENT.
                    ++present;
                }
            }

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
