#pragma once

#include <memory>
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

namespace OBJECTS
{
    /// A piece of the ark that can be built in the overworld.
    /// The entire ark is segmented into tile-like pieces on the overworld map
    /// that can be individually built using wood logs collected by the player.
    /// An ark piece always exists in the appropriate spot on a tile map for
    /// simplicity's sake, but it isn't visible until it gets built.
    class ArkPiece
    {
    public:
        // CONSTRUCTION.
        explicit ArkPiece(const unsigned int id, const std::shared_ptr<GRAPHICS::Texture>& texture);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// An ID for the ark piece that identifies which graphical tile is used to display it.
        unsigned int Id;
        /// The graphical sprite for the ark piece.
        GRAPHICS::Sprite Sprite;
        /// True if the ark piece has been built yet; false otherwise.
        bool Built;
    };
}
