#include "Graphics/Renderer.h"

namespace GRAPHICS
{
    /// Constructor.
    /// @param[in]  render_target - The target to render to.
    ///     @todo   Try and decouple the camera from the render target
    ///     so that it doesn't have to be passed here.
    Renderer::Renderer(const std::shared_ptr<sf::RenderTarget>& render_target) :
        Camera(render_target)
    {}

    /// @todo   Document.
    void Renderer::Render(const GRAPHICS::GUI::Text& text, Screen& screen)
    {
        text.Render(screen);
    }

    /// @todo   Document.
    void Renderer::RenderScreenRectangle(
        const MATH::FloatRectangle& rectangle,
        const GRAPHICS::Color& color,
        Screen& screen)
    {
        // CONVERT THE RECTANGLE POSITION TO A WORLD POSITION.
        // This is necessary so that the rectangle can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        int left_screen_position = static_cast<int>(rectangle.GetLeftXPosition());
        int top_screen_position = static_cast<int>(rectangle.GetTopYPosition());
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            left_screen_position,
            top_screen_position));

        // CREATE THE RECTANGLE TO RENDER.
        sf::RectangleShape renderable_rectangle;
        renderable_rectangle.setFillColor(sf::Color(color.Red, color.Green, color.Blue));

        float width = rectangle.GetWidth();
        float height = rectangle.GetHeight();
        renderable_rectangle.setSize(sf::Vector2f(width, height));

        renderable_rectangle.setPosition(top_left_world_position);

        // RENDER THE RECTANGLE.
        screen.RenderTarget->draw(renderable_rectangle);
    }

    /// @todo   Document.
    void Renderer::RenderKeyIcon(
        const char key,
        const GRAPHICS::GUI::Font& font,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        Screen& screen)
    {
        // CONVERT THE SCREEN POSITION TO A WORLD POSITION.
        // This is necessary so that the key icon can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));

        // CREATE A RECTANGLE TO RESEMBLE A KEY ON A KEYBOARD.
        const sf::Color DARK_GRAY(128, 128, 128);
        const sf::Color LIGHT_GRAY(176, 176, 176);
        sf::RectangleShape key_background_icon;
        key_background_icon.setFillColor(LIGHT_GRAY);
        key_background_icon.setOutlineColor(DARK_GRAY);
        key_background_icon.setOutlineThickness(2.0f);
        key_background_icon.setSize(sf::Vector2f(
            static_cast<float>(GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS)));
        key_background_icon.setPosition(top_left_world_position);

        // RENDER THE BACKGROUND RECTANGLE FOR THE KEY.
        screen.RenderTarget->draw(key_background_icon);

        // GET THE GLYPH FOR THE KEY.
        GRAPHICS::GUI::Glyph glyph = font.GetGlyph(key);

        // CALCULATE THE CENTER WORLD POSITION OF THE GLYPH.
        float glyph_width = glyph.TextureSubRectangle.GetWidth();
        float glyph_half_width = glyph_width / 2.0f;
        float glyph_center_world_x_position = top_left_world_position.x + glyph_half_width;
        float glyph_height = glyph.TextureSubRectangle.GetHeight();
        float glyph_half_height = glyph_height / 2.0f;
        float glyph_center_world_y_position = top_left_world_position.y + glyph_half_height;
        MATH::Vector2f glyph_center_world_position(
            glyph_center_world_x_position,
            glyph_center_world_y_position);

        // CREATE A SPRITE FOR THE GLYPH.
        Sprite key_character_sprite(
            glyph.Texture,
            glyph.TextureSubRectangle);
        key_character_sprite.SetWorldPosition(glyph_center_world_position);

        // RENDER THE GLYPH FOR THE KEY.
        key_character_sprite.Render(screen);
    }

    /// @todo   Document.
    void Renderer::RenderGuiIcon(
        const GRAPHICS::Texture& texture,
        const MATH::FloatRectangle& texture_sub_rectangle,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        Screen& screen)
    {
        // CREATE A SPRITE FOR THE ICON USING THE TEXTURE INFORMATION.
        sf::IntRect texture_rectangle;
        texture_rectangle.top = static_cast<int>(texture_sub_rectangle.GetTopYPosition());
        texture_rectangle.left = static_cast<int>(texture_sub_rectangle.GetLeftXPosition());
        texture_rectangle.width = static_cast<int>(texture_sub_rectangle.GetWidth());
        texture_rectangle.height = static_cast<int>(texture_sub_rectangle.GetHeight());
        sf::Sprite gui_icon(texture.TextureResource, texture_rectangle);

        // POSITION THE GUI ICON SPRITE.
        // The screen position must be converted to a world position so that the GUI icon
        // can be rendered appropriately on screen regardless of how the camera might
        // move around the world.
        sf::Vector2f top_left_world_position = screen.RenderTarget->mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));
        gui_icon.setPosition(top_left_world_position);

        // RENDER THE GUI ICON.
        screen.RenderTarget->draw(gui_icon);
    }

    /// Renders an overworld.
    /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last rendering of the world.
    ///     @todo   Look at ways to remove this parameter.
    /// @param[in]  overworld - The overworld to render.
    ///     @todo   Look at how to make overworld parameter const.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(
        const float elapsed_time_in_seconds, 
        MAPS::Overworld& overworld, 
        Screen& screen)
    {
        /// @todo   Factor out updating from this method?

        MATH::FloatRectangle camera_bounds = Camera.GetViewBounds();
        MATH::Vector2f camera_view_center = camera_bounds.GetCenterPosition();

        MAPS::TileMap* current_tile_map = overworld.GetTileMap(camera_view_center.X, camera_view_center.Y);
        assert(current_tile_map);

        // RENDER THE CURRENT TILE MAP.
        /// @todo   Consider signed ints.  Right now, we're just
        /// rendering a few surrounding tile maps.
        unsigned int min_tile_map_row = current_tile_map->OverworldRowIndex;
        bool top_tile_map_exists = (min_tile_map_row > 0);
        if (top_tile_map_exists)
        {
            --min_tile_map_row;
        }
        unsigned int min_tile_map_column = current_tile_map->OverworldColumnIndex;
        bool left_tile_map_exists = (min_tile_map_column > 0);
        if (left_tile_map_exists)
        {
            --min_tile_map_column;
        }
        unsigned int max_tile_map_row = min_tile_map_row + 2;
        unsigned int max_tile_map_column = min_tile_map_column + 2;
        for (unsigned int tile_map_row = min_tile_map_row; tile_map_row <= max_tile_map_row; ++tile_map_row)
        {
            for (unsigned int tile_map_column = min_tile_map_column; tile_map_column <= max_tile_map_column; ++tile_map_column)
            {
                // GET THE CURRENT TILE MAP.
                MAPS::TileMap* tile_map = overworld.GetTileMap(tile_map_row, tile_map_column);
                bool tile_map_exists = (nullptr != tile_map);
                if (!tile_map_exists)
                {
                    // Continue trying to render other tile maps.
                    continue;
                }

                // UPDATE THE CURRENT TILE MAP'S TREES.
                /// @todo   Figure out if this maybe should be handled elsewhere.
                for (auto tree = tile_map->Trees.begin(); tree != tile_map->Trees.end(); ++tree)
                {
                    tree->Update(elapsed_time_in_seconds);
                }

                // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
                /// @todo   Maybe we only need a single dust cloud?  At least in theory, it shouldn't be
                /// possible for more than one tree to in this state at a given time.
                for (auto dust_cloud = tile_map->TreeDustClouds.begin(); dust_cloud != tile_map->TreeDustClouds.end();)
                {
                    // UPDATE THE CURRENT DUST CLOUD.
                    dust_cloud->Update(elapsed_time_in_seconds);

                    // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
                    bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
                    if (dust_cloud_disappeared)
                    {
                        // REMOVE THE DUST CLOUD.
                        dust_cloud = tile_map->TreeDustClouds.erase(dust_cloud);
                    }
                    else
                    {
                        // MOVE TO UPDATING THE NEXT DUST CLOUD.
                        ++dust_cloud;
                    }
                }

                // RENDER THE TILE MAP.
                Render(*tile_map, screen);
            }
        }

        // RENDER THE PLAYER.
        // Make sure his axe/sprite are updated.
        overworld.NoahPlayer.Inventory->Axe->Update(elapsed_time_in_seconds);
        overworld.NoahPlayer.Sprite.Update(elapsed_time_in_seconds);
        Render(overworld.NoahPlayer.Sprite, screen);

        // The axe should only be rendered if it is swinging.
        if (overworld.NoahPlayer.Inventory->Axe->IsSwinging())
        {
            Render(overworld.NoahPlayer.Inventory->Axe->Sprite, screen);
        }
    }

    /// Renders a tile map.
    /// @param[in]  tile_map - The tile map to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const MAPS::TileMap& tile_map, Screen& screen)
    {
        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
        MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
            {
                const std::shared_ptr<MAPS::Tile>& tile = tile_map.Ground.Tiles(tile_column, tile_row);
                Render(tile->Sprite, screen);
            }
        }

        // RENDER THE CURRENT TILE MAP'S WOOD LOGS.
        for (const auto& wood_log : tile_map.WoodLogs)
        {
            Render(wood_log.Sprite, screen);
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            Render(tree.Sprite, screen);
        }

        // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
        for (const auto& dust_cloud : tile_map.TreeDustClouds)
        {
            Render(dust_cloud.Sprite, screen);
        }
    }

    /// Renders a sprite.
    /// @param[in]  sprite - The sprite to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const GRAPHICS::Sprite& sprite, Screen& screen)
    {
        sprite.Render(screen);
    }

    /// Renders an animated sprite.
    /// @param[in]  sprite - The animated sprite to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const GRAPHICS::AnimatedSprite& sprite, Screen& screen)
    {
        sprite.Render(screen);
    }

    /// Renders a HUD.
    /// @param[in]  hud - The HUD to render.
    /// @param[in,out]  screen - The screen to render to.
    void Renderer::Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, Screen& screen)
    {
        hud.Render(screen);
    }
}
