#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Collision/Movement.h"
#include "Core/Direction.h"
#include "Events/AxeSwingEvent.h"
#include "Input/KeyboardInputController.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"

/// The game exited successfully.
int EXIT_CODE_SUCCESS = 0;
/// A standard exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN = 1;
/// An unknown exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN = 2;
/// Game assets failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_ASSETS = 3;

void PopulateOverworld(const MAPS::OverworldMapFile& overworld_map_file, RESOURCES::Assets& assets, MAPS::Overworld& overworld)
{
    // LOAD TILE MAPS FOR EACH ROW.
    for (unsigned int row = 0; row < overworld_map_file.OverworldHeightInTileMaps; ++row)
    {
        // LOAD TILE MAPS FOR EACH COLUMN.
        for (unsigned int column = 0; column < overworld_map_file.OverworldWidthInTileMaps; ++column)
        {
            // GET THE CURRENT TILE MAP FILE.
            const auto& tile_map_file = assets.TileMapFiles(column, row);
            bool tile_map_file_found = (nullptr != tile_map_file);
            if (!tile_map_file_found)
            {
                assert(tile_map_file_found);
                // Continue trying to create other tile maps.
                continue;
            }

            // GET THE TILESET FOR THE CURRENT FILE.
            std::shared_ptr<MAPS::Tileset> tileset = assets.GetTileset(tile_map_file->Tilesets);
            bool tileset_found = (nullptr != tileset);
            if (!tileset_found)
            {
                assert(tileset_found);
                // Continue trying to create other tile maps.
                continue;
            }

            // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
            MATH::Vector2f map_center_world_position;

            float map_width_in_pixels = static_cast<float>(tile_map_file->MapWidthInTiles * tile_map_file->TileWidthInPixels);
            float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
            float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
            map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

            float map_height_in_pixels = static_cast<float>(tile_map_file->MapHeightInTiles * tile_map_file->TileHeightInPixels);
            float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
            float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
            map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

            // CREATE AN EMPTY TILE MAP.
            MATH::Vector2ui map_dimensions_in_tiles(
                tile_map_file->MapWidthInTiles,
                tile_map_file->MapHeightInTiles);
            std::shared_ptr<MAPS::TileMap> tile_map = std::make_shared<MAPS::TileMap>(
                row,
                column,
                map_center_world_position,
                map_dimensions_in_tiles);

            // POPULATE THE TILE MAP FROM THE CURRENT FILE'S LAYERS.
            for (const auto& layer_description : tile_map_file->Layers)
            {
                // DETERMINE THE TYPE OF THE LAYER.
                switch (layer_description.Type)
                {
                    case MAPS::TileMapLayerType::TILE_LAYER:
                    {
                        // CHECK IF THIS LAYER IS THE GROUND LAYER.
                        const std::string GROUND_LAYER_NAME = "GroundLayer";
                        bool is_ground_layer = (GROUND_LAYER_NAME == layer_description.Name);
                        if (is_ground_layer)
                        {
                            // CREATE TILES IN THE GROUND LAYER.
                            for (unsigned int current_tile_y = 0;
                                current_tile_y < tile_map_file->MapHeightInTiles;
                                ++current_tile_y)
                            {
                                // CREATE TILES FOR THIS ROW.
                                for (unsigned int current_tile_x = 0;
                                    current_tile_x < tile_map_file->MapWidthInTiles;
                                    ++current_tile_x)
                                {
                                    // CREATE THE CURRENT TILE.
                                    MAPS::TileId tile_id = layer_description.TileIds(current_tile_x, current_tile_y);
                                    std::shared_ptr<MAPS::Tile> tile = tileset->CreateTile(tile_id);
                                    bool tile_exists_in_tileset = (nullptr != tile);
                                    if (!tile_exists_in_tileset)
                                    {
                                        assert(tile_exists_in_tileset);
                                        // Skip to trying to create the next tile.  The layer
                                        // simply won't have any tile at this location.
                                        continue;
                                    }

                                    // SET THE TILE IN THE GROUND LAYER.
                                    tile_map->Ground.SetTile(current_tile_x, current_tile_y, tile);
                                }
                            }
                        }

                        break;
                    }
                    case MAPS::TileMapLayerType::OBJECT_LAYER:
                    {
                        // CREATE ANY TREES IN THE LAYER.
                        for (const auto& object_description : layer_description.Objects)
                        {
                            const std::string TREE_OBJECT_TYPE = "TREE";
                            bool is_tree = (TREE_OBJECT_TYPE == object_description.Type);
                            if (is_tree)
                            {
                                // GET THE TREE TEXTURE.
                                std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::TREE_TEXTURE_ID);
                                bool tree_texture_retrieved = (nullptr != tree_texture);
                                if (!tree_texture_retrieved)
                                {
                                    assert(tree_texture_retrieved);
                                    // Continue trying to create other objects.
                                    /// @todo   Maybe we should just load this once...as opposed to performing each iteration?
                                    continue;
                                }

                                // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                                // Different sub-rectangles are used depending on the tree's size.
                                sf::IntRect tree_texture_sub_rectangle;
                                const MATH::Vector2ui SMALL_TREE_DIMENSIONS_IN_PIXELS(16, 16);
                                const MATH::Vector2ui TALL_TREE_DIMENSIONS_IN_PIXELS(16, 32);
                                const MATH::Vector2ui LARGE_TREE_DIMENSIONS_IN_PIXELS(32, 32);
                                MATH::Vector2ui tree_dimensions_in_pixels(object_description.WidthInPixels, object_description.HeightInPixels);
                                if (SMALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect SMALL_TREE_TEXTURE_SUB_RECTANGLE(0, 0, 16, 16);
                                    tree_texture_sub_rectangle = SMALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (TALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect TALL_TREE_TEXTURE_SUB_RECTANGLE(32, 0, 16, 32);
                                    tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else if (LARGE_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
                                {
                                    sf::IntRect LARGE_TREE_TEXTURE_SUB_RECTANGLE(0, 16, 32, 32);
                                    tree_texture_sub_rectangle = LARGE_TREE_TEXTURE_SUB_RECTANGLE;
                                }
                                else
                                {
                                    // An invalid size was specified for the tree.
                                    // Continue trying to create other objects.
                                    assert(false);
                                    continue;
                                }

                                // CREATE THE TREE'S SPRITE.
                                std::shared_ptr<sf::Sprite> tree_sprite_resource = std::make_shared<sf::Sprite>(
                                    *tree_texture->GetTextureResource(),
                                    tree_texture_sub_rectangle);

                                // The center of the sprite should be the center of its visible portion of the texture.
                                float tree_local_center_x = static_cast<float>(tree_texture_sub_rectangle.width) / 2.0f;
                                float tree_local_center_y = static_cast<float>(tree_texture_sub_rectangle.height) / 2.0f;
                                tree_sprite_resource->setOrigin(tree_local_center_x, tree_local_center_y);

                                float tree_world_x_position = static_cast<float>(object_description.TopLeftPositionInPixels.X) + tree_local_center_x;
                                float tree_world_y_position = static_cast<float>(object_description.TopLeftPositionInPixels.Y) + tree_local_center_y;
                                tree_sprite_resource->setPosition(tree_world_x_position, tree_world_y_position);

                                GRAPHICS::Sprite tree_sprite(tree_sprite_resource);

                                // CREATE THE TREE.
                                OBJECTS::Tree tree(tree_sprite);
                                tile_map->Trees.push_back(tree);
                            }
                        }

                        break;
                    }
                }
            }

            // SET THE TILE MAP IN THE OVERWORLD.
            overworld.TileMaps(column, row) = tile_map;
        }
    }
}

void InitializePlayer(const MATH::Vector2f& initial_world_position, RESOURCES::Assets& assets, OBJECTS::Noah& noah_player)
{
    // LOAD THE ANIMATION SEQUENCES.
    std::shared_ptr<GRAPHICS::AnimationSequence> walk_front_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);
    bool walk_front_animation_loaded = (nullptr != walk_front_animation);
    if (!walk_front_animation_loaded)
    {
        assert(walk_front_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_back_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID);
    bool walk_back_animation_loaded = (nullptr != walk_back_animation);
    if (!walk_back_animation_loaded)
    {
        assert(walk_back_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_left_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID);
    bool walk_left_animation_loaded = (nullptr != walk_left_animation);
    if (!walk_left_animation_loaded)
    {
        assert(walk_left_animation_loaded);
        return;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walk_right_animation = assets.GetAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID);
    bool walk_right_animation_loaded = (nullptr != walk_right_animation);
    if (!walk_right_animation_loaded)
    {
        assert(walk_right_animation_loaded);
        return;
    }

    // GET THE TEXTURE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> texture = assets.GetTexture(RESOURCES::NOAH_TEXTURE_ID);
    bool texture_loaded = (nullptr != texture);
    if (!texture_loaded)
    {
        assert(texture_loaded);
        return;
    }

    // CREATE THE SPRITE FOR NOAH.
    /// @todo   Better way to determine initial subrect.
    const sf::IntRect TEXTURE_SUB_RECT(0, 0, 16, 16);
    std::shared_ptr<sf::Sprite> sprite_resource = std::make_shared<sf::Sprite>(*texture->GetTextureResource(), TEXTURE_SUB_RECT);
    /// @todo better way to set center.
    sprite_resource->setOrigin(8.0f, 8.0f);
    std::shared_ptr<GRAPHICS::Sprite> sprite = std::make_shared<GRAPHICS::Sprite>(sprite_resource);
    GRAPHICS::AnimatedSprite animated_sprite(sprite);

    // SET ANIMATION SEQUENCES.
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID, walk_front_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID, walk_back_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID, walk_left_animation);
    animated_sprite.AddAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID, walk_right_animation);

    // Set the initial animation sequence to have the player facing forward (downward).
    animated_sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);

    // SET NOAH'S INITIAL POSITION.
    animated_sprite.SetWorldPosition(initial_world_position.X, initial_world_position.Y);

    // INITIALIZE NOAH.
    noah_player.Sprite = animated_sprite;
    noah_player.FacingDirection = CORE::Direction::DOWN;

    // LOAD THE AXE FOR NOAH.
    std::shared_ptr<GRAPHICS::Texture> axe_texture = assets.GetTexture(RESOURCES::AXE_TEXTURE_ID);
    bool axe_resources_loaded = (nullptr != axe_texture);
    if (!axe_resources_loaded)
    {
        // The axe cannot be created without its resources.
        assert(axe_resources_loaded);
        return;
    }

    const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
    const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
    const float AXE_WIDTH_IN_PIXELS = 11.0f;
    const float AXE_HEIGHT_IN_PIXELS = 14.0f;
    MATH::FloatRectangle axe_texture_sub_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
        AXE_SPRITE_X_OFFSET_IN_PIXELS,
        AXE_SPRITE_Y_OFFSET_IN_PIXELS,
        AXE_WIDTH_IN_PIXELS,
        AXE_HEIGHT_IN_PIXELS);
    std::shared_ptr<GRAPHICS::Sprite> axe_sprite = std::make_shared<GRAPHICS::Sprite>(
        *axe_texture,
        axe_texture_sub_rectangle);
    noah_player.Axe = std::make_shared<OBJECTS::Axe>(axe_sprite);
}

