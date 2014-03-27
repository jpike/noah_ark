#pragma once

#include <hgesprite.h>
#include "Graphics/IGraphicsComponent.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// \brief  A graphical sprite.
    ///////////////////////////////////////////////////////////
    class Sprite : public IGraphicsComponent
    {
    public:
        /// @brief  Constructor.
        explicit Sprite();
        
        /// @brief  Destructor.
        virtual ~Sprite();

    private:
        hgeSprite* m_pSprite;   ///< The internal sprite.  Memory is externally managed.
    };
}