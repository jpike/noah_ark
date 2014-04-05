#pragma once

#include <memory>
#include "Graphics/Sprite.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A 2D graphical tile intended to be used as a building block for tile maps.
    ///////////////////////////////////////////////////////////
    class Tile
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  sprite - The sprite resource used for rendering the tile.
        explicit Tile(const std::shared_ptr<GRAPHICS::Sprite>& sprite);
        /// @brief      Copy constructor.
        /// @param[in]  tileToCopy - The tile to copy.
        explicit Tile(const Tile& tileToCopy);
        /// @brief  Destructor.
        ~Tile();

        /// @brief      Assignment operator.
        /// @param[in]  rhsTile - The tile to assign from.
        Tile& operator= (const Tile& rhsTile);

    private:
        /// @brief      Helper method for copying a tile.
        /// @param[in]  tileToCopy - The tile to copy.
        void Copy(const Tile& tileToCopy);

        std::shared_ptr<GRAPHICS::Sprite> m_sprite;   ///< The sprite used for rendering the tile.
    };
}