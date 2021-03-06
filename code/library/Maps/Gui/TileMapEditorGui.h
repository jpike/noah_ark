#pragma once

#include <memory>
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Input/InputController.h"
#include "Maps/Gui/TilePalette.h"
#include "Maps/Tile.h"
#include "Maps/TileMap.h"
#include "Math/Vector2.h"

namespace MAPS
{
/// Holds code related to GUI functionality for maps.
namespace GUI
{
    /// A developer/debug GUI used for editing a single tile map on the screen.
    class TileMapEditorGui
    {
    public:
        // INPUT METHODS.
        void RespondToInput(const INPUT_CONTROL::InputController& input_controller);

        // RENDERING METHODS.
        void Render(GRAPHICS::Renderer& renderer) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the GUI is currently visible; false otherwise.
        bool Visible = false;
        /// The current tile map being edited by the GUI.
        MAPS::TileMap* CurrentTileMap = nullptr;
    private:
        // PRIVATE HELPER METHODS.
        static void WriteTileMapFile(const MAPS::TileMap& tile_map);

        // MEMBER VARIABLES.
        /// The palette of tiles from which a user can select tiles in the editor.
        MAPS::GUI::TilePalette TilePalette = MAPS::GUI::TilePalette();
        /// The currently selected tile (if one is selected).
        std::shared_ptr<Tile> SelectedTile = nullptr;
        /// The most recent screen position of the mouse.
        MATH::Vector2f MouseScreenPosition = MATH::Vector2f();
    };
}
}
