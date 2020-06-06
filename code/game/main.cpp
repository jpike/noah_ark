// BIG TODO LIST:
// - Create new background music with better timing:
//      - For intro
//      - For new game intro
//      - For gameplay
// - Move animals more properly in ark
// - Add underground caves
// - Add special stuff for specific Bible verses
// - Allow planting of trees
// - Raft creation for crossing waterways
// - People...specifically Noah's family
// - Add gathering of family
// - Have animals cry if hit with axe
// - Flood cutscene graphics
// - Ark gameplay
// - Post-ark gameplay/cutscenes
// - Completed game credits
// - Cutscene/text boxes for transition into ark
// - Polish (custom fancy graphics) for inventory GUI
// - Better word wrapping with verses
// - Handle stopping music when quickly switching between states?
// - Sword guarding garden of Eden

#include <chrono>
#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Input/InputController.h"
#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Math/Number.h"
#include "Resources/AnimalSounds.h"
#include "Resources/AssetPackage.h"
#include "Resources/Assets.h"
#include "Resources/FoodGraphics.h"
#include "Resources/PredefinedAssetPackages.h"
#include "States/GameStates.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

bool LoadIntroSequenceAssets(RESOURCES::Assets& asset_collection)
{
    auto load_start_time = std::chrono::system_clock::now();

    std::vector<RESOURCES::Asset> intro_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);
    bool assets_loaded = !intro_assets.empty() && asset_collection.Populate(intro_assets);

    std::shared_ptr<GRAPHICS::GUI::Font> default_sans_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
    if (!default_sans_serif_font)
    {
        DEBUGGING::DebugConsole::WriteErrorLine("Failed to load default sans serif font.");
        return false;
    }

    std::shared_ptr<GRAPHICS::GUI::Font> default_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(ANSI_FIXED_FONT);
    if (!default_serif_font)
    {
        DEBUGGING::DebugConsole::WriteErrorLine("Failed to load default serif font.");
        return false;
    }

    asset_collection.Fonts[RESOURCES::AssetId::FONT_TEXTURE] = default_sans_serif_font;
    asset_collection.Fonts[RESOURCES::AssetId::SERIF_FONT_TEXTURE] = default_serif_font;

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Intro sequence load time: ", load_time_diff.count());

    return assets_loaded;
}

/// Loads the world.
/// @param[in,out]  assets - The assets to use for the world.
/// @return The world, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::World> LoadWorld(RESOURCES::Assets& assets)
{
    // CREATE THE EMPTY WORLD.
    auto load_start_time = std::chrono::system_clock::now();
    std::shared_ptr<MAPS::World> world = MAPS::World::Populate(assets);
    auto load_end_time = std::chrono::system_clock::now();

    // Overworld load time: 82,263,372 (file-based)
    // Overworld load time: 17,972,479 (array-based)
    // World load time (with ark interior): 31,132,753
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("World load time: ", load_time_diff.count());

    return world;
}

