#pragma once

namespace RESOURCES
{
    /// Defines the different types of assets supported by the game.
    enum class AssetType
    {
        /// Reserved to indicate an invalid type of asset.
        INVALID,
        /// A \ref GRAPHICS::GUI::Font.
        FONT,
        /// A piece of music.
        MUSIC,
        /// A shader.
        SHADER
    };
}