void Render(GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target)
{
    sprite.Render(render_target);
}

void Render(GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target)
{
    sprite.Render(render_target);
}

void Render(MAPS::TileMap& tile_map, sf::RenderTarget& render_target)
{
    // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
    MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
    for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
    {
        for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
        {
            std::shared_ptr<MAPS::Tile> tile = tile_map.Ground.Tiles(tile_column, tile_row);
            Render(tile->Sprite, render_target);
        }
    }

    // RENDER THE CURRENT TILE MAP'S TREES.
    for (auto& tree : tile_map.Trees)
    {
        Render(tree.Sprite, render_target);
    }
}


bool CollidesWithTree(const MAPS::Overworld& overworld, const MATH::FloatRectangle& rectangle, MATH::FloatRectangle& tree_rectangle)
{
    // CLEAR THE OUT PARAMETER.
    tree_rectangle = MATH::FloatRectangle();

    // GET THE TREES NEAR THE RECTANGLE.
    MATH::Vector2f object_center_position = rectangle.GetCenterPosition();
    std::shared_ptr<MAPS::TileMap> current_tile_map = overworld.GetTileMap(object_center_position.X, object_center_position.Y);
    bool tile_map_exists = (nullptr != current_tile_map);
    if (!tile_map_exists)
    {
        // No tile map exists with trees to collide with.
        return false;
    }

    // CHECK IF ANY OF THE TREES COLLIDE WITH THE RECTANGLE.
    for (auto tree = current_tile_map->Trees.cbegin(); current_tile_map->Trees.cend() != tree; ++tree)
    {
        // SHRINK THE TREE'S BOUNDING BOX SO THAT OBJECTS DON'T GET CAUGHT ON EDGES.
        const float TREE_DIMENSION_SHRINK_AMOUNT = 2.0f;
        MATH::FloatRectangle tree_bounds = tree->GetBoundingBox();
        float new_tree_width = tree_bounds.GetWidth() - TREE_DIMENSION_SHRINK_AMOUNT;
        float new_tree_height = tree_bounds.GetHeight() - TREE_DIMENSION_SHRINK_AMOUNT;
        MATH::FloatRectangle new_tree_bounds = MATH::FloatRectangle::FromCenterAndDimensions(
            tree_bounds.GetCenterXPosition(),
            tree_bounds.GetCenterYPosition(),
            new_tree_width,
            new_tree_height);

        // CHECK IF A COLLISION OCCURS WITH THE CURRENT TREE.
        bool collides_with_tree = rectangle.Intersects(new_tree_bounds);
        if (collides_with_tree)
        {
            // RETURN THAT A COLLISION OCCURRED WITH THE CURRENT TREE.
            tree_rectangle = new_tree_bounds;
            return true;
        }
    }

    // No trees were found to collide with the rectangle.
    return false;
}

