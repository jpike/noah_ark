#pragma once

#include <hge.h>

namespace INPUT_CONTROL
{
    ///////////////////////////////////////////////////////////
    /// @brief  Represents keyboard hardware that can supply user input.
    ///////////////////////////////////////////////////////////
    class Keyboard
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  pGameEngine - The HGE game engine.  Must not be nullptr.
        explicit Keyboard(HGE* const pGameEngine);
        /// @brief  Destructor.
        ~Keyboard();

        /// @brief      Checks if a key is currently pressed down.
        /// @param[in]  keyCode - The HGE virtual key code to check.
        /// @return     True if the key is down; false otherwise.
        bool IsKeyDown(const int keyCode) const;
        /// @brief      Checks if a key is currently not pressed down.
        /// @param[in]  keyCode - The HGE virtual key code to check.
        /// @return     True if the key is up; false otherwise.
	    bool IsKeyUp(const int keyCode) const;
        
    private:

        HGE* const m_pGameEngine; ///< The underlying HGE game engine.
    };
}