#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "Graphics/Texture.h"
#include "Maps/Tile.h"
#include "Math/Rectangle.h"

namespace MAPS
{
    /// A collection of tile data from which tiles can be created.
    class Tileset
    {
    public:
        explicit Tileset();

        void SetTile(
            const TileId tile_id, 
            const MATH::FloatRectangle& texture_sub_rectangle);

        std::shared_ptr<Tile> CreateTile(const TileId tile_id) const;

    private:
        /// Sub-rectangles within textures for tiles mapped by tile ID.
        std::unordered_map<TileId, MATH::FloatRectangle> TileTextureSubRectangles = std::unordered_map<TileId, MATH::FloatRectangle>();
    };
}