MATH::Vector2f MoveUpWithCollisionDetection(const MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box)
{
    // INITIALIZE THE NEW POSITION FOR THE OBJECT.
    // The object should remain at its current position if no movement occurs.
    MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

    // VERIFY THAT THE MOVEMENT IS FOR THE 'UP' DIRECTION.
    bool movement_for_up_direction = (CORE::Direction::UP == movement.Direction);
    if (!movement_for_up_direction)
    {
        // An invalid movement was provided to this method since the direction was not up.
        assert(movement_for_up_direction);
        return object_new_world_position;
    }

    // GRADUALLY MOVE THE OBJECT UPWARD UNTIL WE COLLIDE WITH SOMETHING.
    // The bounding box of the object will be modified through multiple iterations of movement
    // to calculate the final position.
    MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
    float distance_left_to_move = movement.DistanceInPixels;
    while (distance_left_to_move > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE TOP OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
        float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();

        // Get the tile for the top-left corner.
        float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();
        // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        float collision_box_width = object_current_bounding_box.GetWidth();
        float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;
        collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> top_left_tile = overworld.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

        // Get the tile right above the collision box's center.
        float collison_box_center_x_position = object_current_bounding_box.GetCenterXPosition();
        std::shared_ptr<MAPS::Tile> top_center_tile = overworld.GetTileAtWorldPosition(collison_box_center_x_position, collision_box_top_y_position);

        // Get the tile for the top-right corner.
        float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();
        collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> top_right_tile = overworld.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

        // MAKE SURE THE TOP TILES EXIST.
        bool top_tiles_exist = (nullptr != top_left_tile) && (nullptr != top_center_tile) && (nullptr != top_right_tile);
        if (!top_tiles_exist)
        {
            // There aren't any tiles above the collision box to move to.
            break;
        }

        // CHECK IF THE TOP TILES ARE WALKABLE.
        bool top_tiles_walkable = (top_left_tile->IsWalkable()) && (top_center_tile->IsWalkable()) && (top_right_tile->IsWalkable());
        if (!top_tiles_walkable)
        {
            // At least one of the top tiles isn't walkable, so it is blocking any further upward movement.
            break;
        }

        // CHECK IF THE OBJECT COLLIDES WITH A TREE.
        MATH::FloatRectangle tree_rectangle;
        bool collides_with_tree = CollidesWithTree(overworld, object_current_bounding_box, tree_rectangle);
        if (collides_with_tree)
        {
            // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
            // A collision could have occurred with a tree that isn't being moved toward,
            // in which case movement should not be stopped.
            // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
            const float MIN_UP_MOVEMENT = -1.0f;
            float old_collision_box_bottom = object_current_bounding_box.GetBottomYPosition();
            float old_collision_box_top = object_current_bounding_box.GetTopYPosition();
            float new_collision_box_top = old_collision_box_top + MIN_UP_MOVEMENT;
            float tree_bottom = tree_rectangle.GetBottomYPosition();

            // A minimum distance between the collision box's center and the tree's
            // center is enforced so that moving objects don't get stuck on
            // parts of trees not directly related to their current movement.
            float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
            float object_center_x = object_current_bounding_box.GetCenterXPosition();
            float tree_center_x = tree_rectangle.GetCenterXPosition();
            float object_to_tree_distance = fabs(object_center_x - tree_center_x);
            bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

            bool moving_up_collides_with_tree = (
                collision_distance_met &&
                (old_collision_box_bottom >= tree_bottom) &&
                (new_collision_box_top <= tree_bottom));
            if (moving_up_collides_with_tree)
            {
                // A tree is blocking further movement.
                break;
            }
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float top_tile_top_y_position = top_center_tile->GetTopYPosition();
        float distance_from_top_of_collision_box_to_top_of_tile = fabs(collision_box_top_y_position - top_tile_top_y_position);
        bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_top_of_collision_box_to_top_of_tile);
        if (remaining_movement_confined_to_current_tile)
        {
            // Move the collision box the total movement amount since it is confined to a walkable tile.
            // Upward movement is in the negative Y direction.
            object_current_bounding_box.Move(MATH::Vector2f(0.0f, -1.0f * distance_left_to_move));

            // The entire movement has completed, so exit the movement loop.
            break;
        }
        else
        {
            // Ensure that we move up at least 1 pixel to prevent an infinite loop.
            float distance_to_move_for_current_tile = std::max(distance_from_top_of_collision_box_to_top_of_tile, 1.0f);

            // Move the collision box to the top of the current top tile.
            // Upward movement is in the negative Y direction.
            object_current_bounding_box.Move(MATH::Vector2f(0.0f, -1.0f * distance_to_move_for_current_tile));

            // Track the distance we just moved.
            distance_left_to_move -= distance_to_move_for_current_tile;

            // At this point, the loop will continue another iteration.
            // New top tiles should be detected for the new position of the collision box,
            // allowing movement to be simulated for the next set of top tiles.
        }
    }

    // RETURN THE OBJECT'S NEW POSITION.
    object_new_world_position = object_current_bounding_box.GetCenterPosition();

    float original_x_position = object_world_bounding_box.GetCenterPosition().X;
    float new_x_position = object_new_world_position.X;
    assert(original_x_position == new_x_position);

    return object_new_world_position;
}

