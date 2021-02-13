#include <cassert>
#include "Debugging/DebugConsole.h"
#include "States/GameStates.h"

namespace STATES
{
    /// Updates the current state of the game.
    /// @param[in,out]  world - The world to be updated.
    /// @param[in,out]  camera - The camera defining the viewable region of the game world.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @return The next state that the game should switch to.  The current state as tracked
    ///     in this class is not automatically updated to this new state.
    GameState GameStates::Update(
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // UPDATE THE CURRENT STATE OF THE GAME.
        GameState next_game_state = CurrentSavedGame.CurrentGameState;
        switch (CurrentSavedGame.CurrentGameState)
        {
            case GameState::INTRO_SEQUENCE:
                next_game_state = IntroSequence.Update(gaming_hardware);
                break;
            case GameState::TITLE_SCREEN:
                next_game_state = TitleScreen.Update(gaming_hardware);
                break;
            case GameState::CREDITS_SCREEN:
                next_game_state = CreditsScreen.Update(gaming_hardware);
                break;
            case GameState::GAME_SELECTION_SCREEN:
                next_game_state = GameSelectionScreen.Update(gaming_hardware);
                break;
            case GameState::NEW_GAME_INTRO_SEQUENCE:
                next_game_state = NewGameIntroSequence.Update(gaming_hardware);
                break;
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                next_game_state = NewGameInstructionSequence.Update(gaming_hardware);
                break;
            case GameState::FLOOD_CUTSCENE:
                next_game_state = FloodCutscene.Update(gaming_hardware);
                break;
            case GameState::PRE_FLOOD_GAMEPLAY:
                next_game_state = PreFloodGameplayState.Update(
                    gaming_hardware,
                    world,
                    camera,
                    CurrentSavedGame);
                break;
            case GameState::DURING_FLOOD_GAMEPLAY:
                next_game_state = DuringFloodGameplayState.Update(
                    gaming_hardware,
                    world,
                    camera,
                    CurrentSavedGame);
                break;
            case GameState::POST_FLOOD_GAMEPLAY:
                next_game_state = PostFloodGameplayState.Update(
                    gaming_hardware,
                    world,
                    camera,
                    CurrentSavedGame);
                break;
            case GameState::ENDING_CREDITS_SCREEN:
                next_game_state = EndingCreditsScreen.Update(gaming_hardware);
                break;
        }

        // RETURN THE NEXT STATE THE GAME SHOULD BE IN.
        return next_game_state;
    }

    /// Renders the current state of the game.
    /// @param[in,out]  world - The game world to potentially render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @return The rendered state of the game.
    sf::Sprite GameStates::Render(
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // CLEAR THE SCREEN OF THE PREVIOUSLY RENDERED FRAME.
        renderer.Screen->Clear();

        // RENDER THE CURRENT GAME STATE.
        sf::Sprite screen_sprite;
        switch (CurrentSavedGame.CurrentGameState)
        {
            case GameState::INTRO_SEQUENCE:
                screen_sprite = IntroSequence.Render(renderer);
                break;
            case GameState::TITLE_SCREEN:
                screen_sprite = TitleScreen.Render(renderer);
                break;
            case GameState::CREDITS_SCREEN:
                screen_sprite = CreditsScreen.Render(renderer);
                break;
            case GameState::GAME_SELECTION_SCREEN:
                screen_sprite = GameSelectionScreen.Render(renderer);
                break;
            case GameState::NEW_GAME_INTRO_SEQUENCE:
                screen_sprite = NewGameIntroSequence.Render(renderer);
                break;
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                screen_sprite = NewGameInstructionSequence.Render(world, renderer, gaming_hardware);
                break;
            case GameState::FLOOD_CUTSCENE:
                screen_sprite = FloodCutscene.Render(renderer, gaming_hardware);
                break;
            case GameState::PRE_FLOOD_GAMEPLAY:
                screen_sprite = PreFloodGameplayState.Render(world, CurrentSavedGame, renderer);
                break;
            case GameState::DURING_FLOOD_GAMEPLAY:
                screen_sprite = DuringFloodGameplayState.Render(world, CurrentSavedGame, renderer);
                break;
            case GameState::POST_FLOOD_GAMEPLAY:
                screen_sprite = PostFloodGameplayState.Render(world, CurrentSavedGame, renderer);
                break;
            case GameState::ENDING_CREDITS_SCREEN:
                screen_sprite = EndingCreditsScreen.Render(renderer);
                break;
        }
        return screen_sprite;
    }

