#pragma once

#include <string>
#include "Core/Array2D.h"
#include "Math/Vector2.h"

namespace MAPS
{
    /// A specification of the overworld map.  The specification
    /// describes the individual tile maps that comprise the overworld.
    /// It can be loaded from a properly formatted XML document.
    class OverworldMapSpecification
    {
    public:
        /// Constructor.
        explicit OverworldMapSpecification();
        /// Destructor.
        ~OverworldMapSpecification();

        /// Loads the specification from an XML file.
        /// @param[in]  filepath - The path to the XML file to load.
        /// @return True if loaded successfully; false otherwise.
        bool LoadFromFile(const std::string& filepath);

        /// Gets the number of tile maps in a single row of the overworld.
        /// Width is 1-based.
        /// @return The width of the overworld in units of tile maps.
        unsigned int GetOverworldWidthInTileMaps() const;

        /// Gets the number of tile maps in a single column of the overworld.
        /// Height is 1-based.
        /// @return The height of the overworld in units of tile maps.
        unsigned int GetOverworldHeightInTileMaps() const;

        /// Gets the filepath to the starting tile map of the overworld.
        /// @return The filepath to the overworld's starting tile map.
        std::string GetStartingTileMapFilepath() const;

        /// Gets the 2D x, y (column, row) position of the starting tile map
        /// in the overworld map.
        /// @return The 2D position of the starting tile map.
        MATH::Vector2ui GetStartingTileMapPosition() const;

        /// Checks if the provided position is within a valid range for tile maps
        /// in the overworld map.
        /// @param[in]  row - The row to check (in units of tile maps).
        /// @param[in]  column - The column to check (in units of tile maps).
        /// @return True if the position is valid; false otherwise.
        bool PositionInRange(const unsigned int row, const unsigned int column) const;

        /// Gets the filepath of the tile map at the specified row and column of the overworld.
        /// Row and column coordinates are 0-based.
        /// @param[in]  row - The row for which to get the tile map filepath (in units of tile maps).
        /// @param[in]  column - The column for which to get the tile map filepath (in units of tile maps).
        /// @return The filepath to the tile map at the specified location.
        /// @exception  std::out_of_range - Thrown if an invalid row or column value is provided.
        std::string GetTileMapFilepath(const unsigned int row, const unsigned int column) const;

    private:
        OverworldMapSpecification(const OverworldMapSpecification& specificationToCopy);  ///< Private to disallow copying.
        OverworldMapSpecification& operator=(const OverworldMapSpecification& rhsSpecification); ///< Private to disallow assignment.

        unsigned int OverworldWidthInTileMaps;    ///< The width of the overworld (in units of tile maps).
        unsigned int OverworldHeightInTileMaps;   ///< The height of the overworld (in units of tile maps).

        std::string StartingTileMapFilepath;  ///< The filepath to the overworld's starting tile map.
        MATH::Vector2ui StartingTileMapPosition;  ///< The 2D position of the starting tile map.
        
        /// Tile map filepaths within the overworld map, stored by their 2D positions relative to each other.
        CORE::Array2D<std::string> TileMapFilepaths;
    };
}