#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace GRAPHICS
{
    /// Represents the screen onto which the game may be rendered.
    ///
    /// This only encompasses the actual area in which the game
    /// is displayed - it doesn't include any additional area
    /// like window borders that may be included in an actual
    /// hardware display.
    class Screen
    {
    public:
        // STATIC CONSTANTS.
        /// The width of the screen, in pixels.
        /// Set to be the width of a single tile map.
        static const unsigned int WIDTH_IN_PIXELS = 512;
        /// The height of the screen, in pixels.
        /// Set to be the height of a single tile map.
        static const unsigned int HEIGHT_IN_PIXELS = 384;

        // CONSTRUCTION.
        explicit Screen(const std::shared_ptr<sf::RenderTarget>& render_target);

        // CLEARING.
        void Clear();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The actual render target for the screen.
        /// Guaranteed to not be null after construction.
        const std::shared_ptr<sf::RenderTarget> RenderTarget;
    };
}
