#include "States/GameStates.h"

namespace STATES
{
    /// Updates the current state of the game.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @param[in,out]  world - The world to be updated.
    /// @param[in,out]  camera - The camera defining the viewable region of the game world.
    /// @return The next state that the game should switch to.  The current state as tracked
    ///     in this class is not automatically updated to this new state.
    GameState GameStates::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        GRAPHICS::GUI::HeadsUpDisplay& hud,
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
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                next_game_state = NewGameInstructionSequence.Update(gaming_hardware, world, CurrentSavedGame, hud);
                break;
            case GameState::FLOOD_CUTSCENE:
                next_game_state = FloodCutscene.Update(gaming_hardware);
                break;
            case GameState::GAMEPLAY:
                next_game_state = GameplayState.Update(
                    gaming_hardware,
                    world,
                    camera,
                    hud,
                    CurrentSavedGame);
                break;
        }

        // RETURN THE NEXT STATE THE GAME SHOULD BE IN.
        return next_game_state;
    }

    /// Renders the current state of the game.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output devices.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered state of the game.
    sf::Sprite GameStates::Render(
        HARDWARE::GamingHardware& gaming_hardware, 
        MAPS::World& world,
        GRAPHICS::GUI::HeadsUpDisplay& hud,
        GRAPHICS::Renderer& renderer)
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
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                screen_sprite = NewGameInstructionSequence.Render(gaming_hardware, world, CurrentSavedGame, hud, renderer);
                break;
            case GameState::FLOOD_CUTSCENE:
                screen_sprite = FloodCutscene.Render(renderer);
                break;
            case GameState::GAMEPLAY:
                screen_sprite = GameplayState.Render(world, hud, CurrentSavedGame, renderer);
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
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        GRAPHICS::GUI::HeadsUpDisplay& hud)
    {
        // CHECK IF THE GAME STATE HAS CHANGED.
        bool game_state_changed = (new_state != CurrentGameState);
        if (!game_state_changed)
        {
            // The game is already in the correct state.
            return;
        }

        /// @todo   Stop audio?

        // UPDATE THE SAVED GAME DATA BEING USED IF A SAVED GAME IS BEING LOADED.
        bool saved_game_being_loaded = (GameState::GAME_SELECTION_SCREEN == CurrentGameState);
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
                // RESET THE INTRO SEQUENCE TO THE BEGINNING.
                NewGameIntroSequence.ResetToBeginning();
                break;
            case GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                NewGameInstructionSequence.Initialize(world, hud);
                break;
            case GameState::FLOOD_CUTSCENE:
                FloodCutscene.Initialize();
                break;
            case GameState::GAMEPLAY:
            {
                // INITIALIZE THE GAMEPLAY STATE.
                bool gameplay_state_initialized = GameplayState.Initialize(
                    CurrentSavedGame,
                    world,
                    renderer);
                if (!gameplay_state_initialized)
                {
                    /// @todo   Error-handling!
                    return;
                }

                break;
            }
        }
    }
}
