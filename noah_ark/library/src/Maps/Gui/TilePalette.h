#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Maps/Tile.h"
#include "Maps/Tileset.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace MAPS
{
namespace GUI
{
    /// A GUI component that displays tiles that can be selected.
    class TilePalette
    {
    public:
        // CONSTRUCTION.
        explicit TilePalette(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture);

        // RENDERING METHODS.
        void Render(GRAPHICS::Renderer& renderer) const;

        // TILE RETRIEVAL.
        std::shared_ptr<Tile> GetTileAtScreenPosition(const MATH::Vector2f& screen_position) const;

    private:
        // MEMBER VARIABLES.
        /// The tileset for maps edited via the GUI.
        MAPS::Tileset Tileset;
        /// A mapping of tiles in the tileset by ID.
        /// Used for quickly rendering each tile.
        std::unordered_map< MAPS::TileType::Id, std::shared_ptr<Tile> > TilesById;
        /// A mapping of screen bounds for each tile by ID.
        std::unordered_map<MAPS::TileType::Id, MATH::FloatRectangle> TileScreenBoundsById;
    };
}
}
