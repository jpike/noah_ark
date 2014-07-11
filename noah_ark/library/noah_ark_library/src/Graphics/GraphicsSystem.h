#pragma once

#include <hgeresource.h>
#include <list>
#include <memory>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationSequence.h"
#include "Graphics/Camera.h"
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

/// A namespace for graphics code.
namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  The graphics system for the game.  Manages and renders
    ///         graphics components.
    ///////////////////////////////////////////////////////////
    class GraphicsSystem
    {
    public:
        // CONSTANTS DEFINING THE Z-VALUES TO SET FOR LAYERING GRAPHICS ON TOP FOR EACH OTHER.
        static const float GROUND_LAYER_Z_VALUE;    ///< The z-value for graphics on the ground level.
        static const float PLAYER_LAYER_Z_VALUE;    ///< The z-value for graphics of the player.
        
        /// @brief      Constructor.
        /// @param      pGameEngine - The underlying HGE game engine.  Must not be NULL.
        /// @param[in]  resourceManager - The resource manager used to access graphics resources.
        explicit GraphicsSystem(HGE* const pGameEngine, const std::shared_ptr<hgeResourceManager>& resourceManager);
        /// @brief  Destructor.
        ~GraphicsSystem();

        /// @brief  Renders all visible graphics components managed by this system.
        ///         Components are rendered in order of creation within the graphics
        ///         system and relies on z-buffering to handle certains components
        ///         being in back/front of others.  Therefore, you should make
        ///         sure that z values have been properly set for all components objects.
        void Render();

        /// @brief      Sets the camera used by the graphics system for rendering.
        /// @param[in]  camera - The camera indicating the viewable area of the world.
        void SetCamera(const Camera& camera);

        /// @brief      Gets a texture resource with the specified name.
        ///             Textures are not automatically rendered by the graphics system
        ///             and must be explicitly attached to a graphics component that
        ///             does get rendered.
        /// @param[in]  textureResourceName - The name of the texture resource to retrieve,
        ///             as specified in a resource script.
        /// @return     The texture resource, if found; nullptr otherwise.
        std::shared_ptr<Texture> GetTexture(const std::string& textureResourceName);

        /// @brief      Gets an animation resource with the specified name.
        ///             Animations are not automatically rendered by the graphics system
        ///             and must be explicitly attached to a graphics component that
        ///             does get rendered.
        /// @param[in]  animationResourceName - The name of the animation resource to retrieve,
        ///             as specified in a resource script.
        /// @return     The animation resource, if found; nullptr otherwise.
        std::shared_ptr<AnimationSequence> GetAnimationSequence(const std::string& animationResourceName);
        
        /// @brief      Creates a new sprite within the graphics system.
        /// @param[in]  texture - The texture resource for the sprite.
        /// @param[in]  xPositionInTexels - The x-coordinate of the left of the rectangle within 
        ///             the texture to use for the sprite image.
        /// @param[in]  yPositionInTexels - The y-coordinate of the top of the rectangle within
        ///             the texture to use for the sprite image.
        /// @param[in]  widthInTexels - The width of the rectangle within the texture to use for the sprite image.
        /// @param[in]  heightInTexels - The height of the rectangle within the texture to use for the sprite image..
        /// @return     The newly created sprite.
        std::shared_ptr<Sprite> CreateSprite(
            const std::shared_ptr<Texture>& texture,
            const float xPositionInTexels,
            const float yPositionInTexels,
            const float widthInTexels,
            const float heightInTexels);

        /// @brief  Creates a new animated sprite within the graphics system.
        /// @return The newly created animated sprite.
        std::shared_ptr<AnimatedSprite> CreateAnimatedSprite();

    private:
        GraphicsSystem(const GraphicsSystem& systemToCopy); ///< Private to disallow copying.
        GraphicsSystem& operator= (const GraphicsSystem& rhsSystem);    ///< Private to disallow assignment.

        /// @brief      Renders the provided graphics component if it is visible.
        /// @param[in]  graphicsComponent - The graphics component to render.
        void RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent);

        HGE* m_pGameEngine; ///< The underlying HGE game engine.
        Camera m_camera;    ///< The camera indicating what portion of the world is currently in view.
        std::shared_ptr<hgeResourceManager> m_resourceManager;   ///< The resource manager used to access graphics resources.
        std::list< std::shared_ptr<IGraphicsComponent> > m_graphicsComponents;    ///< The graphics components to be rendered.
    };
}