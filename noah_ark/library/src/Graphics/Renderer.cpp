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

    /// Renders an overworld.
    /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last rendering of the world.
    ///     @todo   Look at ways to remove this parameter.
    /// @param[in]  overworld - The overworld to render.
    ///     @todo   Look at how to make overworld parameter const.
    /// @param[in,out]  render_target - The target to render to.
    void Renderer::Render(
        const float elapsed_time_in_seconds, 
        MAPS::Overworld& overworld, 
        sf::RenderTarget& render_target)
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
                Render(*tile_map, render_target);
            }
        }

        // RENDER THE PLAYER.
        // Make sure his axe/sprite are updated.
        overworld.NoahPlayer.Inventory->Axe->Update(elapsed_time_in_seconds);
        overworld.NoahPlayer.Sprite.Update(elapsed_time_in_seconds);
        Render(overworld.NoahPlayer.Sprite, render_target);

        // The axe should only be rendered if it is swinging.
        if (overworld.NoahPlayer.Inventory->Axe->IsSwinging())
        {
            Render(overworld.NoahPlayer.Inventory->Axe->Sprite, render_target);
        }
    }

    /// Renders a tile map.
    /// @param[in]  tile_map - The tile map to render.
    /// @param[in,out]  render_target - The target to render to.
    void Renderer::Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target)
    {
        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
        MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
            {
                const std::shared_ptr<MAPS::Tile>& tile = tile_map.Ground.Tiles(tile_column, tile_row);
                Render(tile->Sprite, render_target);
            }
        }

        // RENDER THE CURRENT TILE MAP'S WOOD LOGS.
        for (const auto& wood_log : tile_map.WoodLogs)
        {
            Render(wood_log.Sprite, render_target);
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            Render(tree.Sprite, render_target);
        }

        // RENDER THE CURRENT TILE MAP'S DUST CLOUDS.
        for (const auto& dust_cloud : tile_map.TreeDustClouds)
        {
            Render(dust_cloud.Sprite, render_target);
        }
    }

    /// Renders a sprite.
    /// @param[in]  sprite - The sprite to render.
    /// @param[in,out]  render_target - The target to render to.
    void Renderer::Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target)
    {
        sprite.Render(render_target);
    }

    /// Renders an animated sprite.
    /// @param[in]  sprite - The animated sprite to render.
    /// @param[in,out]  render_target - The target to render to.
    void Renderer::Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target)
    {
        sprite.Render(render_target);
    }

    /// Renders a HUD.
    /// @param[in]  hud - The HUD to render.
    /// @param[in,out]  render_target - The target to render to.
    void Renderer::Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, sf::RenderTarget& render_target)
    {
        hud.Render(render_target);
    }
}