    /// Switches to the new state, if the state has changed.
    /// @param[in]  new_state - The potentially new state the game should be in.
    /// @param[in]  world - The game world needed for some states.
    /// @param[in,out]  renderer - The renderer used for the game.
    /// @param[in,out]  gaming_hardware - The hardware the game is being played on.
    void GameStates::SwitchStatesIfChanged(
        const GameState& new_state, 
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // CHECK IF THE GAME STATE HAS CHANGED.
        bool game_state_changed = (new_state != CurrentSavedGame.CurrentGameState);
        if (!game_state_changed)
        {
            // The game is already in the correct state.
            return;
        }

        // STOP ALL AUDIO TO PREVENT IT FROM BLEEDING BETWEEN STATES.
        gaming_hardware.Speakers->StopAllAudio();

        // UPDATE THE SAVED GAME DATA BEING USED IF A SAVED GAME IS BEING LOADED.
        bool saved_game_being_loaded = (GameState::GAME_SELECTION_SCREEN == CurrentSavedGame.CurrentGameState);
        if (saved_game_being_loaded)
        {
            // LOAD THE GAME'S SAVE FILE.
            // The empty check is for handling the case where no saved games exist yet, primarily when
            // switching between states in debug mode.
            if (!GameSelectionScreen.SavedGames.empty())
            {
                CurrentSavedGame = *GameSelectionScreen.SavedGames.at(GameSelectionScreen.SelectedGameIndex);
            }

            // UPDATE NOAH PLAYER INFORMATION.
            world.NoahPlayer = CurrentSavedGame.Player;
        }

        // CHANGE THE GAME'S STATE.
        DEBUGGING::DebugConsole::WriteLine("Changing state to ", new_state);
        CurrentSavedGame.CurrentGameState = new_state;
        switch (CurrentSavedGame.CurrentGameState)
        {
            case GameState::INTRO_SEQUENCE:
                IntroSequence.Start(*gaming_hardware.Speakers);
                break;
            case GameState::CREDITS_SCREEN:
                // RESET THE ELAPSED TIME FOR THE CREDITS SCREEN.
                CreditsScreen.ElapsedTime = sf::Time::Zero;
                break;
            case GameState::GAME_SELECTION_SCREEN:
                GameSelectionScreen.LoadSavedGames();
                GameSelectionScreen.CurrentSubState = GameSelectionScreen::SubState::LISTING_GAMES;
                break;
            case GameState::NEW_GAME_INTRO_SEQUENCE:
                // RESET THE INTRO SEQUENCE TO THE BEGINNING.
                NewGameIntroSequence = {};
                world.NoahPlayer->SetWorldPosition(OBJECTS::Noah::DEFAULT_START_WORLD_POSITION);
                break;
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
            {
                unsigned int main_text_box_width_in_pixels = renderer.Screen->WidthInPixels<unsigned int>();
                const unsigned int LINE_COUNT = 2;
                unsigned int main_text_box_height_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS * LINE_COUNT;
                /// @todo   Rethink this text box initialization...It has to be done before the rest of initialization.
                NewGameInstructionSequence.InstructionTextBox = GRAPHICS::GUI::TextBox(
                    main_text_box_width_in_pixels,
                    main_text_box_height_in_pixels,
                    renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE]);
                NewGameInstructionSequence.Initialize(world);
                renderer.Camera.SetCenter(world.NoahPlayer->GetWorldPosition());
                break;
            }
            case GameState::FLOOD_CUTSCENE:
                FloodCutscene = STATES::FloodCutscene();
                break;
            case GameState::PRE_FLOOD_GAMEPLAY:
            {
                // INITIALIZE THE GAMEPLAY STATE.
                bool gameplay_state_initialized = PreFloodGameplayState.Initialize(
                    CurrentSavedGame,
                    world,
                    renderer,
                    gaming_hardware.RandomNumberGenerator);
                if (!gameplay_state_initialized)
                {
                    /// @todo   Error-handling!
                    return;
                }

                break;
            }
            case GameState::DURING_FLOOD_GAMEPLAY:
            {
                // INITIALIZE THE WORLD.
                DuringFloodGameplayState.CurrentMapGrid = &world.Ark.Interior.LayersFromBottomToTop[MAPS::Ark::LOWEST_LAYER_INDEX];

                // During the flood, all family members should appear randomly in the ark.
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

                // MOVE THE PLAYER INTO THE ENTRANCE.
                std::shared_ptr<MAPS::TileMap> entrance_map = world.Ark.GetEntranceMap();
                MATH::Vector2f entrance_map_center_position = entrance_map->GetCenterWorldPosition();
                world.NoahPlayer->SetWorldPosition(entrance_map_center_position);
                renderer.Camera.SetCenter(entrance_map_center_position);

                /// @todo CLOSE THE EXITWAY FROM THE ARK.

                // INITIALIZE THE HUD.
                unsigned int main_text_box_width_in_pixels = renderer.Screen->WidthInPixels<unsigned int>();
                const unsigned int LINE_COUNT = 2;
                unsigned int main_text_box_height_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS * LINE_COUNT;
                DuringFloodGameplayState.Hud = GRAPHICS::GUI::HeadsUpDisplay(
                    renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE],
                    main_text_box_width_in_pixels,
                    main_text_box_height_in_pixels);
                break;
            }
            case GameState::POST_FLOOD_GAMEPLAY:
            {
                // INITIALIZE THE WORLD.
                PostFloodGameplayState.CurrentMapGrid = &world.Overworld.MapGrid;

                // The ark should be completely built at this point.
                unsigned int tile_map_row_count = PostFloodGameplayState.CurrentMapGrid->TileMaps.GetHeight();
                unsigned int tile_map_column_count = PostFloodGameplayState.CurrentMapGrid->TileMaps.GetWidth();
                for (unsigned int tile_map_row_index = 0; tile_map_row_index < tile_map_row_count; ++tile_map_row_index)
                {
                    for (unsigned int tile_map_column_index = 0; tile_map_column_index < tile_map_column_count; ++tile_map_column_index)
                    {
                        // BUILD ALL ARK PIECES IN THE CURRENT TILE MAP.
                        MAPS::TileMap* tile_map = PostFloodGameplayState.CurrentMapGrid->GetTileMap(tile_map_row_index, tile_map_column_index);
                        assert(tile_map);
                        for (OBJECTS::ArkPiece& ark_piece : tile_map->ArkPieces)
                        {
                            ark_piece.Built = true;
                        }
                    }
                }

                // FOCUS THE CAMERA ON THE PLAYER.
                // The player is positioned at the exit of the ark
                constexpr unsigned int ARK_EXIT_TILE_MAP_ROW_INDEX = 0;
                constexpr unsigned int ARK_EXIT_TILE_MAP_COLUMN_INDEX = 2;
                MAPS::TileMap* ark_exit_tile_map = PostFloodGameplayState.CurrentMapGrid->GetTileMap(ARK_EXIT_TILE_MAP_ROW_INDEX, ARK_EXIT_TILE_MAP_COLUMN_INDEX);
                assert(ark_exit_tile_map);
                MATH::Vector2f ark_exit_map_center_world_position = ark_exit_tile_map->GetCenterWorldPosition();
                world.NoahPlayer->SetWorldPosition(ark_exit_map_center_world_position);
                MATH::Vector2f player_start_world_position = world.NoahPlayer->GetWorldPosition();
                renderer.Camera.SetCenter(player_start_world_position);
                break;
            }
            case GameState::ENDING_CREDITS_SCREEN:
            {
                EndingCreditsScreen = {};
                break;
            }
        }
    }
}
