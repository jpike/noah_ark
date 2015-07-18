#pragma once

#include <SFML/Graphics.hpp>

namespace GRAPHICS
{   
    /// An interface for graphics components.  Graphics
    /// components provide a way for game objects to
    /// have a visual representation in the game.
    class IGraphicsComponent
    {
    public:
        /// Virtual destructor for this interface.
        virtual ~IGraphicsComponent() {};

        /// Indicates whether or not the graphics component is visible.
        /// @return The visibility of the graphics component.
        virtual bool IsVisible() const = 0;

        /// Sets whether the graphics component is visible.
        /// @param[in]  is_visible - True to make the graphics component visible; false to hide it.
        virtual void SetVisible(const bool is_visible) = 0;

        /// Renders the graphics component.
        /// @param[in,out]  render_target - The target to render to.
        virtual void Render(sf::RenderTarget& render_target) = 0;

        /// Updates the graphics component for a single frame based on
        /// the provided time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last frame
        ///     for wich to update the graphics component.
        virtual void Update(const float elapsed_time_in_seconds) = 0;
    };
}