// BIG TODO LIST:
// - Move animals more properly in ark
// - Add underground caves
// - Add special stuff for specific Bible verses
// - Allow planting of trees
// - Raft creation for crossing waterways
// - People...specifically Noah's family
// - Add gathering of family
// - Ark gameplay
// - Post-ark gameplay/cutscenes
// - Completed game credits
// - Cutscene/text boxes for transition into ark
// - Polish (custom fancy graphics) for inventory GUI
// - Sword guarding garden of Eden
//
// Areas for code cleanup:
// - Graphics
// - Inventory
// - States

#include <cassert>
#include <chrono>
#include <exception>
#include <memory>
#include <string>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Hardware/GamingHardware.h"
#include "Input/InputController.h"
#include "Resources/AnimalSounds.h"
#include "Resources/AssetPackage.h"
#include "Resources/Assets.h"
#include "Resources/FoodGraphics.h"
#include "Resources/PredefinedAssetPackages.h"
#include "States/GameStates.h"
#include "States/GameState.h"

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

    auto overworld_music = assets.GetMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
    if (overworld_music)
    {
        // The pitch is lowered to give the background music a more somber, ambient tone.
        // It might be possible to make it go lower to achieve more of an ambient effect,
        // at the expense of sounding more "scary".
        overworld_music->Sfml.setPitch(0.5f);
        overworld_music->Sfml.setLoop(true);
        speakers.AddMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC, overworld_music);
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Sound load time: ", load_time_diff.count());
}

std::shared_ptr<RESOURCES::Assets> LoadRemainingAssets(const std::shared_ptr<RESOURCES::Assets>& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();
    
    std::unordered_map<RESOURCES::AssetId, RESOURCES::Asset> remaining_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::MAIN_ASSET_PACKAGE_FILENAME);
    std::vector<RESOURCES::Asset> temporary_assets;
    for (const auto& [asset_id, asset] : remaining_assets)
    {
        if (asset.Type == RESOURCES::AssetType::MUSIC || asset.Type == RESOURCES::AssetType::SOUND_EFFECT)
        {
            if (speakers.Enabled)
            {
                temporary_assets.push_back(asset);
            }
        }
        else
        {
            temporary_assets.push_back(asset);
        }
    }
    bool assets_loaded = !remaining_assets.empty() && assets->Populate(temporary_assets);
    if (!assets_loaded)
    {
        /// @todo   Allow game to continue even if not all assets loaded?  return nullptr;
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Remaining asset raw load time: ", load_time_diff.count());

    if (speakers.Enabled)
    {
        LoadSoundsAfterIntroAssets(*assets, speakers);
    }

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
/// Runs the Noah's Ark game until the user chooses to exit or an error occurs.
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

        // LOAD THE INITIAL ASSETS.
        std::unordered_map<RESOURCES::AssetId, RESOURCES::Asset> intro_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);

        // INITIALIZE THE GAMING HARDWARE.
        HARDWARE::GamingHardware gaming_hardware = HARDWARE::GamingHardware::Initialize();

        // INITIALIZE THE RENDERER.
        GRAPHICS::Renderer renderer(gaming_hardware.Screen);

        const auto& colored_texture_shader = intro_assets[RESOURCES::AssetId::COLORED_TEXTURE_SHADER];
        bool colored_texture_shader_loaded = renderer.ColoredTextShader.loadFromMemory(colored_texture_shader.BinaryData, sf::Shader::Fragment);
        assert(colored_texture_shader_loaded);

        std::shared_ptr<GRAPHICS::GUI::Font> default_sans_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        assert(default_sans_serif_font);
        renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE] = default_sans_serif_font;

        std::shared_ptr<GRAPHICS::GUI::Font> default_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(ANSI_FIXED_FONT);
        assert(default_serif_font);
        renderer.Fonts[RESOURCES::AssetId::SERIF_FONT_TEXTURE] = default_serif_font;

        // INITIALIZE THE SPEAKERS.
        const auto& intro_music_asset = intro_assets[RESOURCES::AssetId::INTRO_MUSIC];
        gaming_hardware.Speakers->LoadMusic(intro_music_asset.Id, intro_music_asset.BinaryData);

        // INITIALIZE THE INTRO SEQUENCE.
        STATES::GameStates game_states;
        game_states.IntroSequence.Initialize(*gaming_hardware.Speakers);

        // LOAD REMAINING ASSETS.
        std::shared_ptr<RESOURCES::Assets> assets = std::make_shared<RESOURCES::Assets>();
        std::shared_future< std::shared_ptr<RESOURCES::Assets> > assets_being_loaded = std::async(LoadRemainingAssets, assets, std::ref(*gaming_hardware.Speakers));
        std::future< std::shared_ptr<MAPS::World> > world_being_loaded = std::async(LoadWorldAfterAssetsFinishLoading, assets_being_loaded);

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
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
                        gaming_hardware.InputController.DisableInput();
                        break;
                    case sf::Event::GainedFocus:
                        // ENABLE INPUT.
                        // This will allow the user to resume moving the player
                        // now that the window has focus.
                        DEBUGGING::DebugConsole::WriteLine("Enabling input.");
                        gaming_hardware.InputController.EnableInput();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window->isOpen())
            {
                // READ USER INPUT.
                sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(*window);
                gaming_hardware.InputController.Mouse.ScreenPosition = MATH::Vector2f(
                    static_cast<float>(mouse_screen_position.x),
                    static_cast<float>(mouse_screen_position.y));
                gaming_hardware.InputController.ReadInput();

                // GET THE ELAPSED TIME FOR THE NEW FRAME.
                gaming_hardware.TickClockForFrame();

                // UPDATE THE GAME'S CURRENT STATE.
                STATES::GameState next_game_state = game_states.Update(gaming_hardware, renderer.Camera, *assets);

                // RENDER THE CURRENT STATE OF THE GAME TO THE WINDOW.
                sf::Sprite screen_sprite = game_states.Render(gaming_hardware, renderer);
                window->draw(screen_sprite);
                window->display();

                // OVERRIDE GAME STATE SWITCHES WITH DEBUG KEY PRESSES.
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num1))
                {
                    next_game_state = STATES::GameState::INTRO_SEQUENCE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num2))
                {
                    next_game_state = STATES::GameState::TITLE_SCREEN;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num3))
                {
                    next_game_state = STATES::GameState::CREDITS_SCREEN;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num4))
                {
                    next_game_state = STATES::GameState::GAME_SELECTION_SCREEN;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num5))
                {
                    next_game_state = STATES::GameState::NEW_GAME_INTRO_SEQUENCE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num6))
                {
                    next_game_state = STATES::GameState::FLOOD_CUTSCENE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num7))
                {
                    next_game_state = STATES::GameState::GAMEPLAY;
                    gaming_hardware.Speakers->StopAllAudio();
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
                game_states.SwitchStatesIfChanged(next_game_state, world, assets, renderer);
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
