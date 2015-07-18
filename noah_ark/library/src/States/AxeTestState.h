#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Objects/Axe.h"
#include "Resources/Assets.h"
#include "States/IGameState.h"

namespace STATES
{
    /// A state for testing out swinging of the axe.
    class AxeTestState : public IGameState
    {
    public:
        /// Constructor.
        explicit AxeTestState(
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system);
        /// Destructor.
        virtual ~AxeTestState();

        /// @copydoc    IGameState::Update
        virtual bool Update(const float elapsed_time_in_seconds);

    private:

        RESOURCES::Assets GraphicsAssets;

        /// The system for rendering graphics.
        std::shared_ptr<GRAPHICS::GraphicsSystem> GraphicsSystem;

        
        /// The axe being tested.
        std::shared_ptr<OBJECTS::Axe> Axe;
    };
}