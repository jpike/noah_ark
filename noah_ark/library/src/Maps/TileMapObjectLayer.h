#pragma once

#include <string>
#include <vector>
#include "Maps/ITileMapLayer.h"
#include "Maps/TileMapObjectDescription.h"
#include "Maps/Tileset.h"
#include "Objects/Tree.h"

namespace MAPS
{
    /// A single layer of objects in a tile map.
    class TileMapObjectLayer : public ITileMapLayer
    {
    public:
        /// Constructor.
        /// @param[in]  name - A name for the layer.
        /// @param[in]  objects - Descriptions of objects in this layer.
        /// @param[in,out]  graphics_system - The graphics system supplying
        ///     graphical resources for the objects.
        explicit TileMapObjectLayer(
            const std::string& name,
            const std::vector<TileMapObjectDescription>& objects,
            GRAPHICS::GraphicsSystem& graphics_system);
        /// Destructor.
        virtual ~TileMapObjectLayer();

        /// @copydoc    ITileMapLayer::GetTileAtGridPosition
        /// Object layers do not have tiles, so this will always return null.
        virtual std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position,
            const unsigned int tile_y_grid_position) const;

        /// @copydoc    ITileMapLayer::Render
        virtual void Render(sf::RenderTarget& render_target);

    private:

        /// The name of the layer.  Primarily used for debugging purposes.
        std::string Name;
        /// Trees in the layer.
        std::vector<OBJECTS::Tree> Trees;
    };
}
