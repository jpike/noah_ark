#include "Debugging/DebugConsole.h"
#include "ErrorHandling/Asserts.h"
#include "Resources/AssetId.h"
#include "States/EnteringArkCutscene.h"

namespace STATES
{
    /// Loads the cutscene into its initial state.
    /// @param[in,out]  world - The world to configure for this state.
    /// @param[in,out]  renderer - The renderer from which to load some initial data.
    /// @param[in,out]  gaming_hardware - The gaming hardware to use for loading the cutscene.
    void EnteringArkCutscene::Load(MAPS::World& world, GRAPHICS::Renderer& renderer, HARDWARE::GamingHardware& gaming_hardware)
    {
        // RESET BASIC MEMBER VARIABLES TO THE BEGINNING OF THE CUTSCENE.
        CurrentSubstate = Substate::FADING_IN;
        ElapsedTimeForCurrentSubstate = sf::Time::Zero;
        ArkEntranceTileMap = nullptr;

        // INITIALIZE THE TEXT BOX IF POSSIBLE.
        std::shared_ptr<GRAPHICS::GUI::Font> text_box_font = renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE];
        ASSERT_THEN_IF(text_box_font)
        {
            unsigned int text_box_width_in_pixels = renderer.Screen->WidthInPixels<unsigned int>();
            // The text displayed in this text box is slightly longer than normal, so 3 lines are used instead of the normal 2.
            const unsigned int LINE_COUNT = 3;
            unsigned int text_box_height_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS * LINE_COUNT;
            TextBox = GRAPHICS::GUI::TextBox(
                text_box_width_in_pixels,
                text_box_height_in_pixels,
                text_box_font);
        }

