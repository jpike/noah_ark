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
        // CONSTRUCTION.
        explicit Screen(const std::shared_ptr<sf::RenderTarget>& render_target);

        // CLEARING.
        void Clear();

        // DIMENSION RETRIEVAL.
        template <typename T>
        T WidthInPixels() const;
        template <typename T>
        T HeightInPixels() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The actual render target for the screen.
        /// Guaranteed to not be null after construction.
        const std::shared_ptr<sf::RenderTarget> RenderTarget;
    };

    /// Gets the width, in pixels, of the screen as the specified data type.
    /// @tparam T - The type to return the width as.  The underlying width
    ///     value must be implicitly convertible via a static cast to this
    ///     type.
    /// @return The width of the screen in pixels.
    template <typename T>
    T Screen::WidthInPixels() const
    {
        sf::Vector2u size_in_pixels = RenderTarget->getSize();
        T width_in_pixels = static_cast<T>(size_in_pixels.x);
        return width_in_pixels;
    }

    /// Gets the height, in pixels, of the screen as the specified data type.
    /// @tparam T - The type to return the height as.  The underlying height
    ///     value must be implicitly convertible via a static cast to this
    ///     type.
    /// @return The height of the screen in pixels.
    template <typename T>
    T Screen::HeightInPixels() const
    {
        sf::Vector2u size_in_pixels = RenderTarget->getSize();
        T height_in_pixels = static_cast<T>(size_in_pixels.y);
        return height_in_pixels;
    }
}
