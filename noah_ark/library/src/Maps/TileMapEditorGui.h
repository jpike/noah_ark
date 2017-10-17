#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Maps/Tile.h"
#include "Maps/Tileset.h"

namespace MAPS
{
    /// A developer/debug GUI used for editing a single tile map on the screen.
    class TileMapEditorGui
    {
    public:
        // CONSTRUCTION.
        explicit TileMapEditorGui(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture);

        // RENDERING METHODS.
        void Render(GRAPHICS::Renderer& renderer) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the GUI is currently visible; false otherwise.
        bool Visible;

    private:
        /// The tileset for maps edited via the GUI.
        MAPS::Tileset Tileset;
        /// A mapping of tiles in the tileset by ID.
        /// Used for quickly rendering each tile.
        std::unordered_map< MAPS::TileType::Id, std::shared_ptr<Tile> > TilesById;
    };
}