        // SET THE GAME TO FOCUS ON THE ARK ENTRANCE.
        constexpr unsigned int ARK_ENTRANCE_TILE_MAP_ROW = 0;
        constexpr unsigned int ARK_ENTRANCE_TILE_MAP_COLUMN = 2;
        ArkEntranceTileMap = world.Overworld.MapGrid.GetTileMap(ARK_ENTRANCE_TILE_MAP_ROW, ARK_ENTRANCE_TILE_MAP_COLUMN);
        ASSERT_THEN_IF(ArkEntranceTileMap)
        {
            // SET THE CAMERA TO FOCUS ON THIS TILE MAP.
            MATH::Vector2f ark_entrance_tile_map_center = ArkEntranceTileMap->GetCenterWorldPosition();
            renderer.Camera.SetCenter(ark_entrance_tile_map_center);

            // MOVE NOAH'S FAMILY TO INITIAL POSITIONS FOR THE CUTSCENE.
            // They're positioned slightly below the entrance.
            MATH::Vector2f noah_world_position = ark_entrance_tile_map_center + MATH::Vector2f(0.0f, 64.0f);
            world.NoahPlayer->SetWorldPosition(noah_world_position);

            // The family is positioned randomly a bit below Noah.
            MATH::Vector2f base_family_member_world_position = noah_world_position + MATH::Vector2f(0.0f, 32.0f);
            for (OBJECTS::FamilyMember& family_member : world.FamilyMembers)
            {
                constexpr float MAX_FAMILY_MEMBER_OFFSET = 32.0f;
                float family_member_x_offset = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan<float>(MAX_FAMILY_MEMBER_OFFSET);
                float family_member_y_offset = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan<float>(MAX_FAMILY_MEMBER_OFFSET);
                MATH::Vector2f current_family_member_position = base_family_member_world_position + MATH::Vector2f(family_member_x_offset, family_member_y_offset);
                family_member.Sprite.SetWorldPosition(current_family_member_position);
            }
        }
    }

    /// Updates the state.
    /// @param[in]  elapsed_time - The elapsed time for which to update the cutscene.
    /// @param[in,out]  world - The game world to update.
    /// @return The state the game should be in after the update.
    GameState EnteringArkCutscene::Update(const sf::Time& elapsed_time, MAPS::World& world)
    {
        // UPDATE BASED ON THE SUBSTATE.
        // The game should remain on the current state until this cutscene is completed.
        GameState next_game_state = GameState::ENTER_ARK_CUTSCENE;
        switch (CurrentSubstate)
        {
            case Substate::FADING_IN:
            {
                // FADE IN UNTIL WE'VE COMPLETELY FADED IN.
                ElapsedTimeForCurrentSubstate += elapsed_time;
                float elapsed_time_for_current_substate_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
                bool fading_in_complete = (elapsed_time_for_current_substate_in_seconds >= FADING_MAX_TIME_IN_SECONDS);
                if (fading_in_complete)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::DISPLAYING_INSTRUCTIONS;
                    // The instruction text is based on Genesis 7:1 but hardcoded here for simplicity.
                    TextBox.StartDisplayingText("GOD: Come thou and all thy house into the ark; for thee have I seen righteous before me in this generation.");
                }
                break;
            }
            case Substate::DISPLAYING_INSTRUCTIONS:
            {
                // DISPLAY INSTRUCTIONS TO ENTER THE ARK UNTIL THEY'RE COMPLETED.
                TextBox.Update(elapsed_time);
                bool current_page_of_text_finished = TextBox.CurrentPageOfTextFinishedBeingDisplayed();
                if (current_page_of_text_finished)
                {
                    TextBox.MoveToNextPage();
                }

                if (!TextBox.IsVisible)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::ENTERING_INTO_ARK;
                }
                break;
            }
            case Substate::ENTERING_INTO_ARK:
            {
                // SHOW NOAH AND FAMILY ENTERING THE ARK.
                world;

                // SWITCH TO THE NEXT SUBSTATE IF THE FAMILY HAS ENTERED THE ARK.
                /// @todo   Add real logic!
                bool family_members_entered_ark = true;
                if (family_members_entered_ark)
                {
                    // MOVE TO THE NEXT SUBSTATE.
                    ElapsedTimeForCurrentSubstate = sf::Time::Zero;
                    CurrentSubstate = Substate::FADING_OUT;
                }
                break;
            }
            case Substate::FADING_OUT:
            {
                // FADE OUT UNTIL WE'VE COMPLETELY FADED IN.
                ElapsedTimeForCurrentSubstate += elapsed_time;
                float elapsed_time_for_current_substate_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
                bool fading_out_complete = (elapsed_time_for_current_substate_in_seconds >= FADING_MAX_TIME_IN_SECONDS);
                if (fading_out_complete)
                {
                    // MOVE TO THE NEXT REAL GAME STATE.
                    next_game_state = GameState::FLOOD_CUTSCENE;
                }
                break;
            }
        }

        return next_game_state;
    }

    /// Renders the current state of the cutscene.
    /// @param[in]  world - The game world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @param[in,out]  gaming_hardware - The gaming hardware to use for rendering.
    sf::Sprite EnteringArkCutscene::Render(const MAPS::World& world, GRAPHICS::Renderer& renderer, HARDWARE::GamingHardware& gaming_hardware)
    {
        gaming_hardware;

        renderer.RenderScreenRectangle(renderer.Screen->GetBoundingRectangle<float>(), GRAPHICS::Color::WHITE);

        // RENDER THE CURRENT TILE MAP.
        ASSERT_THEN_IF(ArkEntranceTileMap)
        {
            renderer.Render(*ArkEntranceTileMap);
        }

        // RENDER THE FAMILY MEMBERS.
        for (const OBJECTS::FamilyMember& family_member : world.FamilyMembers)
        {
            renderer.Render(family_member.Sprite.CurrentFrameSprite);
        }

        // RENDER NOAH.
        renderer.Render(world.NoahPlayer->Sprite.CurrentFrameSprite);

        // RENDER THE TEXT BOX IF IT'S VISIBLE.
        if (TextBox.IsVisible)
        {
            TextBox.Render(renderer);
        }

        // COMPUTE SHADING BASED ON THE CURRENT SUBSTATE.
        float elapsed_time_for_fading_in_seconds = ElapsedTimeForCurrentSubstate.asSeconds();
        float current_ratio_through_current_fade = elapsed_time_for_fading_in_seconds / FADING_MAX_TIME_IN_SECONDS;
        GRAPHICS::Color current_tint = GRAPHICS::Color::WHITE;
        switch (CurrentSubstate)
        {
            case Substate::FADING_IN:
            {
                current_tint.ScaleRgb(current_ratio_through_current_fade);
                break;
            }
            case Substate::FADING_OUT:
            {
                constexpr float MAX_RATIO = 1.0f;
                float current_ratio_remaining_for_fade = MAX_RATIO - current_ratio_through_current_fade;
                current_tint.ScaleRgb(current_ratio_remaining_for_fade);
                break;
            }
        }

        DEBUGGING::DebugConsole::WriteLine("EnteringArkCutscene - state = ", static_cast<unsigned int>(CurrentSubstate));
        DEBUGGING::DebugConsole::WriteLine("EnteringArkCutscene - current_tint = ", current_tint);

        sf::RenderStates tinting = sf::RenderStates::Default;

        std::shared_ptr<sf::Shader> colored_texture_shader = renderer.GraphicsDevice->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
        ASSERT_THEN_IF(colored_texture_shader)
        {
            colored_texture_shader->setUniform("color", sf::Glsl::Vec4(sf::Color(current_tint.Red, current_tint.Green, current_tint.Blue, current_tint.Alpha)));
            colored_texture_shader->setUniform("texture", sf::Shader::CurrentTexture);
            tinting.shader = colored_texture_shader.get();

            DEBUGGING::DebugConsole::WriteLine("EnteringArkCutscene - Setting tinting shader");
        }

        // RENDER THE FINAL SCREEN.
        sf::Sprite screen_sprite = renderer.RenderFinalScreen(tinting);
        return screen_sprite;
    }
}
