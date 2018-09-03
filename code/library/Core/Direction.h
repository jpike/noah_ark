#pragma once

/// Holds generic core code for the Noah's Ark game that doesn't
/// necessarily fit within a specific subsystem.  Examples would
/// be primitive and common data types.
namespace CORE
{
    /// Defines the different directions supported by the game.
    enum class Direction
    {
        INVALID, ///< An invalid direction.
        UP, ///< The up direction.
        DOWN, ///< The down direction.
        LEFT, ///< The left direction.
        RIGHT ///< The right direction.
    };
}