#pragma once

#include <memory>
#include <vector>
#include "Core/Array2D.h"
#include "Graphics/IGraphicsComponent.h"
#include "Maps/Tile.h"
#include "Math/Vector2.h"
#include "Objects/Noah.h"
#include "Objects/Tree.h"
#include "World/GroundLayer.h"

namespace WORLD
{
    /// A smaller area that forms a subset of the entire world.  By dividing the
    /// world into these smaller areas, memory footprint can be reduced by
    /// only having a few of these smaller areas in memory at any given time.
    class WorldArea
    {
    public:
        /// Constructor.
        /// @param[in]  center_world_position - The world position of the center
        ///     of the area.
        /// @param[in]  dimensions_in_tiles - The dimensions of the area in tiles.
        explicit WorldArea(
            const MATH::Vector2f& center_world_position,
            const MATH::Vector2ui& dimensions_in_tiles);

        /// Populates the tile at the specified offset on the ground in the area.
        /// @param[in]  x_offset_from_left_in_tiles - The x offset from the left of
        ///     the area, in tiles.
        /// @param[in]  y_offset_from_top_in_tiles - The y offset from the top of
        ///     the area, in tiles.
        /// @param[in]  tile - The tile to set at the specified position.  Its
        ///     world position will be populated to reflect its positioning
        ///     within this area within the larger world.
        void SetGroundTile(
            const unsigned int x_offset_from_left_in_tiles,
            const unsigned int y_offset_from_top_in_tiles,
            const std::shared_ptr<MAPS::Tile>& tile);

        /// Adds a tree to the world area.
        /// @todo   Should this verify the tree's position is actually within this area's bounds?
        /// @param[in]  tree - The tree to add to the area.
        void AddTree(const std::shared_ptr<OBJECTS::Tree>& tree);

        /// Sets the instance of Noah in this world area.  Only one instance of Noah
        /// is allowed to exist in a world area, so any previous instance will be replaced.
        /// @param[in]  noah - The instance of Noah to set in this world area.
        void SetNoah(const std::shared_ptr<OBJECTS::Noah>& noah);

        /// Gets all renderable objects within the area.
        /// @return The renderable versions of objects in this layer.
        std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > GetRenderables() const;

        /// Determines if the provided world position is within this world area.
        /// @param[in]  world_position - The world position to check.
        /// @return True if the position is within this world area; false otherwise.
        bool ContainsPosition(const MATH::Vector2f& world_position) const;

        /// Gets the bounding box of the world area.
        /// @return The boundaries of the world area.
        MATH::FloatRectangle GetWorldBounds() const;

    private:

        /// The world position of the center of the area.
        MATH::Vector2f CenterWorldPosition;
        /// The dimensions of the area in tiles.
        MATH::Vector2ui DimensionsInTiles;
        /// The ground in this area.
        GroundLayer Ground;
        /// Trees in this area.
        std::vector< std::shared_ptr<OBJECTS::Tree> > Trees;
        /// Noah, if he exists in this area.
        std::shared_ptr<OBJECTS::Noah> Noah;
    };
}