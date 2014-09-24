#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationSequence.h"
#include "Graphics/Camera.h"
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Resources/ResourceManager.h"

/// A namespace for graphics code.
namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Defines different layers that graphics components
    ///         may be placed for rendering.  Lower layers are
    ///         defined first, so they will rendered below components
    ///         on higher layers.
    ///////////////////////////////////////////////////////////
    enum class GraphicsLayer
    {
        GROUND, ///< The layer defined for physical ground (i.e. tiles).
        PLAYER  ///< The layer defined for the player character.
    };

    ///////////////////////////////////////////////////////////
    /// @brief  The graphics system for the game.  Manages and renders
    ///         graphics components.
    ///////////////////////////////////////////////////////////
    class GraphicsSystem
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  renderTarget - The target which the graphics system should render to.
        /// @param[in]  resourceManager - The resource manager used to access graphics resources.
        explicit GraphicsSystem(
            const std::shared_ptr<sf::RenderTarget>& renderTarget,
            const std::shared_ptr<RESOURCES::ResourceManager>& resourceManager);
        /// @brief  Destructor.
        ~GraphicsSystem();

        /// @brief          Renders all visible graphics components managed by this system.
        ///                 Components are rendered in order of creation within the graphics
        ///                 system and relies on z-buffering to handle certains components
        ///                 being in back/front of others.  Therefore, you should make
        ///                 sure that z values have been properly set for all components objects.
        void Render();

        /// @brief      Updates all graphics components (including invisible ones) that are
        ///             managed by this system for a single frame, based on the provided
        ///             amount of time.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame for
        ///             which to update the graphics components.
        void Update(const float elapsedTimeInSeconds);

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
        /// @param[in]  heightInTexels - The height of the rectangle within the texture to use for the sprite image.
        /// @param[in]  graphics_layer - The visual layer in which the sprite is to be created and rendered.
        /// @return     The newly created sprite.
        std::shared_ptr<Sprite> CreateSprite(
            const std::shared_ptr<Texture>& texture,
            const float xPositionInTexels,
            const float yPositionInTexels,
            const float widthInTexels,
            const float heightInTexels,
            const GraphicsLayer graphicsLayer);

        /// @brief      Creates a new animated sprite within the graphics system.
        /// @param[in]  spriteResourceName - The name of the basic sprite resource to be animated.
        /// @param[in]  graphics_layer - The visual layer in which the sprite is to be created and rendered.
        /// @return     The newly created animated sprite; nullptr if an error occurs.
        std::shared_ptr<AnimatedSprite> CreateAnimatedSprite(
            const std::string& spriteResourceName,
            const GraphicsLayer graphicsLayer);

    private:
        GraphicsSystem(const GraphicsSystem& systemToCopy); ///< Private to disallow copying.
        GraphicsSystem& operator= (const GraphicsSystem& rhsSystem);    ///< Private to disallow assignment.

        /// @brief          Renders the provided graphics component if it is visible.
        /// @param[in,out]  graphicsComponent - The graphics component to render.
        /// @param[in,out]  renderTarget - The target to render to.
        void RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent, sf::RenderTarget& renderTarget);

        /// @brief  Removes unusued graphics components from the system.  Allows freeing
        ///         memory for graphics components that are no longer being used elsewhere
        ///         in the game.
        void RemoveUnusedGraphicsComponents();

        std::shared_ptr<sf::RenderTarget> m_renderTarget;   ///< The target that is rendered to.
        Camera m_camera;    ///< The camera indicating what portion of the world is currently in view.
        std::shared_ptr<RESOURCES::ResourceManager> m_resourceManager;   ///< The resource manager used to access graphics resources.

        /// @brief  The graphics components to be rendered, keyed based on their layer.
        ///         The methods for creating these components return shared_ptrs, but they
        ///         are stored as weak_ptrs to allow their memory to be properly released 
        ///         once the objects holding the shared_ptrs are deleted.
        std::unordered_multimap< GraphicsLayer, std::weak_ptr<IGraphicsComponent> > m_graphicsComponents;
    };
}