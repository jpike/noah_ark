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

        /// @brief      Sets the Z-position for the graphics component.  This is used
        ///             to order how graphics are layered on top of each other.
        ///             Higher values are on the bottom.
        /// @param[in]  zPosition - The z-position to set for the graphics component.
        virtual void SetZPosition(const float zPosition) = 0;
    };
}