#pragma once

#include "Input/IDebugInputController.h"
#include "Input/Keyboard.h"

namespace INPUT_CONTROL
{
    ///////////////////////////////////////////////////////////
    /// @brief  An debug input controller used for testing
    ///         scrolling the overworld map to other tile maps.
    ///////////////////////////////////////////////////////////
    class MapScrollingDebugInputController : public IDebugInputController
    {
    public:
        /// @brief      Constructor.  Input is enabled by default.
        /// @param[in]  pGameEngine - The HGE game engine.  Must not be nullptr.
        explicit MapScrollingDebugInputController(HGE* const pGameEngine);
        /// @brief  Destructor.
        virtual ~MapScrollingDebugInputController();

        /// @copydoc    IDebugInputController::DisableInput()
        virtual void DisableInput();
        /// @copydoc    IDebugInputController::EnableInput()
        virtual void EnableInput();

        /// @copydoc    IDebugInputController::ScrollUpButtonPressed() const
        virtual bool ScrollUpButtonPressed() const;
        /// @copydoc    IDebugInputController::ScrollDownButtonPressed() const
        virtual bool ScrollDownButtonPressed() const;
        /// @copydoc    IDebugInputController::ScrollLeftButtonPressed() const
        virtual bool ScrollLeftButtonPressed() const;
        /// @copydoc    IDebugInputController::ScrollRightButtonPressed() const
        virtual bool ScrollRightButtonPressed() const;

    private:

        bool m_enabled; ///< Whether or not this controller can supply input.
        Keyboard m_keyboard;    ///< The keyboard supplying user input.
    };
}