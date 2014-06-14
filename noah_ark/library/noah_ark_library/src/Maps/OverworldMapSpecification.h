#pragma once

#include <string>
#include <vector>
#include <TmxParser/tinyxml/tinyxml.h>
#include "Math/Vector2.h"

namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A specification of the overworld map.  The specification
    ///         describes the individual tile maps that comprise the overworld.
    ///         It can be loaded from a properly formatted XML document.
    ///////////////////////////////////////////////////////////
    class OverworldMapSpecification
    {
    public:
        /// @brief  Constructor.
        explicit OverworldMapSpecification();
        /// @brief  Destructor.
        ~OverworldMapSpecification();

        /// @brief      Loads the specification from an XML file.
        /// @param[in]  filepath - The path to the XML file to load.
        /// @return     True if loaded successfully; false otherwise.
        bool LoadFromFile(const std::string& filepath);

        /// @brief      Gets the number of tile maps in a single row of the overworld.
        ///             Width is 1-based.
        /// @return     The width of the overworld in units of tile maps.
        unsigned int GetOverworldWidthInTileMaps() const;

        /// @brief      Gets the number of tile maps in a single column of the overworld.
        ///             Height is 1-based.
        /// @return     The height of the overworld in units of tile maps.
        unsigned int GetOverworldHeightInTileMaps() const;

        /// @brief      Gets the filepath to the starting tile map of the overworld.
        /// @return     The filepath to the overworld's starting tile map.
        std::string GetStartingTileMapFilepath() const;

        /// @brief      Gets the 2D x, y (column, row) position of the starting tile map
        ///             in the overworld map.
        /// @return     The 2D position of the starting tile map.
        MATH::Vector2ui GetStartingTileMapPosition() const;

        /// @brief      Gets the filepath of the tile map at the specified row and column of the overworld.
        ///             Row and column coordinates are 0-based.
        /// @param[in]  row - The row for which to get the tile map filepath (in units of tile maps).
        /// @param[in]  column - The column for which to get the tile map filepath (in units of tile maps).
        /// @return     The filepath to the tile map at the specified location.
        /// @exception  std::out_of_range - Thrown if an invalid row or column value is provided.
        std::string GetTileMapFilepath(const unsigned int row, const unsigned int column) const;

    private:
        OverworldMapSpecification(const OverworldMapSpecification& specificationToCopy);  ///< Private to disallow copying.
        OverworldMapSpecification& operator=(const OverworldMapSpecification& rhsSpecification); ///< Private to disallow assignment.

        unsigned int m_overworldWidthInTileMaps;    ///< The width of the overworld (in units of tile maps).
        unsigned int m_overworldHeightInTileMaps;   ///< The height of the overworld (in units of tile maps).

        std::string m_startingTileMapFilepath;  ///< The filepath to the overworld's starting tile map.
        MATH::Vector2ui m_startingTileMapPosition;  ///< The 2D position of the starting tile map.
        
        /// @brief  Tile map filepaths within the overworld map, stored by their 2D positions relative to each other.
        ///         The y (vertical) coordinate comes first, followed by the x (horizontal) coordinate.
        ///         For example, the map at (0,0) is the top-left map.  The map at (0,1) is right of the top-left map.
        ///         The map at (1,0) is below the top-left map.
        std::vector< std::vector<std::string> > m_tileMapFilepaths;
    };
}