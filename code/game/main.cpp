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

// To avoid annoyances with Windows min/max #defines.
#define NOMINMAX

#include <cassert>
#include <chrono>
#include <exception>
#include <future>
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
#include "Resources/FoodGraphics.h"
#include "Resources/PredefinedAssetPackages.h"
#include "States/GameStates.h"
#include "States/GameState.h"

/// Loads the world.
/// @return The world, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::World> LoadWorld()
{
    // CREATE THE EMPTY WORLD.
    auto load_start_time = std::chrono::system_clock::now();
    std::shared_ptr<MAPS::World> world = MAPS::World::Populate();
    auto load_end_time = std::chrono::system_clock::now();

    // Overworld load time: 82,263,372 (file-based)
    // Overworld load time: 17,972,479 (array-based)
    // World load time (with ark interior): 31,132,753
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("World load time: ", load_time_diff.count());
    DEBUGGING::DebugConsole::WriteLine("World load time (ms): ", std::chrono::duration_cast<std::chrono::milliseconds>(load_time_diff).count());
    DEBUGGING::DebugConsole::WriteLine("World load time (s): ", std::chrono::duration_cast<std::chrono::seconds>(load_time_diff).count());

    return world;
}

void LoadRemainingAssets(HARDWARE::GamingHardware& gaming_hardware)
{
    auto load_start_time = std::chrono::system_clock::now();
    
    std::unordered_map<RESOURCES::AssetId, RESOURCES::Asset> remaining_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::MAIN_ASSET_PACKAGE_FILENAME);
    for (const auto& [asset_id, asset] : remaining_assets)
    {
        switch (asset.Type)
        {
            case RESOURCES::AssetType::TEXTURE:
                gaming_hardware.GraphicsDevice->LoadTexture(asset.Id, asset.BinaryData);
                break;
            case RESOURCES::AssetType::FONT:
                assert(false && "Fonts not supported for loading this way!");
                break;
            case RESOURCES::AssetType::SOUND_EFFECT:
                gaming_hardware.Speakers->LoadSound(asset.Id, asset.BinaryData);
                break;
            case RESOURCES::AssetType::MUSIC:
            {
                gaming_hardware.Speakers->LoadMusic(asset.Id, asset.BinaryData);

                /// @todo Make this particular adjustment be in the actual file?
                auto overworld_music = gaming_hardware.Speakers->GetMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
                if (overworld_music)
                {
                    // The pitch is lowered to give the background music a more somber, ambient tone.
                    // It might be possible to make it go lower to achieve more of an ambient effect,
                    // at the expense of sounding more "scary".
                    overworld_music->Sfml.setPitch(0.5f);
                    overworld_music->Sfml.setLoop(true);
                }

                break;
            }
            case RESOURCES::AssetType::SHADER:
                // All shaders are currently fragment shaders.
                gaming_hardware.GraphicsDevice->LoadShader(asset.Id, sf::Shader::Fragment, asset.BinaryData);
                break;
            case RESOURCES::AssetType::INVALID:
                [[fallthrough]];
            default:
                assert(false && "Invalid asset type found!");
                break;
        }
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Remaining asset raw load time: ", load_time_diff.count());
    DEBUGGING::DebugConsole::WriteLine("Remaining asset load time (ms): ", std::chrono::duration_cast<std::chrono::milliseconds>(load_time_diff).count());
    DEBUGGING::DebugConsole::WriteLine("Remaining asset load time (s): ", std::chrono::duration_cast<std::chrono::seconds>(load_time_diff).count());
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
        sf::RenderWindow window(
            sf::VideoMode(GRAPHICS::Screen::DEFAULT_WIDTH_IN_PIXELS, GRAPHICS::Screen::DEFAULT_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // Ensure that only one key event is generated for each key press.
        window.setKeyRepeatEnabled(false);

        // LOAD THE INITIAL ASSETS.
        std::unordered_map<RESOURCES::AssetId, RESOURCES::Asset> intro_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);

        // INITIALIZE THE GAMING HARDWARE.
        HARDWARE::GamingHardware gaming_hardware;

        const auto& intro_music_asset = intro_assets[RESOURCES::AssetId::INTRO_MUSIC];
        gaming_hardware.Speakers->LoadMusic(intro_music_asset.Id, intro_music_asset.BinaryData);

        const auto& colored_texture_shader = intro_assets[RESOURCES::AssetId::COLORED_TEXTURE_SHADER];
        gaming_hardware.GraphicsDevice->LoadShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER, sf::Shader::Fragment, colored_texture_shader.BinaryData);

        std::shared_future<void> assets_being_loaded = std::async(LoadRemainingAssets, std::ref(gaming_hardware));

        // INITIALIZE THE RENDERER.
        GRAPHICS::Renderer renderer;
        renderer.GraphicsDevice = gaming_hardware.GraphicsDevice;
        renderer.Screen = gaming_hardware.Screen;
        renderer.Camera = GRAPHICS::Camera(MATH::FloatRectangle::FromCenterAndDimensions(
            renderer.Screen->RenderTarget.getView().getCenter().x,
            renderer.Screen->RenderTarget.getView().getCenter().y,
            renderer.Screen->RenderTarget.getView().getSize().x,
            renderer.Screen->RenderTarget.getView().getSize().y));

        std::shared_ptr<GRAPHICS::GUI::Font> default_sans_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        assert(default_sans_serif_font);
        renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE] = default_sans_serif_font;

        std::shared_ptr<GRAPHICS::GUI::Font> default_serif_font = GRAPHICS::GUI::Font::LoadSystemDefaultFont(ANSI_FIXED_FONT);
        assert(default_serif_font);
        renderer.Fonts[RESOURCES::AssetId::SERIF_FONT_TEXTURE] = default_serif_font;

        // INITIALIZE THE INTRO SEQUENCE.
        STATES::GameStates game_states;
        game_states.IntroSequence.Initialize(*gaming_hardware.Speakers);

        // LOAD THE WORLD.
        std::future<std::shared_ptr<MAPS::World>> world_being_loaded = std::async(LoadWorld);

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        while (window.isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window.pollEvent(event))
            {
                // HANDLE THE CURRENT EVENT BASED ON ITS TYPE.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        // CLOSE THE WINDOW.
                        // The game will end later.
                        window.close();
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
            if (window.isOpen())
            {
                // READ USER INPUT.
                sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(window);
                gaming_hardware.InputController.Mouse.ScreenPosition = MATH::Vector2f(
                    static_cast<float>(mouse_screen_position.x),
                    static_cast<float>(mouse_screen_position.y));
                gaming_hardware.InputController.ReadInput();

                // UPDATE THE ELAPSED TIME FOR THE NEW FRAME.
                gaming_hardware.Clock.UpdateElapsedTime();

                // UPDATE THE GAME'S CURRENT STATE.
                STATES::GameState next_game_state = game_states.Update(gaming_hardware, renderer.Camera);

                // RENDER THE CURRENT STATE OF THE GAME TO THE WINDOW.
                sf::Sprite screen_sprite = game_states.Render(gaming_hardware, renderer);
                window.draw(screen_sprite);
                window.display();

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
                        DEBUGGING::DebugConsole::WriteErrorLine("Failed to load world");
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
