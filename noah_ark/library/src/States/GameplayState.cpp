#include <algorithm>
#include "Core/NullChecking.h"
#include "States/GameplayState.h"

namespace STATES
{
    /// Constructor.
    /// @param[in]  assets - The assets to be used during gameplay.
    /// @throws std::exception - Thrown if the assets are null.
    GameplayState::GameplayState(const std::shared_ptr<RESOURCES::Assets>& assets) :
        Overworld(),
        RandomNumberGenerator(),
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
        std::unique_ptr<MAPS::Overworld>&& overworld)
    {
        // MAKE SURE AN OVERWORLD WAS PROVIDED.
        bool overworld_exists = (nullptr != overworld);
        if (!overworld_exists)
        {
            // The gameplay state requires an overworld.
            return false;
        }

        // SET THE OVERWORLD.
        Overworld = std::move(overworld);

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
        Hud = InitializeHud(screen_width_in_pixels, *Overworld);
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

        // CHECK IF THE INVENTORY GUI IS DISPLAYED.
        // If the inventory GUI is displayed, then the regular controls for the player
        // in the world shouldn't work.
        if (!Hud->InventoryOpened)
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
                Overworld->Update(
                    elapsed_time,
                    RandomNumberGenerator,
                    input_controller,
                    BibleVersesLeftToFind,
                    *Assets,
                    camera,
                    message_for_text_box);

                // START DISPLAYING A NEW MESSAGE IN THE MAIN TEXT BOX IF ONE EXISTS.
                bool text_box_message_exists = !message_for_text_box.empty();
                if (text_box_message_exists)
                {
                    Hud->MainTextBox.StartDisplayingText(message_for_text_box);
                }
            }
        }
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  elapsed_time - The elapsed time since the previous frame.
    /// @param[in]  renderer - The renderer to use for rendering.
    void GameplayState::Render(const sf::Time& elapsed_time, GRAPHICS::Renderer& renderer)
    {
        renderer.Render(elapsed_time, *Overworld);
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
        const MAPS::Overworld& overworld)
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
            overworld.NoahPlayer->Inventory,
            text_box_width_in_pixels,
            text_box_height_in_pixels,
            axe_texture,
            wood_log_texture);
        return hud;
    }
}
