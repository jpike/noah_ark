#include "Maps/Tileset.h"
#include "Objects/Noah.h"
#include "Objects/Tree.h"
#include "Resources/Assets.h"
#include "States/WorldAreaTestState.h"

namespace STATES
{
    WorldAreaTestState::WorldAreaTestState(
        std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system) :
    GraphicsSystem(graphics_system)
    {
        // GENERATE TILESET.
        std::shared_ptr<GRAPHICS::Texture> tileset_texture = graphics_assets.GetTexture("res/images/ground_tile_set.png");
        MAPS::Tileset tileset;
        MAPS::TileId MIN_TILE_ID = 1;
        MAPS::TileId MAX_TILE_ID = 7;
        for (MAPS::TileId tile_id = MIN_TILE_ID; tile_id <= MAX_TILE_ID; ++tile_id)
        {
            const int TILE_DIMENSION = 16;
            const int FIRST_CENTER_X_OFFSET = 8;
            int center_x_position = FIRST_CENTER_X_OFFSET + (tile_id - 1) * TILE_DIMENSION;
            int center_y_position = TILE_DIMENSION / 2;
            int width = TILE_DIMENSION;;
            int height = TILE_DIMENSION;
            sf::IntRect tile_subrect(
                (tile_id - 1) * TILE_DIMENSION, // left
                0, // top
                width,
                height);

            tileset.SetTile(tile_id, tileset_texture, tile_subrect);
        }

        // GENERATE TILES.
        MATH::Vector2f center_world_position;
        MATH::Vector2ui ground_dimensions_in_tiles(8, 8);
        WorldArea = std::make_shared<WORLD::WorldArea>(center_world_position, ground_dimensions_in_tiles);

        MAPS::TileId tile_id = MIN_TILE_ID;
        for (unsigned int tile_y = 0; tile_y < ground_dimensions_in_tiles.Y; ++tile_y)
        {
            for (unsigned int tile_x = 0; tile_x < ground_dimensions_in_tiles.X; ++tile_x)
            {
                std::shared_ptr<MAPS::Tile> tile = tileset.CreateTile(tile_id);
                bool tile_created = (nullptr != tile);
                if (tile_created)
                {
                    WorldArea->SetGroundTile(tile_x, tile_y, tile);
                }

                ++tile_id;
                tile_id = tile_id % (MAX_TILE_ID + 1);
                if (tile_id == 0)
                {
                    tile_id = 1;
                }
            }
        }

        // GENERATE TREES.
        std::shared_ptr<GRAPHICS::Texture> tree_texture = graphics_assets.GetTexture("res/images/tree_sprite1.png");
        sf::IntRect small_tree_subrect(0, 0, 16, 16);
        sf::IntRect tall_tree_subrect(32, 0, 16, 32);
        sf::IntRect large_tree_subrect(0, 16, 32, 32);

        std::shared_ptr<sf::Sprite> small_tree_sprite_resource = std::make_shared<sf::Sprite>(*tree_texture->GetTextureResource(), small_tree_subrect);
        small_tree_sprite_resource->setOrigin(static_cast<float>(small_tree_subrect.width / 2), static_cast<float>(small_tree_subrect.height / 2));
        small_tree_sprite_resource->setPosition(-48.0f + 8.0f, -48.0f + 8.0f);
        std::shared_ptr<GRAPHICS::Sprite> small_tree_sprite = std::make_shared<GRAPHICS::Sprite>(small_tree_sprite_resource);
        std::shared_ptr<OBJECTS::Tree> small_tree = std::make_shared<OBJECTS::Tree>(small_tree_sprite);

        std::shared_ptr<sf::Sprite> tall_tree_sprite_resource = std::make_shared<sf::Sprite>(*tree_texture->GetTextureResource(), tall_tree_subrect);
        tall_tree_sprite_resource->setOrigin(static_cast<float>(tall_tree_subrect.width / 2), static_cast<float>(tall_tree_subrect.height / 2));
        tall_tree_sprite_resource->setPosition(32.0f + 8.0f, 32.0f);
        std::shared_ptr<GRAPHICS::Sprite> tall_tree_sprite = std::make_shared<GRAPHICS::Sprite>(tall_tree_sprite_resource);
        std::shared_ptr<OBJECTS::Tree> tall_tree = std::make_shared<OBJECTS::Tree>(tall_tree_sprite);

        std::shared_ptr<sf::Sprite> large_tree_sprite_resource = std::make_shared<sf::Sprite>(*tree_texture->GetTextureResource(), large_tree_subrect);
        large_tree_sprite_resource->setOrigin(static_cast<float>(large_tree_subrect.width / 2), static_cast<float>(large_tree_subrect.height / 2));
        large_tree_sprite_resource->setPosition(0.0f, 0.0f);
        std::shared_ptr<GRAPHICS::Sprite> large_tree_sprite = std::make_shared<GRAPHICS::Sprite>(large_tree_sprite_resource);
        std::shared_ptr<OBJECTS::Tree> large_tree = std::make_shared<OBJECTS::Tree>(large_tree_sprite);

        WorldArea->AddTree(small_tree);
        WorldArea->AddTree(tall_tree);
        WorldArea->AddTree(large_tree);

        // ADD NOAH.
        std::shared_ptr<GRAPHICS::Texture> noah_texture = graphics_assets.GetTexture("res/images/noah_sprite1.png");
        sf::IntRect noah_subrect(0, 0, 16, 16);
        std::shared_ptr<sf::Sprite> noah_sprite_resource = std::make_shared<sf::Sprite>(*noah_texture->GetTextureResource(), noah_subrect);
        noah_sprite_resource->setOrigin(8.0f, 8.0f);
        noah_sprite_resource->setPosition(24.0f, -24.0f);
        std::shared_ptr<GRAPHICS::Sprite> sprite = std::make_shared<GRAPHICS::Sprite>(noah_sprite_resource);
        std::shared_ptr<GRAPHICS::AnimatedSprite> noah_sprite = std::make_shared<GRAPHICS::AnimatedSprite>(sprite);
        std::shared_ptr<OBJECTS::Noah> noah = std::make_shared<OBJECTS::Noah>();
        noah->SetSprite(noah_sprite);

        WorldArea->SetNoah(noah);
    }

    WorldAreaTestState::~WorldAreaTestState()
    {}

    bool WorldAreaTestState::Update(const float elapsed_time_in_seconds)
    {
        
        
        // RENDER.
        auto render_target = GraphicsSystem->GetRenderTarget();

        auto default_view = render_target->getDefaultView();

        sf::View camera = default_view;
        camera.setCenter(0.0f, 0.0f);
        camera.setSize(128.0f, 128.0f);
        render_target->setView(camera);

        std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > graphics = WorldArea->GetRenderables();
        for (auto renderable : graphics)
        {
            /// @todo   Null check?
            renderable->Render(*render_target);
        }

        return false;
    }
}