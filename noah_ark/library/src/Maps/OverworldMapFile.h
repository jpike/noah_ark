#pragma once

#include <memory>
#include <string>
#include "Core/Array2D.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A file specifying the layout of the the overworld map.
    /// Specifically, it identifies individual tile maps and where
    /// they are located in the overworld.  The file is a JSON file
    /// in the following format:
    /// @code
    /// {
    ///     "WidthInTilesMaps": <Unsigned Integer>,
    ///     "HeightInTileMaps": <Unsigned Integer>,
    ///     "TileMapWidthInTiles": <Unsigned Integer>,
    ///     "TileMapHeightInTiles": <Unsigned Integer>,
    ///     "TileDimensionInPixels": <Unsigned Integer>,
    ///     "StartingTileMapFilepath": <Filepath>,
    ///     "TileMaps": [
    ///         { "Filepath": <Filepath> },
    ///         { "Filepath": <Filepath> }
    ///         ... (repeat for each tile map)
    ///     ]
    /// }
    ///
    /// Example:
    /// {
    ///     "WidthInTilesMaps": 7,
    ///     "HeightInTileMaps": 7,
    ///     "TileMapWidthInTiles": 32,
    ///     "TileMapHeightInTiles": 24,
    ///     "TileDimensionInPixels": 16,
    ///     "StartingTileMapFilepath": "res/maps/starting_map.json",
    ///     "TileMaps": [
    ///         { "Filepath": "res/maps/starting_map.json" },
    ///         { "Filepath": "res/maps/map_2.json" }
    ///     ]
    /// }
    /// @endcode
    ///
    /// @note    This class uses boost's property tree library,
    /// which isn't thread-safe by default.
    /// This code expects BOOST_SPIRIT_THREADSAFE to be defined for
    /// this class to be used safely across multiple threads.
    class OverworldMapFile
    {
    public:
        // CONSTRUCTION.
        static std::unique_ptr<OverworldMapFile> Load(const std::string& filepath);

        // DATA RETRIEVAL.
        std::string GetTileMapFilepath(const unsigned int row, const unsigned int column) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The width of the overworld (in units of tile maps).
        /// This is the number of tile maps in a single row of the overworld.
        /// Width is 1-based.
        unsigned int OverworldWidthInTileMaps = 0;
        /// The height of the overworld (in units of tile maps).
        /// This is the number of tile maps in a single column in the overworld.
        /// Height is 1-based.
        unsigned int OverworldHeightInTileMaps = 0;
        /// The width of a single tile map in the overworld (in units of tiles).
        unsigned int TileMapWidthInTiles = 0;
        /// The height of a single tile map in the overworld (in units of tiles).
        unsigned int TileMapHeightInTiles = 0;
        /// The dimension (both width and height) of a single tile in the overworld
        /// (in units of pixels).
        unsigned int TileDimensionInPixels = 0;

    private:
        // MEMBER VARIABLES.
        /// The filepath to the overworld's starting tile map.
        std::string StartingTileMapFilepath = "";
        /// The 2D position of the starting tile map.
        MATH::Vector2ui StartingTileMapPosition = MATH::Vector2ui();
        /// Tile map filepaths within the overworld map, stored by their 2D positions relative to each other.
        CORE::Array2D<std::string> TileMapFilepaths = CORE::Array2D<std::string>();
    };
}