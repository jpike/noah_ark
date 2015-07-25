#include <iostream>
#include "World/WorldArea.h"

namespace WORLD
{
    WorldArea::WorldArea(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    CenterWorldPosition(center_world_position),
    DimensionsInTiles(dimensions_in_tiles),
    Ground(center_world_position, dimensions_in_tiles),
    Trees(),
    Noah()
    {}

    void WorldArea::SetGroundTile(
        const unsigned int x_offset_from_left_in_tiles,
        const unsigned int y_offset_from_top_in_tiles,
        const std::shared_ptr<MAPS::Tile>& tile)
    {
        Ground.SetTile(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles, tile);
    }

    void WorldArea::AddTree(const std::shared_ptr<OBJECTS::Tree>& tree)
    {
        Trees.push_back(tree);
    }

    void WorldArea::SetNoah(const std::shared_ptr<OBJECTS::Noah>& noah)
    {
        Noah = noah;
    }

    std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > WorldArea::GetRenderables() const
    {
        std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > renderables;

        renderables = Ground.GetRenderables();

        for (const auto& tree : Trees)
        {
            renderables.push_back(tree->GetSprite());
        }

        renderables.push_back(Noah->GetSprite());

        return renderables;
    }

    bool WorldArea::ContainsPosition(const MATH::Vector2f& world_position) const
    {
        MATH::FloatRectangle world_bounds = GetWorldBounds();
        bool position_within_bounds = world_bounds.Contains(world_position.X, world_position.Y);
        return position_within_bounds;
    }

    MATH::FloatRectangle WorldArea::GetWorldBounds() const
    {
        MATH::Vector2f map_dimensions = Ground.GetSizeInPixels();

        MATH::FloatRectangle world_bounds = MATH::FloatRectangle::FromCenterAndDimensions(
            CenterWorldPosition.X,
            CenterWorldPosition.Y,
            map_dimensions.X,
            map_dimensions.Y);
        return world_bounds;
    }
}