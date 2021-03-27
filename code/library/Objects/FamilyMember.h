#pragma once

#include "Gameplay/Direction.h"
#include "Graphics/AnimatedSprite.h"
#include "Hardware/GamingHardware.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/TileMap.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// A member of Noah's family that has to be gathered.
    class FamilyMember
    {
    public:
        // STATIC CONSTANTS.
        /// The size of a family member in a single dimension.
        static constexpr float SIZE_IN_PIXELS = 16.0f;
        /// The half size of a family member in a single dimension.
        static constexpr float HALF_SIZE_IN_PIXELS = 8.0f;
        /// How fast the family member can move in pixels per second.
        static constexpr float MOVE_SPEED_IN_PIXELS_PER_SECOND = 64.0f;
        /// The name of the animation for walking walking front/down.
        static inline const std::string WALK_FRONT_ANIMATION_NAME = "family_member_walk_front";
        /// The name of the animation for walking walking back/up.
        static inline const std::string WALK_BACK_ANIMATION_NAME = "family_member_walk_back";
        /// The name of the animation for walking walking left.
        static inline const std::string WALK_LEFT_ANIMATION_NAME = "family_member_walk_left";
        /// The name of the animation for walking walking right.
        static inline const std::string WALK_RIGHT_ANIMATION_NAME = "family_member_walk_right";

        // ENUMERATIONS.
        /// The different types of Noah's family members to be gathered.
        /// An enum is wrapped in a class to provid named scoping
        /// while also allowing implicit conversion to integers.
        enum Type
        {
            NOAH_WIFE,
            SHEM,
            SHEM_WIFE,
            HAM,
            HAM_WIFE,
            JAPHETH,
            JAPHETH_WIFE,
            /// An enum value is reserved to make it easier to determine the number of types.
            COUNT
        };

        // CONSTRUCTION.
        explicit FamilyMember(const Type type, const MATH::Vector2f& world_position, const MAPS::MultiTileMapGrid* const map_grid);

        // OTHER METHODS.
        void BeginWalking(const GAMEPLAY::Direction direction, const std::string& walking_animation_name);
        void MoveWithin(const MAPS::TileMap& tile_map, HARDWARE::GamingHardware& gaming_hardware);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The type of the family member.
        Type Identity = Type::COUNT;
        /// The direction the family member is currently facing.
        GAMEPLAY::Direction FacingDirection = GAMEPLAY::Direction::INVALID;
        /// The sprite used for rendering the family member.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The map grid the family member is currently in.
        /// Important for avoiding rendering family members on different map grids (i.e. layers of the ark).
        const MAPS::MultiTileMapGrid* MapGrid = nullptr;
    };
}