/// Loads the sounds/music for the game, adding them to the speakers.
/// @param[in,out]  assets - The assets from which to load the sounds/music.
/// @param[in,out]  speakers - The speakers to add sounds to.
void LoadSoundsAfterIntroAssets(RESOURCES::Assets& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();

    // ADD EACH GENERIC SOUND EFFECT TO THE SPEAKERS.
    const std::array<RESOURCES::AssetId, 5> GENERIC_SOUND_EFFECT_IDS =
    {
        RESOURCES::AssetId::ARK_BUILDING_SOUND,
        RESOURCES::AssetId::AXE_HIT_SOUND,
        RESOURCES::AssetId::COLLECT_BIBLE_VERSE_SOUND,
        RESOURCES::AssetId::FOOD_PICKUP_SOUND,
        RESOURCES::AssetId::TREE_SHAKE_SOUND

    };
    for (const RESOURCES::AssetId sound_id : GENERIC_SOUND_EFFECT_IDS)
    {
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(sound_id);
        if (audio_samples)
        {
            speakers.AddSound(sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE ANIMAL SOUND EFFECTS TO THE SPEAKERS.
    for (int species_id = 0; species_id < static_cast<int>(OBJECTS::AnimalSpecies::COUNT); ++species_id)
    {
        OBJECTS::AnimalSpecies species = static_cast<OBJECTS::AnimalSpecies>(species_id);
        RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(species);
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(animal_sound_id);
        if (audio_samples)
        {
            speakers.AddSound(animal_sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE MUSIC TO THE SPEAKERS.
    auto new_game_intro_music = assets.GetMusic(RESOURCES::AssetId::NEW_GAME_INTRO_MUSIC);
    if (new_game_intro_music)
    {
        speakers.AddMusic(RESOURCES::AssetId::NEW_GAME_INTRO_MUSIC, new_game_intro_music);
    }

    std::shared_ptr<sf::Music> overworld_music = assets.GetMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
    if (overworld_music)
    {
        // The pitch is lowered to give the background music a more somber, ambient tone.
        // It might be possible to make it go lower to achieve more of an ambient effect,
        // at the expense of sounding more "scary".
        overworld_music->setPitch(0.5f);
        overworld_music->setLoop(true);
        speakers.AddMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC, overworld_music);
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Sound load time: ", load_time_diff.count());
}

std::shared_ptr<RESOURCES::Assets> LoadRemainingAssets(const std::shared_ptr<RESOURCES::Assets>& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();
    
    std::vector<RESOURCES::Asset> remaining_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::MAIN_ASSET_PACKAGE_FILENAME);
    bool assets_loaded = !remaining_assets.empty() && assets->Populate(remaining_assets);
    if (!assets_loaded)
    {
        return nullptr;
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Remaining asset raw load time: ", load_time_diff.count());

    LoadSoundsAfterIntroAssets(*assets, speakers);

    return assets;
}

std::shared_ptr<MAPS::World> LoadWorldAfterAssetsFinishLoading(std::shared_future< std::shared_ptr<RESOURCES::Assets> > assets_being_loaded)
{
    // GET THE ASSETS ONCE THEY'VE FINISHED BEING LOADED.
    std::shared_ptr<RESOURCES::Assets> assets = assets_being_loaded.get();
    if (!assets)
    {
        return nullptr;
    }

    // LOAD THE WORLD.   
    std::shared_ptr<MAPS::World> world = LoadWorld(*assets);
    return world;
}

/// The main entry point for the game.
/// Runs the Noah's Ark game until the user
/// chooses to exit or an error occurs.
/// @return 0 for success; 1 upon failure.
int main()
{
    try
    {
        // CREATE THE WINDOW.
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(GRAPHICS::Screen::DEFAULT_WIDTH_IN_PIXELS, GRAPHICS::Screen::DEFAULT_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // Ensure that only one key event is generated for each key press.
        window->setKeyRepeatEnabled(false);

        // LOAD ASSETS NEEDED FOR THE INTRO SEQUENCE.
        // Some assets are explicitly loaded first as they are needed for initial
        // rendering of the intro sequence and title screen.  By loading them first,
        // quick startup time for the game can be maintained, showing the intro
        // sequence while other assets are being loaded.
        std::shared_ptr<RESOURCES::Assets> assets = std::make_shared<RESOURCES::Assets>();
        bool intro_sequence_assets_loaded = LoadIntroSequenceAssets(*assets);
        if (!intro_sequence_assets_loaded)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Failed to load intro sequence assets.");
            return EXIT_FAILURE;
        }

        std::shared_ptr<AUDIO::Speakers> speakers = std::make_shared<AUDIO::Speakers>();
        auto intro_music = assets->GetMusic(RESOURCES::AssetId::INTRO_MUSIC);
        if (!intro_music)
        {
            return EXIT_FAILURE;
        }
        speakers->AddMusic(RESOURCES::AssetId::INTRO_MUSIC, intro_music);
        speakers->PlayMusic(RESOURCES::AssetId::INTRO_MUSIC);

        // LOAD REMAINING ASSETS.
        std::shared_future< std::shared_ptr<RESOURCES::Assets> > assets_being_loaded = std::async(LoadRemainingAssets, assets, std::ref(*speakers));
        std::future< std::shared_ptr<MAPS::World> > world_being_loaded = std::async(LoadWorldAfterAssetsFinishLoading, assets_being_loaded);

        // CREATE THE SCREEN.
        std::unique_ptr<GRAPHICS::Screen> screen = GRAPHICS::Screen::Create();
        if (!screen)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Failed to create game screen.");
            return EXIT_FAILURE;
        }

        // INITIALIZE THE RENDERER.
        GRAPHICS::Renderer renderer(std::move(screen));

        auto colored_texture_shader = assets->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
        auto serif_font = assets->GetFont(RESOURCES::AssetId::SERIF_FONT_TEXTURE);
        auto default_font = assets->GetFont(RESOURCES::AssetId::FONT_TEXTURE);
        bool renderer_assets_loaded = (colored_texture_shader && serif_font && default_font);
        
        if (!renderer_assets_loaded)
        {
            return EXIT_FAILURE;
        }

        renderer.ColoredTextShader = colored_texture_shader;
        renderer.Fonts[RESOURCES::AssetId::SERIF_FONT_TEXTURE] = serif_font;
        renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE] = default_font;

        // INITIALIZE REMAINING SUBSYSTEMS.
        STATES::GameStates game_states(speakers, assets);

        INPUT_CONTROL::InputController input_controller;

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        sf::Clock game_loop_clock;
        sf::Time total_elapsed_time;
        while (window->isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window->pollEvent(event))
            {
                // HANDLE THE CURRENT EVENT BASED ON ITS TYPE.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        // CLOSE THE WINDOW.
                        // The game will end later.
                        window->close();
                        break;
                    case sf::Event::LostFocus:
                        // DISABLE INPUT.
                        // This will prevent the user from accidentally moving
                        // the player until the window regains focus.
                        DEBUGGING::DebugConsole::WriteLine("Disabling input.");
                        input_controller.DisableInput();
                        break;
                    case sf::Event::GainedFocus:
                        // ENABLE INPUT.
                        // This will allow the user to resume moving the player
                        // now that the window has focus.
                        DEBUGGING::DebugConsole::WriteLine("Enabling input.");
                        input_controller.EnableInput();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window->isOpen())
            {
                // READ USER INPUT.
                sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(*window);
                input_controller.Mouse.ScreenPosition = MATH::Vector2f(
                    static_cast<float>(mouse_screen_position.x),
                    static_cast<float>(mouse_screen_position.y));
                input_controller.ReadInput();

                // GET THE ELAPSED TIME FOR THE NEW FRAME.
                sf::Time elapsed_time = game_loop_clock.restart();
                total_elapsed_time += elapsed_time;

                // UPDATE THE GAME'S CURRENT STATE.
                STATES::GameState next_game_state = game_states.Update(elapsed_time, input_controller, renderer.Camera, *speakers);

                // RENDER THE CURRENT STATE OF THE GAME TO THE WINDOW.
                sf::Sprite screen_sprite = game_states.Render(total_elapsed_time, renderer);
                window->draw(screen_sprite);
                window->display();

                // OVERRIDE GAME STATE SWITCHES WITH DEBUG KEY PRESSES.
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_INTRO_SEQUENCE_KEY))
                {
                    next_game_state = STATES::GameState::INTRO_SEQUENCE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_TITLE_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::TITLE_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_CREDITS_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::CREDITS_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_GAME_SELECTION_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::GAME_SELECTION_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_NEW_GAME_INTRO_SEQUENCE_KEY))
                {
                    next_game_state = STATES::GameState::NEW_GAME_INTRO_SEQUENCE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_FLOOD_CUTSCENE_KEY))
                {
                    next_game_state = STATES::GameState::FLOOD_CUTSCENE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_GAMEPLAY_KEY))
                {
                    next_game_state = STATES::GameState::GAMEPLAY;
                }

                // PERFORM ADDITIONAL STEPS NEEDED TO TRANSITION TO CERTAIN NEW GAME STATES.
                bool remaining_assets_needed = (STATES::GameState::INTRO_SEQUENCE != next_game_state);
                if (remaining_assets_needed)
                {
                    assets_being_loaded.wait();
                }
                
                std::shared_ptr<MAPS::World> world;
                bool world_needed = (STATES::GameState::GAMEPLAY == next_game_state);
                if (world_needed)
                {
                    // This protection against the world being invalid is primarily to handle
                    // debug code for quickly switching between states.
                    bool world_loading_valid = world_being_loaded.valid();
                    if (world_loading_valid)
                    {
                        world = world_being_loaded.get();
                    }
                    else
                    {
                        world = game_states.GameplayState.World;
                    }
                    if (!world)
                    {
                        return EXIT_FAILURE;
                    }

                    // GET IMPORTANT SHADERS.
                    renderer.TimeOfDayShader = assets->GetShader(RESOURCES::AssetId::TIME_OF_DAY_SHADER);
                    if (!renderer.TimeOfDayShader)
                    {
                        return EXIT_FAILURE;
                    }
                }
                game_states.SwitchStatesIfChanged(next_game_state, world, renderer);
            }
        }

        // INDICATE THAT THE GAME EXITED SUCCESSFULLY.
        return EXIT_SUCCESS;
    }
    catch (std::exception& exception)
    {
        DEBUGGING::DebugConsole::WriteErrorLine("Standard exception in main: " + std::string(exception.what()));
        return EXIT_FAILURE;
    }
    catch (...)
    {   
        DEBUGGING::DebugConsole::WriteErrorLine("Unknown exception in main.");
        return EXIT_FAILURE;
    }
}
