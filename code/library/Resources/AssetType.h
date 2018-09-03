#pragma once

namespace RESOURCES
{
    /// Defines the different types of assets supported by the game.
    enum class AssetType
    {
        /// Reserved to indicate an invalid type of asset.
        INVALID,
        /// A generic texture.
        TEXTURE,
        /// A \ref GRAPHICS::GUI::Font.
        FONT,
        /// A sound effect.
        SOUND_EFFECT,
        /// A piece of music.
        MUSIC,
        /// A shader.
        SHADER
    };
}
