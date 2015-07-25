#pragma once

#include <memory>
#include <string>
#include "Collision/CollisionSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Input/KeyboardInputController.h"
#include "Maps/OverworldMap.h"
#include "Maps/ScrollableOverworldMap.h"
#include "Objects/Noah.h"
#include "States/IGameState.h"

namespace STATES
{
    /// The main gameplay state that involves the player (Noah)
    /// moving over an overworld.
    class OverworldState : public IGameState
    {
    public:
        /// Constructor.
        /// @param[in]  graphics_system - The graphics system.
        /// @param[in]  collision_system - The collision system.
        /// @throws std::runtime_error - Thrown if the an error occurs during initialization.
        explicit OverworldState(
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system,
            std::shared_ptr<COLLISION::CollisionSystem>& collision_system);
        /// Destructor.
        virtual ~OverworldState();

        /// @copydoc    IGameState::Update
        virtual bool Update(const float elapsed_time_in_seconds);

    private:
        OverworldState(const OverworldState&);  ///< Private to disallow copying.
        OverworldState& operator= (const OverworldState&); ///< Private to disallow assignment.

        /// Attempts to load the overworld map from the provided specification file.
        /// @param[in]  overworld_spec_filepath - The filepath to the overworld map specification.
        /// @return True if the overworld was loaded successfully; false otherwise.
        bool LoadOverworldMap(const std::string& overworld_spec_filepath);

        /// Initializes the player object in the provided Noah character.
        /// @param[out] noah_player - The player Noah character, if successfully initialized.
        /// @return True if the player is successfully initialized; false otherwise.
        bool InitializePlayer(OBJECTS::Noah& noah_player);

        /// Handles user input for a single frame.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for the frame to handle input for.
        /// @param[in]  input_controller - The controller supplying user input.
        void HandleUserInput(
            const float elapsed_time_in_seconds,
            INPUT_CONTROL::KeyboardInputController& input_controller);

        GRAPHICS::Camera Camera;  ///< The camera indicating the portion of the world being shown.

        std::shared_ptr<RESOURCES::Assets> Assets;  ///< Assets for the game.
        std::shared_ptr<GRAPHICS::GraphicsSystem> GraphicsSystem; ///< The graphics system.
        INPUT_CONTROL::KeyboardInputController InputController;  ///< The controller supplying user input.

        std::shared_ptr<COLLISION::CollisionSystem> CollisionSystem; ///< The collision system.

        MAPS::OverworldMapSpecification OverworldSpec;    ///< The specification of the overworld map.
        std::shared_ptr<MAPS::OverworldMap> OverworldMap; ///< The overworld map of the game.
        std::unique_ptr<MAPS::ScrollableOverworldMap> ScrollingOverworld;  ///< Wraps the overworld map to manage scrolling.

        OBJECTS::Noah NoahPlayer; ///< Noah, the main character controlled by the player.
    };
}