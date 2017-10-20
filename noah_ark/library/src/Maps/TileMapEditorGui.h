#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Input/InputController.h"
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

        // INPUT METHODS.
        void RespondToInput(const INPUT_CONTROL::InputController& input_controller);

        // RENDERING METHODS.
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // MEMBER VARIABLES.
        /// True if the GUI is currently visible; false otherwise.
        bool Visible;
        /// The tileset for maps edited via the GUI.
        MAPS::Tileset Tileset;
        /// A mapping of tiles in the tileset by ID.
        /// Used for quickly rendering each tile.
        std::unordered_map< MAPS::TileType::Id, std::shared_ptr<Tile> > TilesById;
        /// The currently selected tile (if one is selected).
        std::shared_ptr<Tile> SelectedTile;
    };
}
