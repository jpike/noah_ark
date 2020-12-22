#pragma once

#include <vector>
#include "Maps/Ark.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/Overworld.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"
#include "Objects/ArkPiece.h"
#include "Objects/FamilyMember.h"
#include "Objects/Noah.h"

namespace MAPS
{
    /// The entire game world.
    class World
    {
    public:
        // CONSTRUCTION/INITIALIZATION.
        static MEMORY::NonNullSharedPointer<World> CreateInitial();
        explicit World();
        void InitializeBuiltArkInOverworld(const std::vector<OBJECTS::ArkPiece>& built_ark_pieces);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The overworld of the game.
        Overworld Overworld;
        /// The ark.
        Ark Ark;
        /// Noah (the player) character within the game.
        /// Stored as a shared pointer because it needs to be shared with the saved game data.
        MEMORY::NonNullSharedPointer<OBJECTS::Noah> NoahPlayer;
        /// The family members.
        /// They are stored in the world, rather than separately in the overworld or ark,
        /// to make them easier to access regardless of game state.
        std::vector<OBJECTS::FamilyMember> FamilyMembers;
    };
}