MATH::Vector2f MoveDownWithCollisionDetection(const MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box)
{
    // INITIALIZE THE NEW POSITION OF THE OBJECT.
    // The object should remain at its current position if no movement occurs.
    MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

    // VERIFY THAT THE MOVEMENT IS FOR THE 'DOWN' DIRECTION.
    bool movement_for_down_direction = (CORE::Direction::DOWN == movement.Direction);
    if (!movement_for_down_direction)
    {
        // An invalid movement was provided to this method since the direction was not down.
        assert(movement_for_down_direction);
        return object_new_world_position;
    }

    // GRADUALLY MOVE THE OBJECT DOWNWARD UNTIL WE COLLIDE WITH SOMETHING.
    // The bounding box of the object will be modified through multiple iterations of movement
    // to calculate the final position.
    MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
    float distance_left_to_move = movement.DistanceInPixels;
    while (distance_left_to_move > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE BOTTOM OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
        float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();

        // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collision_box_width = object_current_bounding_box.GetWidth();
        float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;

        // Get the tile for the bottom-left corner.
        float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();
        collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> bottom_left_tile = overworld.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

        // Get the tile right below the collision box's center.
        float collison_box_center_x_position = object_current_bounding_box.GetCenterXPosition();
        std::shared_ptr<MAPS::Tile> bottom_center_tile = overworld.GetTileAtWorldPosition(collison_box_center_x_position, collision_box_bottom_y_position);

        // Get the tile for the bottom-right corner.
        float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();
        collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> bottom_right_tile = overworld.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

        // MAKE SURE THE BOTTOM TILES EXIST.
        bool bottom_tiles_exist = (nullptr != bottom_left_tile) && (nullptr != bottom_center_tile) && (nullptr != bottom_right_tile);
        if (!bottom_tiles_exist)
        {
            // There aren't any tiles below the collision box to move to.
            break;
        }

        // CHECK IF THE BOTTOM TILES ARE WALKABLE.
        bool bottom_tiles_walkable = (bottom_left_tile->IsWalkable()) && (bottom_center_tile->IsWalkable()) && (bottom_right_tile->IsWalkable());
        if (!bottom_tiles_walkable)
        {
            // At least one of the bottom tiles isn't walkable, so it is blocking any further downward movement.
            break;
        }

        // CHECK IF THE OBJECT COLLIDES WITH A TREE.
        MATH::FloatRectangle tree_rectangle;
        bool collides_with_tree = CollidesWithTree(overworld, object_current_bounding_box, tree_rectangle);
        if (collides_with_tree)
        {
            // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
            // A collision could have occurred with a tree that isn't being moved toward,
            // in which case movement should not be stopped.
            // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
            const float MIN_DOWN_MOVEMENT = 1.0f;
            float old_collision_box_top = object_current_bounding_box.GetTopYPosition();
            float old_collision_box_bottom = object_current_bounding_box.GetBottomYPosition();
            float new_collision_box_bottom = old_collision_box_bottom + MIN_DOWN_MOVEMENT;
            float tree_top = tree_rectangle.GetTopYPosition();

            // A minimum distance between the collision box's center and the tree's
            // center is enforced so that moving objects don't get stuck on
            // parts of trees not directly related to their current movement.
            float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
            float object_center_x = object_current_bounding_box.GetCenterXPosition();
            float tree_center_x = tree_rectangle.GetCenterXPosition();
            float object_to_tree_distance = fabs(object_center_x - tree_center_x);
            bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

            bool moving_down_collides_with_tree = (
                collision_distance_met &&
                (old_collision_box_top <= tree_top) &&
                (new_collision_box_bottom >= tree_top));
            if (moving_down_collides_with_tree)
            {
                // A tree is blocking further movement.
                break;
            }
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float bottom_tile_bottom_y_position = bottom_center_tile->GetBottomYPosition();
        float distance_from_bottom_of_collision_box_to_bottom_of_tile = fabs(bottom_tile_bottom_y_position - collision_box_bottom_y_position);
        bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_bottom_of_collision_box_to_bottom_of_tile);
        if (remaining_movement_confined_to_current_tile)
        {
            // Move the collision box the total movement amount since it is confined to a walkable tile.
            // Downward movement is in the positive Y direction.
            object_current_bounding_box.Move(MATH::Vector2f(0.0f, distance_left_to_move));

            // The entire movement has completed, so exit the movement loop.
            break;
        }
        else
        {
            // Ensure that we move down at least 1 pixel to prevent an infinite loop.
            float distance_to_move_for_current_tile = std::max(distance_from_bottom_of_collision_box_to_bottom_of_tile, 1.0f);

            // Move the collision box to the bottom of the current bottom tile.
            // Downward movement is in the positive Y direction.
            object_current_bounding_box.Move(MATH::Vector2f(0.0f, distance_to_move_for_current_tile));

            // Track the distance we just moved.
            distance_left_to_move -= distance_to_move_for_current_tile;

            // At this point, the loop will continue another iteration.
            // New bottom tiles should be detected for the new position of the collision box,
            // allowing movement to be simulated for the next set of bottom tiles.
        }
    }

    // RETURN THE OBJECT'S NEW POSITION.
    object_new_world_position = object_current_bounding_box.GetCenterPosition();
    return object_new_world_position;
}

MATH::Vector2f MoveLeftWithCollisionDetection(const MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box)
{
    // INITIALIZE THE NEW POSITION FOR THE OBJECT.
    // The object should remain at its current position if no movement occurs.
    MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

    // VERIFY THAT THE MOVEMENT IS FOR THE 'LEFT' DIRECTION.
    bool movement_for_left_direction = (CORE::Direction::LEFT == movement.Direction);
    if (!movement_for_left_direction)
    {
        // An invalid movement was provided to this method since the direction was not left.
        assert(movement_for_left_direction);
        return object_new_world_position;
    }

    // GRADUALLY MOVE THE OBJECT LEFTWARD UNTIL WE COLLIDE WITH SOMETHING.
    // The bounding box of the object will be modified through multiple iterations of movement
    // to calculate the final position.
    MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
    float distance_left_to_move = movement.DistanceInPixels;
    while (distance_left_to_move > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE LEFT OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
        float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();

        // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collision_box_height = object_current_bounding_box.GetHeight();
        float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

        // Get the tile for the top-left corner.
        float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();
        collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> top_left_tile = overworld.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

        // Get the tile to the direct left of the collision box's center.
        float collision_box_center_y_position = object_current_bounding_box.GetCenterYPosition();
        std::shared_ptr<MAPS::Tile> center_left_tile = overworld.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_center_y_position);

        // Get the tile for the bottom-left corner.
        float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();
        collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> bottom_left_tile = overworld.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

        // MAKE SURE THE LEFT TILES EXIST.
        bool left_tiles_exist = (nullptr != top_left_tile) && (nullptr != center_left_tile) && (nullptr != bottom_left_tile);
        if (!left_tiles_exist)
        {
            // There aren't any tiles left of the collision box to move to.
            break;
        }

        // CHECK IF THE LEFT TILES ARE WALKABLE.
        bool left_tiles_walkable = (top_left_tile->IsWalkable()) && (center_left_tile->IsWalkable()) && (bottom_left_tile->IsWalkable());
        if (!left_tiles_walkable)
        {
            // At least one of the left tiles isn't walkable, so it is blocking any further leftward movement.
            break;
        }

        // CHECK IF THE OBJECT COLLIDES WITH A TREE.
        MATH::FloatRectangle tree_rectangle;
        bool collides_with_tree = CollidesWithTree(overworld, object_current_bounding_box, tree_rectangle);
        if (collides_with_tree)
        {
            // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
            // A collision could have occurred with a tree that isn't being moved toward,
            // in which case movement should not be stopped.
            // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
            const float MIN_LEFT_MOVEMENT = -1.0f;
            float old_collision_box_right = object_current_bounding_box.GetRightXPosition();
            float old_collision_box_left = object_current_bounding_box.GetLeftXPosition();
            float new_collision_box_left = old_collision_box_left + MIN_LEFT_MOVEMENT;
            float tree_right = tree_rectangle.GetRightXPosition();

            // A minimum distance between the collision box's center and the tree's
            // center is enforced so that moving objects don't get stuck on
            // parts of trees not directly related to their current movement.
            float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
            float object_center_y = object_current_bounding_box.GetCenterYPosition();
            float tree_center_y = tree_rectangle.GetCenterYPosition();
            float object_to_tree_distance = fabs(object_center_y - tree_center_y);
            bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

            bool moving_left_collides_with_tree = (
                collision_distance_met &&
                (old_collision_box_right >= tree_right) &&
                (new_collision_box_left <= tree_right));
            if (moving_left_collides_with_tree)
            {
                // A tree is blocking further movement.
                break;
            }
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float left_tile_left_x_position = center_left_tile->GetLeftXPosition();
        float distance_from_left_of_collision_box_to_left_of_tile = fabs(collision_box_left_x_position - left_tile_left_x_position);
        bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_left_of_collision_box_to_left_of_tile);
        if (remaining_movement_confined_to_current_tile)
        {
            // Move the collision box the total movement amount since it is confined to a walkable tile.
            object_current_bounding_box.Move(MATH::Vector2f(-1.0f * distance_left_to_move, 0.0f));

            // The entire movement has completed, so exit the movement lop.
            break;
        }
        else
        {
            // Ensure that we move left at least 1 pixel to prevent an infinite loop.
            float distance_to_move_for_current_tile = std::max(distance_from_left_of_collision_box_to_left_of_tile, 1.0f);

            // Move the collision box to the left of the current left tile.
            object_current_bounding_box.Move(MATH::Vector2f(-1.0f * distance_to_move_for_current_tile, 0.0f));

            // Track the distance we just moved.
            distance_left_to_move -= distance_to_move_for_current_tile;

            // At this point, the loop will continue another iteration.
            // New left tiles should be detected for the new position of the collision box,
            // allowing movement to be simulated for the next set of left tiles.
        }
    }

    // RETURN THE OBJECT'S NEW POSITION.
    object_new_world_position = object_current_bounding_box.GetCenterPosition();
    return object_new_world_position;
}

