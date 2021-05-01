// TODO LIST BEFORE BETA 1:
// - Add in ark raven/dove actions
// - Add scene after flood
// - Add in any missing music
// - Have some Bible verses only obtainable during certain sections of game
// - Take a pass at TODOs

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

                auto final_credits_music = gaming_hardware.Speakers->GetMusic(RESOURCES::AssetId::FINAL_CREDITS_MUSIC);
                if (final_credits_music)
                {
                    final_credits_music->Sfml.setLoop(true);
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
        DEBUGGING::DebugConsole::WriteLine("About to load intro assets...");
        std::unordered_map<RESOURCES::AssetId, RESOURCES::Asset> intro_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);

        // INITIALIZE THE GAMING HARDWARE.
        HARDWARE::GamingHardware gaming_hardware;

        const auto& intro_music_asset = intro_assets[RESOURCES::AssetId::INTRO_MUSIC];
        gaming_hardware.Speakers->LoadMusic(intro_music_asset.Id, intro_music_asset.BinaryData);

        const auto& colored_texture_shader = intro_assets[RESOURCES::AssetId::COLORED_TEXTURE_SHADER];
        gaming_hardware.GraphicsDevice->LoadShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER, sf::Shader::Fragment, colored_texture_shader.BinaryData);

        DEBUGGING::DebugConsole::WriteLine("About to load remaining assets...");
        std::shared_future<void> assets_being_loaded = std::async(LoadRemainingAssets, std::ref(gaming_hardware));
        DEBUGGING::DebugConsole::WriteLine("After kicking off loading remaining assets...");

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
        game_states.IntroSequence.Start(*gaming_hardware.Speakers);

        // CREATE THE WORLD.
        MAPS::World world;
        game_states.CurrentSavedGame.Player = world.NoahPlayer;

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        DEBUGGING::DebugConsole::WriteLine("About to start main game loop...");
        while (window.isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window.pollEvent(event))
            {
                // HANDLE THE CURRENT EVENT BASED ON ITS TYPE.
                // Note - "pausing" of the window during dragging, holding title bar, etc.
                // probably can't be resolved (see https://en.sfml-dev.org/forums/index.php?topic=19388.0).
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
                    default:
                        // PROVIDE DEBUG VISIBILITY INTO THE EVENT.
                        DEBUGGING::DebugConsole::WriteLine("Window event: " + std::to_string(event.type));
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
                STATES::GameState next_game_state = game_states.Update(world, renderer.Camera, gaming_hardware);

                // RENDER THE CURRENT STATE OF THE GAME TO THE WINDOW.
                sf::Sprite screen_sprite = game_states.Render(world, renderer, gaming_hardware);
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
                    next_game_state = STATES::GameState::NEW_GAME_INSTRUCTION_SEQUENCE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num7))
                {
                    next_game_state = STATES::GameState::PRE_FLOOD_GAMEPLAY;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num8))
                {
                    next_game_state = STATES::GameState::ENTER_ARK_CUTSCENE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num9))
                {
                    next_game_state = STATES::GameState::FLOOD_CUTSCENE;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Num0))
                {
                    next_game_state = STATES::GameState::DURING_FLOOD_GAMEPLAY;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Dash))
                {
                    next_game_state = STATES::GameState::POST_FLOOD_GAMEPLAY;
                    gaming_hardware.Speakers->StopAllAudio();
                }
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Equal))
                {
                    next_game_state = STATES::GameState::ENDING_CREDITS_SCREEN;
                    gaming_hardware.Speakers->StopAllAudio();
                }

                // ALLOW OTHER DEBUG FEATURES.
                if (gaming_hardware.InputController.ButtonDown(sf::Keyboard::W))
                {
                    game_states.CurrentSavedGame.Player->Inventory.WoodCount += 1;
                }

                // SWITCH THE GAME STATE IF NEEDED.
                game_states.SwitchStatesIfChanged(next_game_state, world, renderer, gaming_hardware);
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
