#include "Collision/CollisionDetectionAlgorithms.h"
#include "ErrorHandling/Asserts.h"
#include "Resources/AnimalGraphics.h"
#include "Resources/AnimalSounds.h"
#include "States/PostFloodGameplayState.h"

namespace STATES
{
    /// Loads the cutscene into its initial state.
    /// @param[in]  saved_game_data - The saved game data.
    /// @param[in,out]  world - The world to configure for this state.
    /// @param[in,out]  renderer - The renderer from which to load some initial data.
    /// @param[in,out]  gaming_hardware - The gaming hardware.
    void PostFloodGameplayState::Load(const STATES::SavedGameData& saved_game_data, MAPS::World& world, GRAPHICS::Renderer& renderer, HARDWARE::GamingHardware& gaming_hardware)
    {
        // RESET BASIC MEMBER VARIABLES TO THE BEGINNING OF THE CUTSCENE.
        CurrentSubstate = Substate::FADING_IN;
        ElapsedTimeForCurrentSubstate = sf::Time::Zero;

        // INITIALIZE THE TEXT BOX IF POSSIBLE.
        std::shared_ptr<GRAPHICS::GUI::Font> text_box_font = renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE];
        ASSERT_THEN_IF(text_box_font)
        {
            unsigned int text_box_width_in_pixels = renderer.Screen->WidthInPixels<unsigned int>();
            // The text displayed in this text box is slightly longer than normal, so 3 lines are used instead of the normal 2.
            const unsigned int LINE_COUNT = 3;
            unsigned int text_box_height_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS * LINE_COUNT;
            TextBox = GRAPHICS::GUI::TextBox(
                text_box_width_in_pixels,
                text_box_height_in_pixels,
                text_box_font);
        }

        // TRACK THE CURRENT MAP GRID.
        CurrentMapGrid = &world.Overworld.MapGrid;