MATH::Vector2f MoveRightWithCollisionDetection(const MAPS::Overworld& overworld, const COLLISION::Movement& movement, const MATH::FloatRectangle& object_world_bounding_box)
{
    // INITIALIZE THE NEW POSITION FOR THE OBJECT.
    // The object should remain at its current position if no movement occurs.
    MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

    // VERIFY THAT THE MOVEMENT IS FOR THE 'RIGHT' DIRECTION.
    bool movement_for_right_direction = (CORE::Direction::RIGHT == movement.Direction);
    if (!movement_for_right_direction)
    {
        // An invalid movement was provided to this method since the direction was not right.
        assert(movement_for_right_direction);
        return object_new_world_position;
    }

    // GRADUALLY MOVE THE OBJECT RIGHTWARD UNTIL WE COLLIDE WITH SOMETHING.
    // The bounding box of the object will be modified through multiple iterations of movement
    // to calculate the final position.
    MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
    float distance_left_to_move = movement.DistanceInPixels;
    while (distance_left_to_move > 0.0f)
    {
        // GET THE CURRENT TILES THAT THE RIGHT OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
        float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();

        // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
        // We will adjust the corners to move them closer to the center of the collision box.
        float collision_box_height = object_current_bounding_box.GetHeight();
        float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

        // Get the tile for the top-right corner.
        float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();
        collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> top_right_tile = overworld.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

        // Get the tile to the direct right of the collision box's center.
        float collision_box_center_y_position = object_current_bounding_box.GetCenterYPosition();
        std::shared_ptr<MAPS::Tile> center_right_tile = overworld.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_center_y_position);

        // Get the tile for the bottom-right corner.
        float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();
        collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
        std::shared_ptr<MAPS::Tile> bottom_right_tile = overworld.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

        // MAKE SURE THE RIGHT TILES EXIST.
        bool right_tiles_exist = (nullptr != top_right_tile) && (nullptr != center_right_tile) && (nullptr != bottom_right_tile);
        if (!right_tiles_exist)
        {
            // There aren't any tiles right of the collision box to move to.
            break;
        }

        // CHECK IF THE RIGHT TILES ARE WALKABLE.
        bool right_tiles_walkable = (top_right_tile->IsWalkable()) && (center_right_tile->IsWalkable()) && (bottom_right_tile->IsWalkable());
        if (!right_tiles_walkable)
        {
            // At least one of the right tiles isn't walkable, so it is blocking any further rightward movement.
            break;
        }

        // CHECK IF THE OBJECT COLLIDES WITH A TREE.
        MATH::FloatRectangle tree_rectangle;
        bool collides_with_tree = CollidesWithTree(overworld, object_current_bounding_box, tree_rectangle);
        if (collides_with_tree)
        {
            // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
            // A collision could have occurred with a tree that isn't being moved toward,
            // in which case movement should not be stopped.
            // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
            const float MIN_RIGHT_MOVEMENT = 1.0f;
            float old_collision_box_left = object_current_bounding_box.GetLeftXPosition();
            float old_collision_box_right = object_current_bounding_box.GetRightXPosition();
            float new_collision_box_right = old_collision_box_right + MIN_RIGHT_MOVEMENT;
            float tree_left = tree_rectangle.GetLeftXPosition();

            // A minimum distance between the collision box's center and the tree's
            // center is enforced so that moving objects don't get stuck on
            // parts of trees not directly related to their current movement.
            float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
            float object_center_y = object_current_bounding_box.GetCenterYPosition();
            float tree_center_y = tree_rectangle.GetCenterYPosition();
            float object_to_tree_distance = fabs(object_center_y - tree_center_y);
            bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

            bool moving_right_collides_with_tree = (
                collision_distance_met &&
                (old_collision_box_left <= tree_left) &&
                (new_collision_box_right >= tree_left));
            if (moving_right_collides_with_tree)
            {
                // A tree is blocking further movement.
                break;
            }
        }

        // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
        float right_tile_right_x_position = center_right_tile->GetRightXPosition();
        float distance_from_right_of_collision_box_to_right_of_tile = fabs(right_tile_right_x_position - collision_box_right_x_position);
        bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_right_of_collision_box_to_right_of_tile);
        if (remaining_movement_confined_to_current_tile)
        {
            // Move the collision box the total movement amount since it is confined
            // to a walkable tile.
            object_current_bounding_box.Move(MATH::Vector2f(distance_left_to_move, 0.0f));

            // The entire movement has completed, so exit the movement loop.
            break;
        }
        else
        {
            // Ensure that we move right at least 1 pixel to prevent an infinite loop.
            float distance_to_move_for_current_tile = std::max(distance_from_right_of_collision_box_to_right_of_tile, 1.0f);

            // Move the collision box to the right of the current right tile.
            object_current_bounding_box.Move(MATH::Vector2f(distance_from_right_of_collision_box_to_right_of_tile, 0.0f));

            // Track the distance we just moved.
            distance_left_to_move -= distance_to_move_for_current_tile;

            // At this point, the loop will continue another iteration.
            // New right tiles should be detected for the new position of the collision box,
            // allowing movement to be simulated for the next set of right tiles.
        }
    }

    // RETURN THE OBJECT'S NEW POSITION.
    object_new_world_position = object_current_bounding_box.GetCenterPosition();
    return object_new_world_position;
}

