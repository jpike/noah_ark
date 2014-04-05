#pragma once

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  An interface for graphics components.
    ///////////////////////////////////////////////////////////
    class IGraphicsComponent
    {
    public:
        /// @brief  Virtual destructor for this interface.
        virtual ~IGraphicsComponent() {};

        /// @brief  Indicates whether or not the graphics component is visible.
        /// @return The visibility of the graphics component.
        virtual bool IsVisible() const = 0;

        /// @brief  Renders the graphics component.
        virtual void Render() = 0;
    };
}