#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    Tile::Tile() :
    Id(std::numeric_limits<TileId>::max()),
    Sprite()
    {}

    Tile::Tile(
        const TileId id,
        const GRAPHICS::Sprite& sprite) :
    Id(id),
    Sprite(sprite)
    {}

    void Tile::SetWorldPosition(const float world_x_position, const float world_y_position)
    {
        Sprite.SetWorldPosition(world_x_position, world_y_position);
    }

    float Tile::GetWidthInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetWidth();
    }

    float Tile::GetHeightInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetHeight();
    }
}
