#include <string>
#include "Debugging/DebugConsole.h"
#include "Maps/Gui/TileMapEditorGui.h"

namespace MAPS
{
namespace GUI
{
    /// Creates an invisible instance of the tile map editor GUI.
    /// @param[in]  tileset_texture - The texture for the tileset used by the GUI.
    /// @throws std::exception - Thrown if the parameter is null or a tile fails to be created.
    TileMapEditorGui::TileMapEditorGui(const std::shared_ptr<GRAPHICS::Texture>& tileset_texture) :
    Visible(false),
    TilePalette(tileset_texture),
    SelectedTile(),
    MouseScreenPosition()
    {}

    /// Updates the tile map editor GUI in response to user input.
    /// @param[in,out]  input_controller - The input controller supplying user input.
    void TileMapEditorGui::RespondToInput(const INPUT_CONTROL::InputController& input_controller)
    {
        // TOGGLE THE MAP EDITOR IF THE KEY WAS PRESSED.
        const bool map_editor_key_pressed = input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::MAP_EDITOR_KEY);
        if (map_editor_key_pressed)
        {
            // TOGGLE THE MAIN GUI.
            Visible = !Visible;

            // TOGGLE THE TILE PALETTE.
            // It should be made visible if the map editor is being opened.
            TilePalette.Visible = Visible;
        }

        // EXIT IF THE GUI ISN'T VISIBLE.
        // If the GUI isn't visible, then it shouldn't be responsive to any more user input.
        if (!Visible)
        {
            return;
        }

        // TRACK THE MOUSE SCREEN POSITION.
        MouseScreenPosition = input_controller.Mouse.ScreenPosition;

        // GET ANY NEWLY SELECTED TILE FROM THE TILE PALETTE.
        std::shared_ptr<MAPS::Tile> newly_selected_tile = TilePalette.RespondToInput(input_controller);
        if (newly_selected_tile)
        {
            SelectedTile = newly_selected_tile;
            DEBUGGING::DebugConsole::WriteLine("Tile selected!");
        }
        else
        {
            // CHECK IF A TILE HAS ALREADY BEEN SELECTED.
            if (SelectedTile)
            {
                /// @todo   Response to mouse clicks here.
                /// If a mouse button is clicked, then the tile should be placed
                /// in the tile map.
            }
        }
    }

    /// Renders the GUI if the GUI is visible.
    /// @param[in,out]  renderer - The renderer to use.
    void TileMapEditorGui::Render(GRAPHICS::Renderer& renderer) const
    {
        // MAKE SURE THE GUI IS VISIBLE.
        if (!Visible)
        {
            return;
        }

        // RENDER THE TILES TO SELECT FROM.
        TilePalette.Render(renderer);

        // RENDER ANY SELECTED TILE AT THE MOUSE CURSOR.
        if (SelectedTile)
        {
            renderer.RenderGuiIcon(SelectedTile->Sprite, MouseScreenPosition);
        }
    }
}
}
