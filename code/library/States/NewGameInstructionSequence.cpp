#include <cassert>
#include "Bible/BibleVerses.h"
#include "States/NewGameInstructionSequence.h"

namespace STATES
{
    /// Sets the new game instruction sequence to its initial state.
    void NewGameInstructionSequence::Initialize(MAPS::World& world)
    {
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
            world.NoahPlayer->Inventory.BibleVerses.insert(*current_verse);
        }

        // START DISPLAYING THE TEXT IN THE TEXT BOX.
        InstructionTextBox.StartDisplayingText(NewGameInstructionText);

        // RESET THE PLAYER'S POSITION.
        world.NoahPlayer->SetWorldPosition(OBJECTS::Noah::DEFAULT_START_WORLD_POSITION);
    }

    /// Updates the instruction sequence for a frame.
    /// @param[in]  gaming_hardware - The gaming hardware supplying input.
    /// @return The state the game should be in after the update.
    GameState NewGameInstructionSequence::Update(const HARDWARE::GamingHardware& gaming_hardware)
    {
        // UPDATE THE INSTRUCTION TEXT BOX.
        if (InstructionTextBox.IsVisible)
        {
            // HAVE THE MAIN TEXT BOX RESPOND TO USER INPUT.
            if (gaming_hardware.InputController.ButtonDown(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
            {
                // CHECK IF THE TEXT BOX IS FINISHED DISPLAYING ITS CURRENT PAGE OF TEXT.
                // If the current page of text has not yet all been displayed, the next
                // page of text should not be moved to so that the user can finish
                // seeing the complete message.
                bool current_text_finished_being_displayed = InstructionTextBox.CurrentPageOfTextFinishedBeingDisplayed();
                if (current_text_finished_being_displayed)
                {
                    // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
                    InstructionTextBox.MoveToNextPage();
                }
            }

            // UPDATE THE TEXT BOX IF IT IS VISIBLE.
            // If the text box is currently being displayed, then it should capture any user input.
            InstructionTextBox.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
        }

        // DETERMINE THE NEXT GAME STATE.
        // This must be done after updating the text box above to prevent an infinite loop from occurring
        // regarding the displaying of the new game instruction text.
        // The game should remain on the current state unless the instructions have been completed.
        GameState next_game_state = GameState::NEW_GAME_INSTRUCTION_SEQUENCE;
        bool new_game_instructions_completed = !InstructionTextBox.IsVisible;
        if (new_game_instructions_completed)
        {
            next_game_state = GameState::PRE_FLOOD_GAMEPLAY;
        }
        
        return next_game_state;
    }

    /// Renders a frame of the new game instruction sequence.
    /// @param[in,out]  world - The world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @param[in,out]  gaming_hardware - The hardware supplying input and output.
    sf::Sprite NewGameInstructionSequence::Render(
        MAPS::World& world,
        GRAPHICS::Renderer& renderer,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // RENDER THE STARTING POINT OF THE OVERWORLD.
        renderer.Render(world.Overworld.MapGrid);

        // RENDER NOAH.
        renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);

        // RENDER THE TEXT BOX IF IT IS VISIBLE.      
        if (InstructionTextBox.IsVisible)
        {
            InstructionTextBox.Render(renderer);
        }

        // RENDER A PULSING LIGHT EFFECT.
        // If the player is beginning a new game with God speaking to Noah, then the pulsing light
        // shader should be used to help communicate that God is speaking to the player.
        /// @todo   Might be better to have a fancier "spinning light" style-effect.
        sf::RenderStates lighting = sf::RenderStates::Default;

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

        // Parameters need to be passed to the shader.
        constexpr float ALPHA_FOR_FULLY_OPAQUE = 1.0f;
        std::shared_ptr<sf::Shader> colored_texture_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::SHINY_BALL_SHADER);
        if (colored_texture_shader)
        {
            colored_texture_shader->setUniform("scale", lighting_scale_factor);
            colored_texture_shader->setUniform("texture", sf::Shader::CurrentTexture);
            lighting.shader = colored_texture_shader.get();
        }

        sf::Sprite screen = renderer.RenderFinalScreen(lighting);
        return screen;
    }
}
