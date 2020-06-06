#pragma once

/// Holds generic code for the Noah's Ark game  gameplay that doesn't
/// necessarily fit within a specific subsystem.  Examples would
/// be primitive and common data types.
namespace GAMEPLAY
{
    /// Defines the different directions supported by the game.
    enum class Direction
    {
        /// An invalid direction.
        INVALID,
        /// The up direction.
        UP, 
        /// The down direction.
        DOWN, 
        /// The left direction.
        LEFT, 
        /// The right direction.
        RIGHT 
    };
}