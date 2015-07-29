#include <sstream>
#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Maps/OverworldMapSpecification.h"

namespace MAPS
{
    OverworldMapSpecification::OverworldMapSpecification() :
    OverworldWidthInTileMaps(0),
    OverworldHeightInTileMaps(0),
    StartingTileMapFilepath(),
    StartingTileMapPosition(0, 0),
    TileMapFilepaths()
    {

    }

    OverworldMapSpecification::~OverworldMapSpecification()
    {

    }

    bool OverworldMapSpecification::LoadFromFile(const std::string& filepath)
    {
        // LOAD THE OVERWORLD SPECIFICATION FROM FILE.
        try
        {
            // PARSE THE JSON FILE.
            boost::property_tree::ptree overworld_map_data;
            boost::property_tree::json_parser::read_json(filepath, overworld_map_data);

            // READ THE MAP DIMENSIONS.
            // Values are stored in local variables before being copied to members
            // in order to make the operation of this method appear atomic.
            // If any part of the loading fails, we don't want the specification
            // to be left in a partial/bad state.
            unsigned int width_in_tile_maps = overworld_map_data.get<unsigned int>("WidthInTilesMaps");
            bool width_valid = (width_in_tile_maps > 0);
            if (!width_valid)
            {
                return false;
            }

            unsigned int height_in_tile_maps = overworld_map_data.get<unsigned int>("HeightInTileMaps");
            bool height_valid = (height_in_tile_maps > 0);
            if (!height_valid)
            {
                return false;
            }

            // READ THE STARTING TILE MAP'S FILEPATH.
            std::string starting_tile_map_filepath = overworld_map_data.get<std::string>("StartingTileMapFilepath");
            bool starting_tile_map_filepath_valid = (!starting_tile_map_filepath.empty());
            if (!starting_tile_map_filepath_valid)
            {
                return false;
            }

            // READ IN THE ALL OF THE TILE MAP FILEPATHS.
            // Keep track of the number of filepaths read so that we can validate that
            // all maps get read.
            unsigned int tile_map_filepaths_read_count = 0;
            unsigned int current_tile_map_row = 0;
            unsigned int current_tile_map_column = 0;
            MATH::Vector2ui starting_tile_map_position(0, 0);
            CORE::Array2D<std::string> tile_map_filepaths(width_in_tile_maps, height_in_tile_maps);
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& tile_map_data, overworld_map_data.get_child("TileMaps"))
            {
                // Read in the current tile map filepath.
                std::string current_tile_map_filepath = tile_map_data.second.get<std::string>("Filepath");
                bool currrent_tile_map_filepath_valid = !current_tile_map_filepath.empty();;
                if (!currrent_tile_map_filepath_valid)
                {
                    return false;
                }

                // Save the current tile map filepath.
                tile_map_filepaths(current_tile_map_column, current_tile_map_row) = current_tile_map_filepath;
                ++tile_map_filepaths_read_count;

                // Store the position of the starting tile map if it matches the current tilemap.
                bool current_filepath_is_for_starting_tile_map = (starting_tile_map_filepath == current_tile_map_filepath);
                if (current_filepath_is_for_starting_tile_map)
                {
                    starting_tile_map_position = MATH::Vector2ui(current_tile_map_column, current_tile_map_row);
                }

                // Calculate the row/column position of the current tile map.
                const unsigned int MAX_TILE_MAP_COLUMN = width_in_tile_maps - 1;
                bool current_row_fully_loaded = (current_tile_map_column >= MAX_TILE_MAP_COLUMN);
                if (current_row_fully_loaded)
                {
                    // Move to the first column of the next row.
                    ++current_tile_map_row;
                    current_tile_map_column = 0;
                }
                else
                {
                    // Move to the next column of the current row.
                    ++current_tile_map_column;
                }
            }

            // VALIDATE THAT THE MAP WAS PROPERLY FORMED.
            // Verify the total number of filepaths.
            unsigned int expected_tile_map_filepaths_count = (width_in_tile_maps * height_in_tile_maps);
            bool expected_tile_map_filepaths_read = (expected_tile_map_filepaths_count == tile_map_filepaths_read_count);
            if (!expected_tile_map_filepaths_read)
            {
                return false;
            }

            // STORE THE VALID SPECIFICATION IN THIS OBJECT.
            OverworldWidthInTileMaps = width_in_tile_maps;
            OverworldHeightInTileMaps = height_in_tile_maps;
            StartingTileMapFilepath = starting_tile_map_filepath;
            StartingTileMapPosition = starting_tile_map_position;
            TileMapFilepaths = tile_map_filepaths;

            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    unsigned int OverworldMapSpecification::GetOverworldWidthInTileMaps() const
    {
        return OverworldWidthInTileMaps;
    }

    unsigned int OverworldMapSpecification::GetOverworldHeightInTileMaps() const
    {
        return OverworldHeightInTileMaps;
    }

    std::string OverworldMapSpecification::GetStartingTileMapFilepath() const
    {
        return StartingTileMapFilepath;
    }

    MATH::Vector2ui OverworldMapSpecification::GetStartingTileMapPosition() const
    {
        return StartingTileMapPosition;
    }

    bool OverworldMapSpecification::PositionInRange(const unsigned int row, const unsigned int column) const
    {
        // Validate that the coordinates are in range.
        bool rowNumberValid = (OverworldHeightInTileMaps > row);
        bool columnNumberValid = (OverworldWidthInTileMaps > column);
        bool positionValid = (rowNumberValid && columnNumberValid);
        return positionValid;
    }

    std::string OverworldMapSpecification::GetTileMapFilepath(const unsigned int row, const unsigned int column) const
    {
        // VALIDATE THE ROW AND COLUMN COORDINATES.
        // We expect that the container containing the actual filepaths was previously validated
        // to have dimensions matching the separate height/width members of this object.
        bool rowNumberValid = (OverworldHeightInTileMaps > row);
        if (!rowNumberValid)
        {
            std::stringstream invalidRowErrorMessage;
            invalidRowErrorMessage
                << "Invalid row of " << row << " specified for a tile map filepath in an overworld.  "
                << "Expected a value less than " << OverworldHeightInTileMaps << std::endl;
            throw std::out_of_range(invalidRowErrorMessage.str());
        }

        bool columnNumberValid = (OverworldWidthInTileMaps > column);
        if (!columnNumberValid)
        {
            std::stringstream invalidColumnErrorMessage;
            invalidColumnErrorMessage
                << "Invalid column of " << column << " specified for a tile map filepath in an overworld.  "
                << "Expected a value less than " << OverworldWidthInTileMaps << std::endl;
            throw std::out_of_range(invalidColumnErrorMessage.str());
        }

        // GET THE REQUEST FILEPATH.
        // We expect that any empty values were not added in population of the member.
        const std::string& tileMapFilepath = TileMapFilepaths(column, row);
        return tileMapFilepath;
    }
}