MATH::Vector2f MoveWithCollisionDetection(
    const MAPS::Overworld& overworld, 
    const float elapsed_time_in_seconds, 
    const CORE::Direction direction, 
    const float move_speed_in_pixels_per_second, 
    const MATH::FloatRectangle& object_world_bounding_box)
{
    float movement_distance_in_pixels = move_speed_in_pixels_per_second * elapsed_time_in_seconds;
    COLLISION::Movement movement(direction, movement_distance_in_pixels);

    // SIMULATE MOVEMENT BASED ON THE PARTICULAR DIRECTION.
    MATH::Vector2f new_world_position = object_world_bounding_box.GetCenterPosition();
    //std::cout << "Original position: (" << new_world_position.X << "," << new_world_position.Y << ")" << std::endl;
    switch (direction)
    {
        case CORE::Direction::UP:
            new_world_position = MoveUpWithCollisionDetection(overworld, movement, object_world_bounding_box);
            break;
        case CORE::Direction::DOWN:
            new_world_position = MoveDownWithCollisionDetection(overworld, movement, object_world_bounding_box);
            break;
        case CORE::Direction::LEFT:
            new_world_position = MoveLeftWithCollisionDetection(overworld, movement, object_world_bounding_box);
            break;
        case CORE::Direction::RIGHT:
            new_world_position = MoveRightWithCollisionDetection(overworld, movement, object_world_bounding_box);
            break;
        case CORE::Direction::INVALID:
            // Intentionally fall through.
        default:
            // No movement can be simulated for an invalid direction.
            break;
    }
    //std::cout << "New position: (" << new_world_position.X << "," << new_world_position.Y << ")" << std::endl;

    return new_world_position;
}

void HandleAxeCollisionsWithTrees(const MATH::FloatRectangle& axe_blade_bounds, MAPS::Overworld& overworld)
{
    // GET THE WORLD AREA CONTAING THE AXE BLADE.
    // While it is technically possible for the axe to intersect multiple world areas,
    // the axe is relatively small, and the player shouldn't be able to see more than
    // one world area.  Therefore, it should be safe to get just the area for the
    // axe blade's center.
    /// @todo   Maybe the world should just encapsulate the above?
    /// @todo   Change overworld map to world area?
    //WorldArea world_area = OverworldMap->GetArea(axe_blade_bounds.GetCenterXPosition(), axe_blade_bounds.GetCenterYPosition());
    MATH::Vector2f axe_center_position = axe_blade_bounds.GetCenterPosition();
    std::shared_ptr<MAPS::TileMap> tile_map = overworld.GetTileMap(axe_center_position.X, axe_center_position.Y);
    assert(tile_map);
    for (auto tree = tile_map->Trees.cbegin(); tile_map->Trees.cend() != tree;)
    {
        MATH::FloatRectangle tree_bounds = tree->GetBoundingBox();
        bool axe_hit_tree = axe_blade_bounds.Intersects(tree_bounds);
        if (axe_hit_tree)
        {
            tree = tile_map->Trees.erase(tree);
        }
        else
        {
            ++tree;
        }
    }
}

void HandleAxeSwingCollisions(MAPS::Overworld& overworld, std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings)
{
    // HANDLE COLLISIONS FOR ALL AXE SWINGS.
    for (auto axe_swing_event = axe_swings.cbegin(); axe_swings.cend() != axe_swing_event;)
    {
        // MAKE SURE THE CURRENT AXE SWING EXISTS.
        bool axe_swing_exists = (nullptr != axe_swing_event->get());
        if (!axe_swing_exists)
        {
            // Remove the invalid event so that it no longer takes up space.
            axe_swing_event = axe_swings.erase(axe_swing_event);

            // Silently skip over this missing axe swing to allow
            // the game to continue running.
            continue;
        }

        const EVENTS::AxeSwingEvent axe_swing = (**axe_swing_event);

        // CHECK IF THE AXE SWING EVENT HAS REACHED ITS MAXIMUM EXTENSION POINT.
        // The axe swing event should only be processed once it has finish
        // being fully swung out to its maximum point.
        bool axe_swing_at_max_extension_point = axe_swing.FullySwungOut();
        if (!axe_swing_at_max_extension_point)
        {
            // The current axe swing event is not ready to be processed,
            // so skip to the next event.
            ++axe_swing_event;
            continue;
        }

        // HANDLE COLLISIONS OF THE AXE WITH TREES.
        MATH::FloatRectangle axe_blade_bounds = axe_swing.GetBladeBounds();
        HandleAxeCollisionsWithTrees(axe_blade_bounds, overworld);

        // REMOVE THE PROCESSED AXE SWING EVENT.
        axe_swing_event = axe_swings.erase(axe_swing_event);
    }
}

