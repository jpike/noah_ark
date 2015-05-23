#include <array>
#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    Tile::Tile() :
    TileId(std::numeric_limits<unsigned int>::max()),
    Sprite()
    {}

    Tile::Tile(
        const unsigned int tile_id,
        const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    TileId(tile_id),
    Sprite(sprite)
    {}

    Tile::Tile(const Tile& tile_to_copy) :
    TileId(),
    Sprite()
    {
        Copy(tile_to_copy);
    }

    Tile::~Tile()
    {
        // Nothing else to do.
    }

    Tile& Tile::operator= (const Tile& rhs_tile)
    {
        // CHECK FOR SELF-ASSIGNMENT.
        bool self_assignment = (this == &rhs_tile);
        if (!self_assignment)
        {
            // Copy the values of the right-hand side tile into this tile.
            Copy(rhs_tile);
        }

        // RETURN THIS OBJECT.
        return (*this);
    }

    std::shared_ptr<Tile> Tile::Clone() const
    {
        // CLONE THE SPRITE IF ONE EXISTS.
        std::shared_ptr<GRAPHICS::Sprite> cloned_sprite;
        bool sprite_exists = (nullptr != Sprite);
        if (sprite_exists)
        {
            cloned_sprite = Sprite->Clone();
        }
        
        // CREATE THE CLONED TILE.
        std::shared_ptr<Tile> cloned_tile = std::make_shared<Tile>(
            TileId,
            cloned_sprite);
        return cloned_tile;
    }

    bool Tile::IsWalkable() const
    {
        // DEFINE ID CONSTANTS FOR EACH TILE IN THE TILESET.
        /// @todo   Re-think invalid tile ID stuff here.
        /// A 0 entry had to be added here to account for
        /// the fact that the first tile ID read from
        /// the tile map file started at 1, not 0.
        const unsigned int INVALID_TILE_ID = 0;
        const unsigned int BLACK_TEST_TILE_ID = 1;
        const unsigned int WHITE_TEST_TILE_ID = 2;
        const unsigned int SAND_TILE_ID = 3;
        const unsigned int GRASS_TILE_ID = 4;
        const unsigned int WATER_TILE_ID = 5;
        const unsigned int BROWN_DIRT_TILE_ID = 6;
        const unsigned int GRAY_STONE_TILE_ID = 7;

        // MAP EACH TILE ID TO A WALKABLE STATUS.
        const unsigned int TILESET_TILE_COUNT = 8;
        std::array<bool, TILESET_TILE_COUNT> tile_is_walkable;
        tile_is_walkable[INVALID_TILE_ID] = false;
        tile_is_walkable[BLACK_TEST_TILE_ID] = false;
        tile_is_walkable[WHITE_TEST_TILE_ID] = false;
        tile_is_walkable[SAND_TILE_ID] = true;
        tile_is_walkable[GRASS_TILE_ID] = true;
        tile_is_walkable[WATER_TILE_ID] = false;
        tile_is_walkable[BROWN_DIRT_TILE_ID] = true;
        tile_is_walkable[GRAY_STONE_TILE_ID] = true;

        // CHECK IF THIS TILE'S ID IS VALID.
        bool tile_id_valid = (TileId < TILESET_TILE_COUNT);
        if (tile_id_valid)
        {
            // Return the walkable status of the valid tile.
            return tile_is_walkable[TileId];
        }
        else
        {
            // The tile ID is invalid, so assume the tile isn't walkable.
            return false;
        }
    }

    float Tile::GetWidthInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetWidth();
    }

    float Tile::GetHeightInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetHeight();
    }

    float Tile::GetLeftXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetLeftXPosition();
    }

    float Tile::GetRightXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetRightXPosition();
    }

    float Tile::GetTopYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetTopYPosition();
    }

    float Tile::GetBottomYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetBottomYPosition();
    }

    void Tile::SetTopLeftWorldPosition(
        const float top_world_position,
        const float left_world_position)
    {
        // MAKE SURE A SPRITE EXISTS.
        bool sprite_exists = (nullptr != Sprite);
        if (!sprite_exists)
        {
            // The position can't be set without a sprite.
            return;
        }

        /// @todo   Re-think if tiles should be positioned at
        /// the top-left corner.  This contradicts some other code.
        Sprite->SetWorldPosition(left_world_position, top_world_position);
    }

    void Tile::Render(sf::RenderTarget& render_target)
    {
        // ONLY RENDER THE SPRITE IF IT EXISTS.
        bool sprite_exists = (nullptr != Sprite);
        if (sprite_exists)
        {
            Sprite->Render(render_target);
        }
    }

    void Tile::Copy(const Tile& tile_to_copy)
    {
        this->TileId = tile_to_copy.TileId;
        this->Sprite = tile_to_copy.Sprite;
    }
}
