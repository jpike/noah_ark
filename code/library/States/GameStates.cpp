#include "States/GameStates.h"

namespace STATES
{
    /// Updates the current state of the game.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @param[in,out]  camera - The camera defining the viewable region of the game world.
    /// @return The next state that the game should switch to.  The current state as tracked
    ///     in this class is not automatically updated to this new state.
    GameState GameStates::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        GRAPHICS::Camera& camera)
    {
        // UPDATE THE CURRENT STATE OF THE GAME.
        GameState next_game_state = CurrentGameState;
        switch (CurrentGameState)
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
            case GameState::FLOOD_CUTSCENE:
                next_game_state = FloodCutscene.Update(gaming_hardware);
                break;
            case GameState::GAMEPLAY:
                next_game_state = GameplayState.Update(
                    gaming_hardware,
                    camera);
                break;
        }

        // RETURN THE NEXT STATE THE GAME SHOULD BE IN.
        return next_game_state;
    }

    /// Renders the current state of the game.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered state of the game.
    sf::Sprite GameStates::Render(HARDWARE::GamingHardware& gaming_hardware, GRAPHICS::Renderer& renderer)
    {
        // CLEAR THE SCREEN OF THE PREVIOUSLY RENDERED FRAME.
        renderer.Screen->Clear();

        // RENDER THE CURRENT GAME STATE.
        sf::Sprite screen_sprite;
        switch (CurrentGameState)
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
            case GameState::FLOOD_CUTSCENE:
                screen_sprite = FloodCutscene.Render(renderer);
                break;
            case GameState::GAMEPLAY:
                screen_sprite = GameplayState.Render(gaming_hardware.Clock.TotalElapsedTime, renderer);
                break;
        }
        return screen_sprite;
    }

    /// Switches to the new state, if the state has changed.
    /// @param[in]  new_state - The potentially new state the game should be in.
    /// @param[in]  world - The game world needed for some states.
    /// @param[in,out]  renderer - The renderer used for the game.
    /// @todo   Clean-up parameters!
    void GameStates::SwitchStatesIfChanged(
        const GameState& new_state, 
        const std::shared_ptr<MAPS::World>& world,
        GRAPHICS::Renderer& renderer)
    {
        // CHECK IF THE GAME STATE HAS CHANGED.
        bool game_state_changed = (new_state != CurrentGameState);
        if (!game_state_changed)
        {
            // The game is already in the correct state.
            return;
        }

        // CHANGE THE GAME'S STATE.
        CurrentGameState = new_state;
        switch (CurrentGameState)
        {
            case GameState::CREDITS_SCREEN:
                // RESET THE ELAPSED TIME FOR THE CREDITS SCREEN.
                CreditsScreen.ElapsedTime = sf::Time::Zero;
                break;
            case GameState::GAME_SELECTION_SCREEN:
                GameSelectionScreen.LoadSavedGames();
                GameSelectionScreen.CurrentSubState = GameSelectionScreen::SubState::LISTING_GAMES;
                break;
            case GameState::NEW_GAME_INTRO_SEQUENCE:
            {
                // RESET THE INTRO SEQUENCE TO THE BEGINNING.
                NewGameIntroSequence.ResetToBeginning();
                break;
            }
            case GameState::FLOOD_CUTSCENE:
            {
                // RESET THE ELAPSED TIME FOR THE CUTSCENE.
                FloodCutscene.ElapsedTime = sf::Time::Zero;

                // INITIALIZE THE CUTSCENE SPRITES.
                /// @todo   Cleaner way to do this!
                MATH::FloatRectangle mountain_sprite_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 256, 256);
                FloodCutscene.Mountain = GRAPHICS::Sprite(RESOURCES::AssetId::FLOOD_CUTSCENE_MOUNTAIN, mountain_sprite_rectangle);

                MATH::FloatRectangle ark_sprite_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 100, 50);
                FloodCutscene.Ark = GRAPHICS::Sprite(RESOURCES::AssetId::FLOOD_CUTSCENE_ARK, ark_sprite_rectangle);

                // The flood sprite is slightly larger than the screen height to account for the "waves" at the top and allowing
                // the sprite to rise up while still having the bottom of the screen appear blue.
                MATH::FloatRectangle flood_water_sprite_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 512, 416);
                FloodCutscene.FloodWaters = GRAPHICS::Sprite(RESOURCES::AssetId::FLOOD_CUTSCENE_WATERS, flood_water_sprite_rectangle);
                break;
            }
            case GameState::GAMEPLAY:
            {
                // LOAD THE GAME'S SAVE FILE.
                // The empty check is for handling the case where no saved games exist yet, primarily when
                // switching between states in debug mode.
                std::shared_ptr<SavedGameData> saved_game_data;
                if (!GameSelectionScreen.SavedGames.empty())
                {
                    saved_game_data = GameSelectionScreen.SavedGames.at(GameSelectionScreen.SelectedGameIndex);
                }
                bool saved_game_data_loaded = (nullptr != saved_game_data);
                if (!saved_game_data_loaded)
                {
                    // USE THE DEFAULT SAVED GAME DATA FOR A NEW GAME.
                    saved_game_data = std::make_shared<SavedGameData>(SavedGameData::DefaultSavedGameData());
                }

                // INITIALIZE THE GAMEPLAY STATE.
                bool gameplay_state_initialized = GameplayState.Initialize(
                    renderer.Screen->WidthInPixels<unsigned int>(),
                    saved_game_data,
                    world,
                    renderer);
                if (!gameplay_state_initialized)
                {
                    /// @todo   Error-handling!
                    return;
                }

                // FOCUS THE CAMERA ON THE PLAYER.
                MATH::Vector2f player_start_world_position = GameplayState.World->NoahPlayer->GetWorldPosition();
                renderer.Camera.SetCenter(player_start_world_position);
                break;
            }
        }
    }
}
