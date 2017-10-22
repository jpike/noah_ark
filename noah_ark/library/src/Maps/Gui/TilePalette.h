#pragma once

#include <memory>
#include <unordered_map>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Input/InputController.h"
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

        // INPUT METHODS.
        std::shared_ptr<Tile> RespondToInput(const INPUT_CONTROL::InputController& input_controller);

        // RENDERING METHODS.
        void Render(GRAPHICS::Renderer& renderer) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the tile palette is currently visible; false otherwise.
        bool Visible;
    private:
        // TILE RETRIEVAL.
        std::shared_ptr<Tile> GetTileAtScreenPosition(const MATH::Vector2f& screen_position) const;

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