/// The main entry point for the game.
/// Runs the Noah's Ark game until the user
/// chooses to exit or an error occurs.
/// @param[in]  argumentCount - The number of command line arguments.
/// @param[in]  arguments - The command line arguments.
/// @return     One of the exit codes defined in NoahArkGame.
int main(int argumentCount, char* arguments[])
{
    try
    {
        // LOAD ALL GAME ASSETS.
        // This will prevent slowdowns from having to wait on disk I/O later
        // for reading assets in the middle of gameplay.
        RESOURCES::Assets assets;
        bool assets_loaded = assets.LoadAll();
        if (!assets_loaded)
        {
            /// @todo   Provide a more specific error from Assets class for loading failures?
            std::cerr << "Failed to load assets." << std::endl;
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        // CREATE THE OVERWORLD.
        const MAPS::OverworldMapFile* const overworld_map_file = assets.GetOverworldMapFile();
        MAPS::Overworld overworld(overworld_map_file->OverworldWidthInTileMaps, overworld_map_file->OverworldHeightInTileMaps);
        PopulateOverworld(*overworld_map_file, assets, overworld);

        // INITIALIZE THE PLAYER NOAH CHARACTER.
        /// @todo   Need to figure out a different way to determine initial position.
        std::shared_ptr<MAPS::TileMap> starting_tile_map = overworld.TileMaps(0, 0);
        MATH::Vector2f noah_initial_position = starting_tile_map->GetCenterWorldPosition();
        OBJECTS::Noah noah_player;
        InitializePlayer(noah_initial_position, assets, noah_player);

        // CREATE THE CONTROLLER USING THE KEYBOARD.
        INPUT_CONTROL::KeyboardInputController input_controller;

        // CREATE THE WINDOW.
        // The width and height are currently set to match the dimensions
        // of a single tile map in the game.
        const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
        const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        sf::RenderWindow window(
            sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // INITIALIZE THE CAMERA.
        auto view = window.getView();
        MATH::Vector2f center_world_position = starting_tile_map->GetCenterWorldPosition();
        view.setCenter(center_world_position.X, center_world_position.Y);
        window.setView(view);

        // CREATE A COLLECTION OF AXE SWING EVENTS.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > axe_swings;

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        bool debug_on = false;
        int debug_count = 0;
        bool scrolling = false;
        float current_total_scroll_time_in_seconds = 0.0f;
        MATH::Vector2f scroll_start_position;
        MATH::Vector2f scroll_end_position;
        sf::Clock game_loop_clock;
        while (window.isOpen())
        {
            auto print_position = noah_player.GetWorldPosition();
            if (debug_on)
            {
                std::cout << print_position.X << ", " << print_position.Y << std::endl;
                ++debug_count;
                if (debug_count > 3)
                {
                    debug_count = 0;
                    debug_on = false;
                    std::cout << std::endl;
                }
            }

            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window.pollEvent(event))
            {
                // Handle the current event based on its type.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window.isOpen())
            {
                // Update the game for the new frame.
                sf::Time elapsed_time = game_loop_clock.restart();
                float elapsed_time_in_seconds = elapsed_time.asSeconds();
                /// @todo   Update here.
                view = window.getView();
                sf::Vector2f view_center = view.getCenter();
                sf::Vector2f view_size = view.getSize();
                MATH::FloatRectangle camera_bounds = MATH::FloatRectangle::FromCenterAndDimensions(
                    view_center.x,
                    view_center.y,
                    view_size.x,
                    view_size.y);
                std::shared_ptr<MAPS::TileMap> current_tile_map = overworld.GetTileMap(view_center.x, view_center.y);
                assert(current_tile_map);

                // MOVE NOAH IN RESPONSE TO USER INPUT.
                // SWING THE PLAYER'S AXE IF THE APPROPRIATE BUTTON WAS PRESSED.
                if (input_controller.PrimaryActionButtonPressed())
                {
                    // A new axe swing may not be created if the player's
                    // axe is already being swung.
                    std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = noah_player.SwingAxe();
                    bool axe_swing_occurred = (nullptr != axe_swing);
                    if (axe_swing_occurred)
                    {
                        // Allow the axe to collide with other objects.
                        axe_swings.push_back(axe_swing);
                    }
                }

                bool noah_moved_this_frame = false;
                const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
                MATH::Vector2f old_noah_position = noah_player.GetWorldPosition();
                if (input_controller.UpButtonPressed())
                {
                    // CHECK IF THE AXE IS SWINGING.
                    // Noah can't move while the axe is swinging.
                    // Movement is prevented to have the axe's position
                    // remain attached to Noah.
                    bool axe_is_swinging = (nullptr != noah_player.Axe) && noah_player.Axe->IsSwinging();
                    if (!axe_is_swinging)
                    {
                        noah_moved_this_frame = true;
                        noah_player.FacingDirection = CORE::Direction::UP;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_BACK_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        //noah_player.MoveUp(elapsed_time_in_seconds);
                        MATH::Vector2f new_position = MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::UP,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_top_y_position = camera_bounds.GetTopYPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetTopYPosition() < camera_top_y_position);
                        if (player_moved_out_of_view)
                        {
                            unsigned int top_tile_map_row_index = current_tile_map->OverworldRowIndex - 1;
                            unsigned int top_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                            std::shared_ptr<MAPS::TileMap> top_tile_map = overworld.GetTileMap(
                                top_tile_map_row_index,
                                top_tile_map_column_index);
                            bool top_tile_map_exists = (nullptr != top_tile_map);
                            if (top_tile_map_exists)
                            {
                                // Move Noah a few more pixels up so that he will be more visibile on the new map.
                                noah_world_position.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                scroll_end_position = top_tile_map->GetCenterWorldPosition();
                                current_total_scroll_time_in_seconds = 0.0f;
                                scrolling = true;

                                input_controller.DisableInput();
                            }
                            else
                            {
                                /// @todo Better handling of edges.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_top_boundary = tile_map_bounding_box.GetTopYPosition();
                                float noah_half_height = noah_player.GetWorldBoundingBox().GetHeight() / 2.0f;

                                noah_world_position = old_noah_position;
                                noah_world_position.Y = tile_map_top_boundary + noah_half_height;
                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        }
                    }
                }
                if (input_controller.DownButtonPressed())
                {
                    // CHECK IF THE AXE IS SWINGING.
                    // Noah can't move while the axe is swinging.
                    // Movement is prevented to have the axe's position
                    // remain attached to Noah.
                    bool axe_is_swinging = (nullptr != noah_player.Axe) && noah_player.Axe->IsSwinging();
                    if (!axe_is_swinging)
                    {
                        noah_moved_this_frame = true;
                        noah_player.FacingDirection = CORE::Direction::DOWN;

                        // PLAY THE WALKING DOWN ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_FRONT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        //noah_player.MoveDown(elapsed_time_in_seconds);
                        MATH::Vector2f new_position = MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::DOWN,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_bottom_y_position = camera_bounds.GetBottomYPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetBottomYPosition() > camera_bottom_y_position);
                        if (player_moved_out_of_view)
                        {
                            unsigned int bottom_tile_map_row_index = current_tile_map->OverworldRowIndex + 1;
                            unsigned int bottom_tile_map_column_index = current_tile_map->OverworldColumnIndex;
                            std::shared_ptr<MAPS::TileMap> bottom_tile_map = overworld.GetTileMap(
                                bottom_tile_map_row_index,
                                bottom_tile_map_column_index);
                            bool bottom_tile_map_exists = (nullptr != bottom_tile_map);
                            if (bottom_tile_map_exists)
                            {
                                // Move Noah a few more pixels down so that he will be more visibile on the new map.
                                noah_world_position.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                scroll_end_position = bottom_tile_map->GetCenterWorldPosition();
                                current_total_scroll_time_in_seconds = 0.0f;
                                scrolling = true;

                                input_controller.DisableInput();
                            }
                            else
                            {
                                /// @todo Better handling of edges.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_bottom_boundary = tile_map_bounding_box.GetBottomYPosition();
                                float noah_half_height = noah_player.GetWorldBoundingBox().GetHeight() / 2.0f;

                                noah_world_position = old_noah_position;
                                noah_world_position.Y = tile_map_bottom_boundary - noah_half_height;
                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        }
                    }
                }
                if (input_controller.LeftButtonPressed())
                {
                    // CHECK IF THE AXE IS SWINGING.
                    // Noah can't move while the axe is swinging.
                    // Movement is prevented to have the axe's position
                    // remain attached to Noah.
                    bool axe_is_swinging = (nullptr != noah_player.Axe) && noah_player.Axe->IsSwinging();
                    if (!axe_is_swinging)
                    {
                        noah_moved_this_frame = true;
                        noah_player.FacingDirection = CORE::Direction::LEFT;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_LEFT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        //noah_player.MoveLeft(elapsed_time_in_seconds);
                        MATH::Vector2f new_position = MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::LEFT,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);

                        MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_left_x_position = camera_bounds.GetLeftXPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetLeftXPosition() < camera_left_x_position);
                        if (player_moved_out_of_view)
                        {
                            unsigned int left_tile_map_row_index = current_tile_map->OverworldRowIndex;
                            unsigned int left_tile_map_column_index = current_tile_map->OverworldColumnIndex - 1;
                            std::shared_ptr<MAPS::TileMap> left_tile_map = overworld.GetTileMap(
                                left_tile_map_row_index,
                                left_tile_map_column_index);
                            bool left_tile_map_exists = (nullptr != left_tile_map);
                            if (left_tile_map_exists)
                            {
                                // Move Noah a few more pixels left so that he will be more visibile on the new map.
                                noah_world_position.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                scroll_end_position = left_tile_map->GetCenterWorldPosition();
                                current_total_scroll_time_in_seconds = 0.0f;
                                scrolling = true;

                                input_controller.DisableInput();
                            }
                            else
                            {
                                /// @todo Better handling of edges.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_left_boundary = tile_map_bounding_box.GetLeftXPosition();
                                float noah_half_width = noah_player.GetWorldBoundingBox().GetWidth() / 2.0f;

                                noah_world_position = old_noah_position;
                                noah_world_position.X = tile_map_left_boundary + noah_half_width;
                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        }
                    }
                }
                if (input_controller.RightButtonPressed())
                {
                    // CHECK IF THE AXE IS SWINGING.
                    // Noah can't move while the axe is swinging.
                    // Movement is prevented to have the axe's position
                    // remain attached to Noah.
                    bool axe_is_swinging = (nullptr != noah_player.Axe) && noah_player.Axe->IsSwinging();
                    if (!axe_is_swinging)
                    {
                        noah_moved_this_frame = true;
                        noah_player.FacingDirection = CORE::Direction::RIGHT;

                        // PLAY THE WALKING UP ANIMATION.
                        noah_player.Sprite.UseAnimationSequence(RESOURCES::NOAH_WALK_RIGHT_ANIMATION_ID);
                        noah_player.Sprite.Play();

                        //noah_player.MoveRight(elapsed_time_in_seconds);
                        MATH::Vector2f new_position = MoveWithCollisionDetection(
                            overworld,
                            elapsed_time_in_seconds,
                            CORE::Direction::RIGHT,
                            OBJECTS::Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND,
                            noah_player.GetWorldBoundingBox());
                        noah_player.SetWorldPosition(new_position);


                        MATH::Vector2f noah_world_position = noah_player.GetWorldPosition();
                        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
                        float camera_right_x_position = camera_bounds.GetRightXPosition();
                        bool player_moved_out_of_view = (noah_world_bounding_box.GetRightXPosition() > camera_right_x_position);
                        if (player_moved_out_of_view)
                        {
                            unsigned int right_tile_map_row_index = current_tile_map->OverworldRowIndex;
                            unsigned int right_tile_map_column_index = current_tile_map->OverworldColumnIndex + 1;
                            std::shared_ptr<MAPS::TileMap> right_tile_map = overworld.GetTileMap(
                                right_tile_map_row_index,
                                right_tile_map_column_index);
                            bool right_tile_map_exists = (nullptr != right_tile_map);
                            if (right_tile_map_exists)
                            {
                                // Move Noah a few more pixels right so that he will be more visibile on the new map.
                                noah_world_position.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                                noah_player.SetWorldPosition(noah_world_position);

                                scroll_start_position = current_tile_map->GetCenterWorldPosition();
                                scroll_end_position = right_tile_map->GetCenterWorldPosition();
                                current_total_scroll_time_in_seconds = 0.0f;
                                scrolling = true;

                                input_controller.DisableInput();
                            }
                            else
                            {
                                /// @todo Better handling of edges.
                                MATH::FloatRectangle tile_map_bounding_box = current_tile_map->GetWorldBoundingBox();
                                float tile_map_right_boundary = tile_map_bounding_box.GetRightXPosition();
                                float noah_half_width = noah_player.GetWorldBoundingBox().GetWidth() / 2.0f;

                                noah_world_position = old_noah_position;
                                noah_world_position.X = tile_map_right_boundary - noah_half_width;
                                noah_player.SetWorldPosition(noah_world_position);
                            }
                        }
                    }
                }

                // STOP NOAH'S ANIMATION FROM PLAYING IF THE PLAYER DIDN'T MOVE THIS FRAME.
                if (!noah_moved_this_frame)
                {
                    noah_player.Sprite.ResetAnimation();
                }

                /// @todo   Camera.
                if (scrolling)
                {
                    const float MAX_SCROLL_TIME_IN_SECONDS = 3.0f;
                    current_total_scroll_time_in_seconds += elapsed_time_in_seconds;
                    bool scrolling_finished = (current_total_scroll_time_in_seconds >= MAX_SCROLL_TIME_IN_SECONDS);
                    if (scrolling_finished)
                    {
                        current_total_scroll_time_in_seconds = MAX_SCROLL_TIME_IN_SECONDS;
                        scrolling = false;
                        input_controller.EnableInput();
                    }

                    float scroll_percentage_so_far = current_total_scroll_time_in_seconds / MAX_SCROLL_TIME_IN_SECONDS;

                    float current_camera_center_x = scroll_start_position.X + ((scroll_end_position.X - scroll_start_position.X) * scroll_percentage_so_far);
                    float current_camera_center_y = scroll_start_position.Y + ((scroll_end_position.Y - scroll_start_position.Y) * scroll_percentage_so_far);
                    view.setCenter(current_camera_center_x, current_camera_center_y);
                    window.setView(view);
                }
                else
                {
                    MATH::Vector2f center_world_position = current_tile_map->GetCenterWorldPosition();
                    view.setCenter(center_world_position.X, center_world_position.Y);
                    window.setView(view);
                }

                // HANDLE AXE SWINGS.
                HandleAxeSwingCollisions(overworld, axe_swings);

                // Render the current state of the game.
                window.clear();
                /// @todo   Render here.
                // RENDER THE CURRENT TILE MAP.
                /// @todo   Consider signed ints.
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
                        std::shared_ptr<MAPS::TileMap> tile_map = overworld.GetTileMap(tile_map_row, tile_map_column);
                        bool tile_map_exists = (nullptr != tile_map);
                        if (!tile_map_exists)
                        {
                            // Continue trying to render other tile maps.
                            continue;
                        }

                        // RENDER THE TILE MAP.
                        Render(*tile_map, window);
                    }
                }
                
                // RENDER THE PLAYER.
                noah_player.Axe->Update(elapsed_time_in_seconds);
                noah_player.Sprite.Update(elapsed_time_in_seconds);
                Render(noah_player.Sprite, window);
                if (noah_player.Axe->IsSwinging())
                {
                    Render(*noah_player.Axe->GetSprite(), window);
                }

                window.display();
            }
        }

        // INDICATE THAT THE GAME EXITED SUCCESSFULLY.
        return EXIT_CODE_SUCCESS;
    }
    catch (std::exception& exception)
    {
        std::cerr << "Standard exception in main: " << exception.what() << std::endl;
        return EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN;
    }
    catch (...)
    {   
        std::cerr << "Unknown exception in main." << std::endl;
        return EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN;
    }
}