        // SET THE GAME TO FOCUS ON THE ARK ENTRANCE.
        MAPS::TileMap* ark_entrace_tile_map = world.Overworld.MapGrid.GetTileMap(MAPS::Overworld::ARK_ENTRANCE_TILE_MAP_ROW, MAPS::Overworld::ARK_ENTRANCE_TILE_MAP_COLUMN);
        ASSERT_THEN_IF(ark_entrace_tile_map)
        {
            // SET THE CAMERA TO FOCUS ON THIS TILE MAP.
            MATH::Vector2f ark_entrance_tile_map_center = ark_entrace_tile_map->GetCenterWorldPosition();
            renderer.Camera.SetCenter(ark_entrance_tile_map_center);

            // MOVE NOAH TO RIGHT OUTSIDE THE ARK.
            // They're positioned slightly below the entrance.
            MATH::Vector2f noah_world_position = ark_entrance_tile_map_center + MATH::Vector2f(0.0f, 64.0f);
            world.NoahPlayer->SetWorldPosition(noah_world_position);
            world.NoahPlayer->Sprite.CurrentFrameSprite.IsVisible = true;

            // RANDOMLY POSITION FAMILY MEMBERS.
            world.FamilyMembers.clear();
            for (std::size_t family_member_index = 0; family_member_index < OBJECTS::FamilyMember::COUNT; ++family_member_index)
            {
                // RANDOMLY PLACE THE FAMILY MEMBER IN THE CURRENT TILE MAP.
                // The dimensions of the family members are taken into account to ensure they'll be completely on-screen.
                MATH::FloatRectangle current_map_bounds = ark_entrace_tile_map->GetWorldBoundingBox();
                float min_family_member_world_x_position = current_map_bounds.LeftTop.X + OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
                float max_family_member_world_x_position = current_map_bounds.RightBottom.X - OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
                float min_family_member_world_y_position = current_map_bounds.LeftTop.Y + OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
                float max_family_member_world_y_position = current_map_bounds.RightBottom.Y - OBJECTS::FamilyMember::HALF_SIZE_IN_PIXELS;
                // This may have to be repeated multiple times in case the first picked tile isn't walkable.
                // But this isn't expected to result in an infinite or even long-running loop.
                bool family_member_placed_in_world = false;
                while (!family_member_placed_in_world)
                {
                    // COMPUTE A RANDOM POSITION FOR THE FAMILY MEMBER.
                    float family_member_x_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                        min_family_member_world_x_position,
                        max_family_member_world_x_position);
                    float family_member_y_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                        min_family_member_world_y_position,
                        max_family_member_world_y_position);
                        
                    // CHECK IF A WALKABLE TILE EXISTS AT THAT RANDOM LOCATION.
                    // If a walkable tile doesn't exist, we'll just continue trying a different location.
                    std::shared_ptr<MAPS::Tile> tile = ark_entrace_tile_map->GetTileAtWorldPosition(family_member_x_position, family_member_y_position);
                    bool tile_is_walkable = (tile && tile->IsWalkable());
                    if (tile_is_walkable)
                    {
                        // PLACE THE FAMILY MEMBER AT THAT WORLD POSITION.
                        family_member_placed_in_world = true;
                        MATH::Vector2f family_member_world_position(family_member_x_position, family_member_y_position);
                        world.FamilyMembers.emplace_back(
                            static_cast<OBJECTS::FamilyMember::Type>(family_member_index),
                            family_member_world_position,
                            CurrentMapGrid);
                    }
                }
            }

            // RANDOMLY POSITION ANIMALS.
            constexpr float ANIMAL_HALF_SIZE_IN_PIXELS = 16.0f;
            float min_animal_world_x_position = ANIMAL_HALF_SIZE_IN_PIXELS;
            float max_animal_world_x_position = (
                MAPS::Overworld::WIDTH_IN_TILE_MAPS * MAPS::TileMap::WIDTH_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<float>)
                - ANIMAL_HALF_SIZE_IN_PIXELS;
            float min_animal_world_y_position = ANIMAL_HALF_SIZE_IN_PIXELS;
            float max_animal_world_y_position = (
                MAPS::Overworld::HEIGHT_IN_TILE_MAPS * MAPS::TileMap::HEIGHT_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<float>)
                - ANIMAL_HALF_SIZE_IN_PIXELS;
            for (unsigned int animal_species_id = 0; animal_species_id < OBJECTS::AnimalSpecies::COUNT; ++animal_species_id)
            {
                // ADD APPROPRIATE ANIMALS FOR EACH GENDER.
                for (unsigned int animal_gender_id = 0; animal_gender_id < OBJECTS::AnimalGender::COUNT; ++animal_gender_id)
                {
                    // CHECK IF THE ANIMAL HAS BEEN COLLECTED AT ALL IN THE ARK.
                    const INVENTORY::AnimalCollectionStatistics& animal_collection_statistics = saved_game_data.CollectedAnimalsBySpeciesThenGender[animal_species_id][animal_gender_id];
                    unsigned int collected_animal_count = animal_collection_statistics.CollectedTotalCount();
                    bool animal_collected_in_ark = (collected_animal_count > 0);
                    if (!animal_collected_in_ark)
                    {
                        // No animals for this species/gender need to be placed in the ark.
                        continue;
                    }
               
                    // ADD THE APPROPRIATE NUMBER OF ANIMALS.
                    OBJECTS::AnimalSpecies::Value animal_species = static_cast<OBJECTS::AnimalSpecies::Value>(animal_species_id);
                    OBJECTS::AnimalGender::Value animal_gender = static_cast<OBJECTS::AnimalGender::Value>(animal_gender_id);
                    OBJECTS::AnimalType animal_type(animal_species, animal_gender);
                    std::shared_ptr<GRAPHICS::AnimatedSprite> animal_sprite = RESOURCES::AnimalGraphics::GetSprite(animal_type);
                    ASSERT_THEN_IF(animal_sprite)
                    {
                        RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(animal_type.Species);
                        for (unsigned int animal_index = 0; animal_index < collected_animal_count; ++animal_index)
                        {
                            // CREATE THE APPROPRIATE ANIMAL.
                            auto animal = MEMORY::NonNullSharedPointer<OBJECTS::Animal>(std::make_shared<OBJECTS::Animal>(
                                animal_type,
                                *animal_sprite,
                                animal_sound_id));

                            // RANDOMLY POSITION THE ANIMAL.
                            float random_animal_x_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(min_animal_world_x_position, max_animal_world_x_position);
                            float random_animal_y_position = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(min_animal_world_y_position, max_animal_world_y_position);
                            std::shared_ptr<MAPS::Tile> tile = world.Overworld.MapGrid.GetTileAtWorldPosition(
                                random_animal_x_position,
                                random_animal_y_position);
                            // If the tile is not walkable, then the animal just won't be placed.
                            // This should be fine since it the placement of animals here in the post-flood state is to just
                            // give the appearance of animals, rather than having the player count them in detail.
                            bool tile_is_walkable = (tile && tile->IsWalkable());
                            if (tile_is_walkable)
                            {
                                animal->Sprite.SetWorldPosition(random_animal_x_position, random_animal_y_position);
                                animal->Sprite.Play();

                                MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(random_animal_x_position, random_animal_y_position);
                                ASSERT_THEN_IF(current_tile_map)
                                {
                                    ark_entrace_tile_map->RoamingAnimals.emplace_back(animal);
                                }
                            }
                        }
                    }
                }
            }
        }

        // ENSURE ALL ARK PIECES ARE BUILT.
        constexpr bool BUILT = true;
        world.Overworld.SetArkPiecesBuiltStatus(BUILT);

        /// @todo   Destroy all trees?
        /// @todo   Alter tile maps to be different from pre-flood?  Or smaller world?
    }

    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output for the update.
    /// @param[in,out]  world - The world to update based on gameplay.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @return The next game state after updating.
    GameState PostFloodGameplayState::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        STATES::SavedGameData& current_game_data)
    {
        // START PLAYING THE BACKGROUND MUSIC IF ITS NOT ALREADY PLAYING.
        gaming_hardware.Speakers->PlayMusicIfNotAlready(RESOURCES::AssetId::AFTER_RAIN_BACKGROUND_SOUNDS);

        // UPDATE THE CURRENT SUBSTATE.
        // By default, we should remain on the current state unless specific conditions are met.
        GameState next_game_state = GameState::POST_FLOOD_GAMEPLAY;
        switch (CurrentSubstate)
        {
            case Substate::FADING_IN:
            {
                // FADE IN UNTIL WE'VE COMPLETELY FADED IN.
                ElapsedTimeForCurrentSubstate += gaming_hardware.Clock.ElapsedTimeSinceLastFrame;
                float elapsed_time_for_current_substate_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
                bool fading_in_complete = (elapsed_time_for_current_substate_in_seconds >= FADING_MAX_TIME_IN_SECONDS);
                if (fading_in_complete)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::JUST_EXITED_ARK;
                }
                break;
            }
            case Substate::JUST_EXITED_ARK:
            {
                // UPDATE THE MAP GRID.
                UpdateMapGrid(
                    gaming_hardware,
                    world,
                    *CurrentMapGrid,
                    camera,
                    current_game_data);

                // CHECK IF THE PLAYER HAS FINISHED OFFERING SACRIFICES.
                MATH::FloatRectangle camera_bounds = camera.ViewBounds;
                MATH::Vector2f camera_view_center = camera_bounds.Center();
                MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
                ASSERT_THEN_IF(current_tile_map)
                {
                    bool offerings_made = current_tile_map->Altar->OfferingSmoke.Sprite.CurrentFrameSprite.IsVisible;
                    if (offerings_made)
                    {
                        // MOVE TO THE NEXT SUBSTATE.
                        ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                        CurrentSubstate = Substate::GOD_SPEAKING_BEFORE_RAINBOW;

                        // The text below is based on Genesis 8:21-9:11 but hardcoded here for simplicity.
                        // The multiple lines of text are automatically joined.
                        TextBox.StartDisplayingText(
                            "I will not again curse the ground any more for man's sake; for the imagination of man's heart is evil from his youth; neither will I again smite any more every thing living, as I have done.  "
                            "While the earth remaineth, seedtime and harvest, and cold and heat, and summer and winter, and day and night shall not cease.  "
                            "Be fruitful, and multiply, and replenish the earth.  "
                            "And the fear of you and the dread of you shall be upon every beast of the earth, and upon every fowl of the air, upon all that moveth upon the earth, and upon all the fishes of the sea; into your hand are they delivered.  "
                            "Every moving thing that liveth shall be meat for you; even as the green herb have I given you all things.  "
                            "But flesh with the life thereof, which is the blood thereof, shall ye not eat.  "
                            "And surely your blood of your lives will I require; at the hand of every beast will I require it, and at the hand of man; at the hand of every man's brother will I require the life of man.  "
                            "Whoso sheddeth man's blood, by man shall his blood be shed: for in the image of God made he man.  "
                            "And you, be ye fruitful, and multiply; bring forth abundantly in the earth, and multiply therein.  "
                            "And I, behold, I establish my covenant with you, and with your seed after you; "
                            "And with every living creature that is with you, of the fowl, of the cattle, and of every beast of the earth with you; from all that go out of the ark, to every beast of the earth.  "
                            "And I will establish my covenant with you, neither shall all flesh be cut off any more by the waters of a flood; neither shall there any more be a flood to destroy the earth.");

                        // The verses referenced above should be added to the player's inventory if necessary.
                        std::vector<std::pair<unsigned int, unsigned int>> chapter_and_verse_numbers =
                        {
                            { 8, 21 },
                            { 8, 22 },
                            { 9, 1 },
                            { 9, 2 },
                            { 9, 3 },
                            { 9, 4 },
                            { 9, 5 },
                            { 9, 6 },
                            { 9, 7 },
                            { 9, 8 },
                            { 9, 9 },
                            { 9, 10 },
                            { 9, 11 },
                        };
                        for (const auto& [chapter_number, verse_number] : chapter_and_verse_numbers)
                        {
                            auto covenant_verse = std::find_if(
                                current_game_data.BibleVersesLeftToFind.begin(),
                                current_game_data.BibleVersesLeftToFind.end(),
                                [=](const BIBLE::BibleVerse& verse) -> bool
                                {
                                    bool correct_verse_found = (
                                        BIBLE::BibleBook::GENESIS == verse.Book &&
                                        chapter_number == verse.Chapter &&
                                        verse_number == verse.Verse);
                                    return correct_verse_found;
                                });
                            bool verse_found = (current_game_data.BibleVersesLeftToFind.end() != covenant_verse);
                            if (verse_found)
                            {
                                current_game_data.Player->Inventory.BibleVerses.insert(*covenant_verse);
                                current_game_data.BibleVersesLeftToFind.erase(covenant_verse);
                            }
                        }
                    }
                }

                break;
            }
            case Substate::GOD_SPEAKING_BEFORE_RAINBOW:
            {
                // ANIMATE THE OFFERING SMOKE IF APPLICABLE.
                MATH::FloatRectangle camera_bounds = camera.ViewBounds;
                MATH::Vector2f camera_view_center = camera_bounds.Center();
                MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
                ASSERT_THEN_IF(current_tile_map)
                {
                    ASSERT_THEN_IF(current_tile_map->Altar)
                    {
                        current_tile_map->Altar->OfferingSmoke.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                    }

                    // Dust clouds should be animated too.
                    for (auto& dust_cloud : current_tile_map->DustClouds)
                    {
                        dust_cloud.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                    }
                }

                // DISPLAY GOD'S WORDS, ALLOWING THE USER TO MOVE TO DIFFERENT PAGES OF TEXT.
                TextBox.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                bool current_page_of_text_finished = TextBox.CurrentPageOfTextFinishedBeingDisplayed();
                if (current_page_of_text_finished)
                {
                    // MOVE TO THE NEXT PAGE IF THE USER HAS PRESSED THE APPROPRIATE BUTTON.
                    bool user_finished_reading_text_and_pressed_button = gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY);
                    if (user_finished_reading_text_and_pressed_button)
                    {
                        TextBox.MoveToNextPage();
                    }
                }

                // MOVE TO THE NEXT SUBSTATE IF ALL OF THE TEXT FOR THE CURRENT STATE HAS FINISHED.
                if (!TextBox.IsVisible)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::GOD_SPEAKING_DURING_RAINBOW;

                    // The text below is based on Genesis 9:12-17 but hardcoded here for simplicity.
                    // The multiple lines of text are automatically joined.
                    TextBox.StartDisplayingText(
                        "This is the token of the covenant which I make between me and you and every living creature that is with you, for perpetual generations: "
                        "I do set my bow in the cloud, and it shall be for a token of a covenant between me and the earth.  "
                        "And it shall come to pass, when I bring a cloud over the earth, that the bow shall be seen in the cloud: "
                        "And I will remember my covenant, which is between me and you and every living creature of all flesh; and the waters shall no more become a flood to destroy all flesh.  "
                        "And the bow shall be in the cloud; and I will look upon it, that I may remember the everlasting covenant between God and every living creature of all flesh that is upon the earth.  "
                        "This is the token of the covenant, which I have established between me and all flesh that is upon the earth.");

                    // The verses referenced above should be added to the player's inventory if necessary.
                    std::vector<std::pair<unsigned int, unsigned int>> chapter_and_verse_numbers =
                    {
                        { 9, 12 },
                        { 9, 13 },
                        { 9, 14 },
                        { 9, 15 },
                        { 9, 16 },
                        { 9, 17 },
                    };
                    for (const auto& [chapter_number, verse_number] : chapter_and_verse_numbers)
                    {
                        auto covenant_verse = std::find_if(
                            current_game_data.BibleVersesLeftToFind.begin(),
                            current_game_data.BibleVersesLeftToFind.end(),
                            [=](const BIBLE::BibleVerse& verse) -> bool
                            {
                                bool correct_verse_found = (
                                    BIBLE::BibleBook::GENESIS == verse.Book &&
                                    chapter_number == verse.Chapter &&
                                    verse_number == verse.Verse);
                                return correct_verse_found;
                            });
                        bool verse_found = (current_game_data.BibleVersesLeftToFind.end() != covenant_verse);
                        if (verse_found)
                        {
                            current_game_data.Player->Inventory.BibleVerses.insert(*covenant_verse);
                            current_game_data.BibleVersesLeftToFind.erase(covenant_verse);
                        }
                    }
                }
                break;
            }
            case Substate::GOD_SPEAKING_DURING_RAINBOW:
            {
                // ANIMATE THE OFFERING SMOKE IF APPLICABLE.
                MATH::FloatRectangle camera_bounds = camera.ViewBounds;
                MATH::Vector2f camera_view_center = camera_bounds.Center();
                MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
                ASSERT_THEN_IF(current_tile_map)
                {
                    ASSERT_THEN_IF(current_tile_map->Altar)
                    {
                        current_tile_map->Altar->OfferingSmoke.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                    }

                    // Dust clouds should be animated too.
                    for (auto& dust_cloud : current_tile_map->DustClouds)
                    {
                        dust_cloud.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                    }
                }

                // DISPLAY GOD'S WORDS, ALLOWING THE USER TO MOVE TO DIFFERENT PAGES OF TEXT.
                TextBox.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                bool current_page_of_text_finished = TextBox.CurrentPageOfTextFinishedBeingDisplayed();
                if (current_page_of_text_finished)
                {
                    // MOVE TO THE NEXT PAGE IF THE USER HAS PRESSED THE APPROPRIATE BUTTON.
                    bool user_finished_reading_text_and_pressed_button = gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY);
                    if (user_finished_reading_text_and_pressed_button)
                    {
                        TextBox.MoveToNextPage();
                    }
                }

                // MOVE TO THE NEXT SUBSTATE IF ALL OF THE TEXT FOR THE CURRENT STATE HAS FINISHED.
                if (!TextBox.IsVisible)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::FADING_OUT;
                }
                break;
            }
            case Substate::FADING_OUT:
            {
                // FADE OUT UNTIL WE'VE COMPLETELY FADED IN.
                ElapsedTimeForCurrentSubstate += gaming_hardware.Clock.ElapsedTimeSinceLastFrame;
                float elapsed_time_for_current_substate_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
                bool fading_out_complete = (elapsed_time_for_current_substate_in_seconds >= FADING_MAX_TIME_IN_SECONDS);
                if (fading_out_complete)
                {
                    // MOVE TO THE NEXT REAL GAME STATE.
                    next_game_state = GameState::ENDING_CREDITS_SCREEN;
                }
                break;
            }
        }

        // RETURN THE NEXT GAME STATE.
        return next_game_state;
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  world - The world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @param[in]  gaming_hardware - The gaming hardware.
    /// @return The rendered gameplay state.
    sf::Sprite PostFloodGameplayState::Render(
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        HARDWARE::GamingHardware& gaming_hardware)
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

        // RE-RENDER SMOKE CLOUDS TO ENSURE THEY APPEAR ON TOP OF NOAH.
        // This is a hack, but it is not yet worth re-writing other things for proper z-ordering.
        MATH::FloatRectangle camera_bounds = renderer.Camera.ViewBounds;
        MATH::Vector2f camera_view_center = camera_bounds.Center();
        MAPS::TileMap* current_tile_map = CurrentMapGrid->GetTileMap(camera_view_center.X, camera_view_center.Y);
        // Only needed if Noah is facing down.
        bool noah_facing_down = (GAMEPLAY::Direction::DOWN == world.NoahPlayer->FacingDirection);
        if (noah_facing_down)
        {
            ASSERT_THEN_IF(current_tile_map)
            {
                if (current_tile_map->Altar)
                {
                    renderer.Render(current_tile_map->Altar->Sprite);

                    // Any smoke should also be rendered.
                    if (current_tile_map->Altar->OfferingSmoke.Sprite.CurrentFrameSprite.IsVisible)
                    {
                        renderer.Render(current_tile_map->Altar->OfferingSmoke.Sprite.CurrentFrameSprite);
                    }
                }

                // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
                for (auto& dust_cloud : current_tile_map->DustClouds)
                {
                    bool dust_cloud_visible = !dust_cloud.HasDisappeared();
                    if (dust_cloud_visible)
                    {
                        renderer.Render(dust_cloud.Sprite.CurrentFrameSprite);
                    }
                }
            }
        }

        // RENDER APPROPRIATE SHADING EFFECTS FOR CERTAIN SUBSTATES.
        sf::RenderStates shading_effect = sf::RenderStates::Default;

        float elapsed_time_for_fading_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
        float current_ratio_through_current_fade = elapsed_time_for_fading_in_seconds / FADING_MAX_TIME_IN_SECONDS;
        switch (CurrentSubstate)
        {
            case Substate::FADING_IN:
            {
                GRAPHICS::Color current_tint_for_fading = GRAPHICS::Color::WHITE;
                current_tint_for_fading.ScaleRgb(current_ratio_through_current_fade);
                std::shared_ptr<sf::Shader> colored_texture_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
                ASSERT_THEN_IF(colored_texture_shader)
                {
                    colored_texture_shader->setUniform("color", sf::Glsl::Vec4(sf::Color(current_tint_for_fading.Red, current_tint_for_fading.Green, current_tint_for_fading.Blue, current_tint_for_fading.Alpha)));
                    colored_texture_shader->setUniform("texture", sf::Shader::CurrentTexture);
                    shading_effect.shader = colored_texture_shader.get();
                }
                break;
            }
            case Substate::JUST_EXITED_ARK:
            {
                // RENDER INSTRUCTIONS FOR BUILDING AN ALTAR IF IT HAS NOT BEEN BUILT YET.
                ASSERT_THEN_IF(current_tile_map)
                {
                    bool altar_built = current_tile_map->Altar.has_value();
                    if (!altar_built)
                    {
                        // CHECK IF AN ALTAR CAN BE BUILT AT THE CURRENT LOCATION.
                        MATH::Vector2f altar_center_world_position = GetAltarBuildPosition(*world.NoahPlayer);
                        std::shared_ptr<MAPS::Tile> tile_where_altar_would_be_built = current_tile_map->GetTileAtWorldPosition(altar_center_world_position.X, altar_center_world_position.Y);
                        // The tile map not exist if on the very edge of the map.
                        if (tile_where_altar_would_be_built)
                        {
                            bool ark_can_be_built_on_tile = tile_where_altar_would_be_built->IsWalkable() && !MAPS::TileType::IsForArk(tile_where_altar_would_be_built->Type);
                            if (ark_can_be_built_on_tile)
                            {
                                // RENDER AN ICON WITH INFORMATION FOR BUILDING THE ALTAR.
                                constexpr char BUILD_ALTAR_KEY_TEXT = INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY_TEXT;
                                const MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
                                renderer.RenderKeyIcon(BUILD_ALTAR_KEY_TEXT, TOP_LEFT_SCREEN_POSITION_IN_PIXELS);

                                constexpr float KEY_ICON_WIDTH_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS);
                                MATH::Vector2f build_altar_text_left_top_screen_position_in_pixels(KEY_ICON_WIDTH_IN_PIXELS, 0.0f);
                                renderer.RenderText(
                                    "Build Altar",
                                    RESOURCES::AssetId::FONT_TEXTURE,
                                    build_altar_text_left_top_screen_position_in_pixels);
                            }
                        }
                    }
                    else if (altar_built)
                    {
                        // CHECK IF THE PLAYER IS FACING THE ALTAR.
                        MATH::Vector2f place_in_front_of_player = GetAltarBuildPosition(*world.NoahPlayer);
                        MATH::FloatRectangle altar_bounding_box = current_tile_map->Altar->Sprite.GetWorldBoundingBox();
                        bool player_facing_altar = altar_bounding_box.Contains(place_in_front_of_player.X, place_in_front_of_player.Y);
                        if (player_facing_altar)
                        {
                            // RENDER AN ICON WITH INFORMATION FOR OFFERING SACRIFICES.
                            constexpr char MAKE_OFFERINGS_KEY_TEXT = INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY_TEXT;
                            const MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
                            renderer.RenderKeyIcon(MAKE_OFFERINGS_KEY_TEXT, TOP_LEFT_SCREEN_POSITION_IN_PIXELS);

                            constexpr float KEY_ICON_WIDTH_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS);
                            MATH::Vector2f make_offerings_text_left_top_screen_position_in_pixels(KEY_ICON_WIDTH_IN_PIXELS, 0.0f);
                            renderer.RenderText(
                                "Make Offerings",
                                RESOURCES::AssetId::FONT_TEXTURE,
                                make_offerings_text_left_top_screen_position_in_pixels);
                        }
                    }
                }
                break;
            }
            case Substate::GOD_SPEAKING_DURING_RAINBOW:
            {
                // ADD A RAINBOW EFFECT.
                std::shared_ptr<sf::Shader> rainbow_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::RAINBOW_SHADER);
                ASSERT_THEN_IF(rainbow_shader)
                {
                    // COMPUTE THE LIGHTING FOR THE SHADER.
                    // The rainbow's intensity should pulse based on elapsed time.
                    float elapsed_time_in_seconds = gaming_hardware.Clock.TotalElapsedTime.asSeconds();
                    // The range of sin() is [-1, 1].  We want the alpha to be in the range of [0.4, 0.8]
                    // (a very subtle pulse).
                    // The initial multiplication brings the range to [-0.2, 0.2].
                    constexpr float ADDITIONAL_ALPHA_RANGE = 0.2f;
                    float alpha_for_rainbow = ADDITIONAL_ALPHA_RANGE * std::sinf(elapsed_time_in_seconds);
                    // An addition shifts it into the appropriate range.
                    constexpr float ADDITIONAL_ALPHA_SHIFT_AMOUNT = 0.6f;
                    alpha_for_rainbow += ADDITIONAL_ALPHA_SHIFT_AMOUNT;

                    rainbow_shader->setUniform("alpha_for_rainbow", alpha_for_rainbow);
                    rainbow_shader->setUniform("texture", sf::Shader::CurrentTexture);
                    shading_effect.shader = rainbow_shader.get();
                }
                break;
            }
            case Substate::FADING_OUT:
            {
                // ADD A RAINBOW EFFECT.
                // This should still persist even during fading out.
                std::shared_ptr<sf::Shader> rainbow_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::RAINBOW_SHADER);
                ASSERT_THEN_IF(rainbow_shader)
                {
                    // COMPUTE THE LIGHTING FOR THE SHADER.
                    // The rainbow's intensity should pulse based on elapsed time.
                    float elapsed_time_in_seconds = gaming_hardware.Clock.TotalElapsedTime.asSeconds();
                    // The range of sin() is [-1, 1].  We want the alpha to be in the range of [0.4, 0.8]
                    // (a very subtle pulse).
                    // The initial multiplication brings the range to [-0.2, 0.2].
                    constexpr float ADDITIONAL_ALPHA_RANGE = 0.2f;
                    float alpha_for_rainbow = ADDITIONAL_ALPHA_RANGE * std::sinf(elapsed_time_in_seconds);
                    // An addition shifts it into the appropriate range.
                    constexpr float ADDITIONAL_ALPHA_SHIFT_AMOUNT = 0.6f;
                    alpha_for_rainbow += ADDITIONAL_ALPHA_SHIFT_AMOUNT;

                    rainbow_shader->setUniform("alpha_for_rainbow", alpha_for_rainbow);
                    rainbow_shader->setUniform("texture", sf::Shader::CurrentTexture);
                    shading_effect.shader = rainbow_shader.get();

                    // The screen needs to be rendered with this effect before applying the next.
                    renderer.RenderFinalScreen(shading_effect);
                }

                // FADE THE SCENE OUT.
                constexpr float MAX_RATIO = 1.0f;
                float current_ratio_remaining_for_fade = MAX_RATIO - current_ratio_through_current_fade;
                GRAPHICS::Color current_tint_for_fading = GRAPHICS::Color::WHITE;
                current_tint_for_fading.ScaleRgb(current_ratio_remaining_for_fade);
                std::shared_ptr<sf::Shader> colored_texture_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
                ASSERT_THEN_IF(colored_texture_shader)
                {
                    colored_texture_shader->setUniform("color", sf::Glsl::Vec4(sf::Color(current_tint_for_fading.Red, current_tint_for_fading.Green, current_tint_for_fading.Blue, current_tint_for_fading.Alpha)));
                    colored_texture_shader->setUniform("texture", sf::Shader::CurrentTexture);
                    shading_effect.shader = colored_texture_shader.get();
                }
                break;
            }
        }

        // RENDER THE SCREEN WITH ANY SPECIAL EFFECTS.
        sf::Sprite screen = renderer.RenderFinalScreen(shading_effect);

        // RENDER THE TEXT BOX ON TOP IF IT'S VISIBLE.
        if (TextBox.IsVisible)
        {
            TextBox.Render(renderer);
        }

        return screen;
    }

    /// Gets the center world position in which the altar would be built.
    /// @param[in]  noah_player - The Noah player in front of which the altar would be built.
    /// @return The center world position for building the altar.
    MATH::Vector2f PostFloodGameplayState::GetAltarBuildPosition(const OBJECTS::Noah& noah_player) const
    {
        MATH::Vector2f altar_center_world_position = noah_player.GetWorldPosition();

        // The altar is a little wider than Noah, rather than being a square,
        // so some addiitional horizontal adjustments are needed.
        constexpr float ALTAR_HALF_WIDTH_IN_PIXELS = 16.0f;
        MATH::FloatRectangle noah_bounding_box = noah_player.GetWorldBoundingBox();
        switch (noah_player.FacingDirection)
        {
            case GAMEPLAY::Direction::UP:
                altar_center_world_position.Y -= noah_bounding_box.Height();
                break;
            case GAMEPLAY::Direction::DOWN:
                altar_center_world_position.Y += noah_bounding_box.Height();
                break;
            case GAMEPLAY::Direction::LEFT:
                altar_center_world_position.X -= (noah_bounding_box.Width() + ALTAR_HALF_WIDTH_IN_PIXELS);
                break;
            case GAMEPLAY::Direction::RIGHT:
                altar_center_world_position.X += (noah_bounding_box.Width() + ALTAR_HALF_WIDTH_IN_PIXELS);
                break;
        }

        return altar_center_world_position;
    }

    /// Updates a map grid based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output.
    /// @param[in,out]  world - The world being updated.
    /// @param[in,out]  map_grid - The map grid to update.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @param[in,out]  current_game_data - The current game data to use and update.
    void PostFloodGameplayState::UpdateMapGrid(
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

        // UPDATE THE PLAYER BASED ON INPUT.
        UpdatePlayerBasedOnInput(
            world,
            *current_tile_map,
            map_grid,
            camera,
            gaming_hardware);

        // UPDATE THE REST OF THE WORLD WITHIN CURRENT TILE MAP.
        // Movement is disallowed to prevent animals from moving around.
        constexpr bool OBJECTS_CANNOT_MOVE = false;
        current_tile_map->Update(OBJECTS_CANNOT_MOVE, current_game_data, gaming_hardware);

        // UPDATE THE CAMERA'S WORLD VIEW.
        UpdateCameraWorldView(
            gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
            camera,
            gaming_hardware.InputController,
            *current_tile_map);
    }

    /// Updates the player and related items in the tile map based on input and elapsed time.
    /// @param[in,out]  world - The world in which the player is being updated.
    /// @param[in,out]  current_tile_map - The tile map the player is currently located in.
    /// @param[in,out]  map_grid - The map grid containing the current tile map.
    /// @param[in,out]  camera - The camera defining the viewable region of the map grid.
    /// @param[in,out]  gaming_hardware - The gaming hardware.
    void PostFloodGameplayState::UpdatePlayerBasedOnInput(
        MAPS::World& world,
        MAPS::TileMap& current_tile_map,
        MAPS::MultiTileMapGrid& map_grid,
        GRAPHICS::Camera& camera,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // ALLOW NOAH TO BUILD AN ALTAR OR OFFER SACRIFICES.
        MATH::FloatRectangle camera_bounds = camera.ViewBounds;
        bool altar_built = current_tile_map.Altar.has_value();
        if (!altar_built)
        {
            if (gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
            {
                // BUILD AN ALTAR IN FRONT OF NOAH IN THE DIRECTION HE IS FACING IF POSSIBLE.
                MATH::Vector2f altar_center_world_position = GetAltarBuildPosition(*world.NoahPlayer);
                std::shared_ptr<MAPS::Tile> tile_where_altar_would_be_built = current_tile_map.GetTileAtWorldPosition(altar_center_world_position.X, altar_center_world_position.Y);
                ASSERT_THEN_IF(tile_where_altar_would_be_built)
                {
                    bool ark_can_be_built_on_tile = tile_where_altar_would_be_built->IsWalkable() && !MAPS::TileType::IsForArk(tile_where_altar_would_be_built->Type);
                    if (ark_can_be_built_on_tile)
                    {
                        // BUILD THE ALTAR.
                        current_tile_map.Altar = OBJECTS::Altar(altar_center_world_position);

                        // ADD SOME DUST CLOUDS FOR THE ALTAR BEING BUILT.
                        constexpr unsigned int DUST_CLOUD_COUNT = 3;
                        for (unsigned int dust_cloud_index = 0; dust_cloud_index < DUST_CLOUD_COUNT; ++dust_cloud_index)
                        {
                            // RANDOMLY ADD THE DUST CLOUD NEAR THE ALTAR.
                            OBJECTS::DustCloud dust_cloud(RESOURCES::AssetId::DUST_CLOUD_TEXTURE);

                            constexpr float DUST_CLOUD_POSITION_MAX_OFFSET = 16.0f;
                            float dust_cloud_x_offset = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(-DUST_CLOUD_POSITION_MAX_OFFSET, DUST_CLOUD_POSITION_MAX_OFFSET);
                            float dust_cloud_y_offset = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(-DUST_CLOUD_POSITION_MAX_OFFSET, DUST_CLOUD_POSITION_MAX_OFFSET);

                            MATH::Vector2f dust_cloud_world_position = altar_center_world_position;
                            dust_cloud_world_position.X += dust_cloud_x_offset;
                            dust_cloud_world_position.Y += dust_cloud_y_offset;
                            dust_cloud.Sprite.SetWorldPosition(dust_cloud_world_position);

                            // The dust cloud should start animating immediately.
                            dust_cloud.Sprite.Play();

                            current_tile_map.DustClouds.push_back(dust_cloud);
                        }
                    }
                }
            }
        }
        else if (altar_built)
        {
            if (gaming_hardware.InputController.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
            {
                // CHECK IF THE PLAYER IS FACING THE ALTAR.
                MATH::Vector2f place_in_front_of_player = GetAltarBuildPosition(*world.NoahPlayer);
                MATH::FloatRectangle altar_bounding_box = current_tile_map.Altar->Sprite.GetWorldBoundingBox();
                bool player_facing_altar = altar_bounding_box.Contains(place_in_front_of_player.X, place_in_front_of_player.Y);
                if (player_facing_altar)
                {
                    // MAKE OFFERINGS.
                    current_tile_map.Altar->OfferingSmoke.Sprite.CurrentFrameSprite.IsVisible = true;
                    current_tile_map.Altar->OfferingSmoke.Sprite.Play();
                }
            }
        }

        // GET THE TILE UNDER NOAH.
        // This is needed to help track if Noah moves onto a different type of tile.
        MATH::Vector2f old_noah_position = world.NoahPlayer->GetWorldPosition();
        std::shared_ptr<MAPS::Tile> original_tile_under_noah = current_tile_map.GetTileAtWorldPosition(
            old_noah_position.X,
            old_noah_position.Y);

        // MOVE NOAH IN RESPONSE TO USER INPUT.
        bool noah_moved_this_frame = false;
        const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
        if (gaming_hardware.InputController.ButtonDown(sf::Keyboard::Up))
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
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
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
                    gaming_hardware.InputController.DisableInput();
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
        if (gaming_hardware.InputController.ButtonDown(sf::Keyboard::Down))
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
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
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
                    gaming_hardware.InputController.DisableInput();
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
        if (gaming_hardware.InputController.ButtonDown(sf::Keyboard::Left))
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
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
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
                    gaming_hardware.InputController.DisableInput();
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
        if (gaming_hardware.InputController.ButtonDown(sf::Keyboard::Right))
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
                gaming_hardware.Clock.ElapsedTimeSinceLastFrame,
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
                    gaming_hardware.InputController.DisableInput();
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
            world.NoahPlayer->Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
        }
        else
        {
            // STOP NOAH'S ANIMATION FROM PLAYING SINCE THE PLAYER DIDN'T MOVE THIS FRAME.
            world.NoahPlayer->Sprite.ResetAnimation();
        }
    }

    /// Updates the camera, along with the world based on any major changes
    /// in what the camera is viewing.
    /// @param[in]  elapsed_time - The elapsed time by which to update the camera.
    /// @param[in,out]  camera - The camera to update.
    /// @param[in,out]  input_controller - The input controller that might be tweaked based on camera movement.
    /// @param[in,out]  current_tile_map - The current tile map in view by the camera.
    void PostFloodGameplayState::UpdateCameraWorldView(
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
