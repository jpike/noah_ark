#include <cassert>
#include "Bible/BibleVerses.h"
#include "States/NewGameInstructionSequence.h"

namespace STATES
{
    /// Sets the new game instruction sequence to its initial state.
    void NewGameInstructionSequence::Initialize(MAPS::World& world, GRAPHICS::GUI::HeadsUpDisplay& hud)
    {
        // INDICATE THAT THE INSTRUCTIONS HAVEN'T BEEN COMPLETED.
        NewGameInstructionsCompleted = false;

        // INITIALIZE THE NEW GAME INSTRUCTION TEXT.
        NewGameInstructionText = "";
        constexpr unsigned int MIN_GENESIS_6_VERSE_NUMBER = 13;
        constexpr unsigned int MAX_GENESIS_6_VERSE_NUMBER = 21;
        for (unsigned int genesis_6_verse_number = MIN_GENESIS_6_VERSE_NUMBER; genesis_6_verse_number <= MAX_GENESIS_6_VERSE_NUMBER; ++genesis_6_verse_number)
        {
            // GET THE CURRENT VERSE.
            const BIBLE::BibleVerse* current_verse = BIBLE::FindBibleVerse(BIBLE::BibleBook::GENESIS, 6, genesis_6_verse_number);
            if (!current_verse)
            {
                // This is just a small precaution but shouldn't actually occur.
                assert(current_verse);
                continue;
            }

            // ADD THE VERSE TEXT TO THE NEW GAME INSTRUCTION TEXT.
            // The text for the first verse (Genesis 6:13) is hardcoded in a tweaked form to make it more naturally sound like God is talking to the player (Noah).
            // This is done in the loop (rather than hardcoded before the loop) to ensure that this verse still gets added to the player's inventory.
            bool first_verse = (MIN_GENESIS_6_VERSE_NUMBER == genesis_6_verse_number);
            if (first_verse)
            {
                NewGameInstructionText += "GOD: The end of all flesh is come before me; for the earth is filled with violence through them; and, behold, I will destroy them with the earth.";
            }
            else
            {
                NewGameInstructionText += current_verse->Text;
            }

            // A space should be added before the next verse if there is another verse to add
            // since all of these verses end in punctuation.
            bool more_verses_to_add = (MAX_GENESIS_6_VERSE_NUMBER != genesis_6_verse_number);
            if (more_verses_to_add)
            {
                NewGameInstructionText += ' ';
            }

            // ADD THE VERSE TO THE PLAYER'S INVENTORY.
            // That way, it will already be collected once the player is done.
            world.NoahPlayer.Inventory->BibleVerses.insert(*current_verse);
        }

        // START DISPLAYING THE TEXT IN THE TEXT BOX.
        hud.MainTextBox.StartDisplayingText(NewGameInstructionText);
    }

    /// Updates the instruction sequence for a frame.
    /// @param[in]  gaming_hardware - The gaming hardware supplying input.
    /// @return The state the game should be in after the update.
    GameState NewGameInstructionSequence::Update(
        const HARDWARE::GamingHardware& gaming_hardware,
        const MAPS::World& world, 
        STATES::SavedGameData& current_game_data,
        GRAPHICS::GUI::HeadsUpDisplay& hud)
    {
        // UPDATE THE HUD.
        // As of now, only the HUD is capable of altering the gameplay state.
        GameState next_game_state = hud.Update(gaming_hardware, world, current_game_data);

        // UPDATE THE TEXT BOX IF IT IS VISIBLE.
        // If the text box is currently being displayed, then it should capture any user input.
        /// @todo   Why isn't this encapsulated in the HUD update?
        if (hud.MainTextBox.IsVisible)
        {
            // UPDATE THE TEXT IN THE TEXT BOX.
            hud.MainTextBox.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
        }

        // INDICATE THAT THE NEW GAME INSTRUCTIONS HAVE BEEN COMPLETED IF THE USER HAS GONE THROUGH ALL TEXT.
        // This must be done after updating the HUD to prevent an infinite loop from occurring regarding the
        // displaying of the new game instruction text.
        /// @todo   Maybe this would be simpler as a separate game state?
        if (!NewGameInstructionsCompleted)
        {
            NewGameInstructionsCompleted = !hud.MainTextBox.IsVisible;
        }
        if (NewGameInstructionsCompleted)
        {
            next_game_state = GameState::GAMEPLAY;
        }
        else
        {
            next_game_state = GameState::NEW_GAME_INSTRUCTION_SEQUENCE;
        }
        return next_game_state;
    }

