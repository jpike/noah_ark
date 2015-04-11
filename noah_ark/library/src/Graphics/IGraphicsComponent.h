#pragma once

#include <SFML/Graphics.hpp>

namespace GRAPHICS
{   
    ///////////////////////////////////////////////////////////
    /// @brief  An interface for graphics components.  Graphics
    ///         components provide a way for game objects to
    ///         have a visual representation in the game.
    ///////////////////////////////////////////////////////////
    class IGraphicsComponent
    {
    public:
        /// @brief  Virtual destructor for this interface.
        virtual ~IGraphicsComponent() {};

        /// @brief  Indicates whether or not the graphics component is visible.
        /// @return The visibility of the graphics component.
        virtual bool IsVisible() const = 0;

        /// @brief          Renders the graphics component.
        /// @param[in,out]  renderTarget - The target to render to.
        virtual void Render(sf::RenderTarget& renderTarget) = 0;

        /// @brief      Updates the graphics component for a single frame based on
        ///             the provided time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame
        ///             for wich to update the graphics component.
        virtual void Update(const float elapsedTimeInSeconds) = 0;
    };
}