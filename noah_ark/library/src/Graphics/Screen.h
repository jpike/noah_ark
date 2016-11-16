#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Math/Rectangle.h"

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
        template <typename T>
        MATH::Rectangle<T> GetBoundingRectangle() const;

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

    /// Gets the bounding rectangle of the screen, in screen coordinates.
    /// @tparam T - The type to return the rectangle coordinates as.
    ///     The underlying dimension values must be implicitly convertible
    ///     via a static cast to this type.
    /// @return The bounding rectangle of the screen, in screen coordinates.
    ///     This means that the left/top of the rectangle will always be (0, 0).
    template <typename T>
    MATH::Rectangle<T> Screen::GetBoundingRectangle() const
    {
        const T LEFT_X = 0;
        const T TOP_Y = 0;
        T width_in_pixels = WidthInPixels<T>();
        T height_in_pixels = HeightInPixels<T>();
        MATH::Rectangle<T> screen_bounding_rectangle = MATH::Rectangle<T>::FromLeftTopAndDimensions(
            LEFT_X,
            TOP_Y,
            width_in_pixels,
            height_in_pixels);
        return screen_bounding_rectangle;
    }
}