    /// Renders a frame of the new game instruction sequence.
    /// @param[in]  gaming_hardware - The hardware supplying input and output.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    sf::Sprite NewGameInstructionSequence::Render(
        const HARDWARE::GamingHardware& gaming_hardware, 
        MAPS::World& world,
        const STATES::SavedGameData& current_game_data,
        GRAPHICS::GUI::HeadsUpDisplay& hud,
        GRAPHICS::Renderer& renderer)
    {
        // RENDER THE STARTING POINT OF THE OVERWORLD.
        renderer.Render(world.Overworld);

        // RENDER NOAH.
        renderer.Render(world.NoahPlayer.Sprite.Sprite);

        // RENDER THE HUD.
        hud.Render(current_game_data, *world.NoahPlayer.Inventory, renderer);

        // If the player is beginning a new game with God speaking to Noah, then the pulsing light
        // shader should be used to help communicate that God is speaking to the player.
        /// @todo   Might be better to have a fancier "spinning light" style-effect.
        sf::RenderStates lighting = sf::RenderStates::Default;
        bool pulse_light_for_new_game_text = !NewGameInstructionsCompleted;
        if (pulse_light_for_new_game_text)
        {
            // COMPUTE THE TINT TO APPLY TO THE SCREEN.
            // It should pulse based on elapsed time.
            float elapsed_time_in_seconds = gaming_hardware.Clock.TotalElapsedTime.asSeconds();
            // The range of sin() is [-1, 1].  Since we want to compute an additional lighting factor
            // to add to the base lighting amount, without making it too dark/too bright, the scale
            // of this additional lighting is adjusted to be [-0.4, 0.6].
            // The initial multiplication brings the range to [-0.5, 0.5].
            constexpr float ADDITIONAL_LIGHTING_FACTOR_RANGE = 0.5f;
            float additional_lighting_factor = ADDITIONAL_LIGHTING_FACTOR_RANGE * std::sinf(elapsed_time_in_seconds);
            // An addition shifts it into the appropriate range.
            constexpr float ADDITIONAL_LIGHTING_FACTOR_SHIFT_AMOUNT = 0.1f;
            additional_lighting_factor += ADDITIONAL_LIGHTING_FACTOR_SHIFT_AMOUNT;

            // To ensure that the lighting stays bright enough even as the pulsing occurs and also
            // tends toward brighter (more indicative of God) than darker, the additional factor
            // above is added to a base lighting amount.
            // is added in.
            constexpr float BASE_LIGHTING_AMOUNT = 1.0f;
            float lighting_scale_factor = BASE_LIGHTING_AMOUNT + additional_lighting_factor;

            // RENDER THE SCREEN WITH THE CURRENT LIGHTING.
            constexpr float ALPHA_FOR_FULLY_OPAQUE = 1.0f;
            std::shared_ptr<sf::Shader> colored_text_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
            if (colored_text_shader)
            {
                colored_text_shader->setUniform("color", sf::Glsl::Vec4(lighting_scale_factor, lighting_scale_factor, lighting_scale_factor, ALPHA_FOR_FULLY_OPAQUE));
                colored_text_shader->setUniform("texture", sf::Shader::CurrentTexture);
                lighting.shader = colored_text_shader.get();
            }
        }

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen(lighting);
        return screen;
    }
}
