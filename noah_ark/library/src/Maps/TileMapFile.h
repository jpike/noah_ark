#pragma once

#include <memory>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "Core/Array2D.h"
#include "Maps/TileMapLayerDescription.h"
#include "Maps/TileMapObjectDescription.h"
#include "Maps/TilesetDescription.h"

namespace MAPS
{
    /// A tile map file exported from the Tiled map editor
    /// (http://www.mapeditor.org/) in JSON format.
    /// This class is responsible for reading in these
    /// JSON tile maps.  The JSON format was chosen for its
    /// smaller size than full XML and simplicity in parsing.
    /// This class currently does not support
    /// all features of this JSON tile map format -
    /// only those currently needed.  It may be extended
    /// to support additional fields as needed.
    /// See public methods and returned data structures
    /// for details on the exact features currently
    /// supported.
    ///
    /// @note    This class uses boost's property tree library,
    /// which isn't thread-safe by default.
    /// This code expects BOOST_SPIRIT_THREADSAFE to be defined for
    /// this class to be used safely across multiple threads.
    class TileMapFile
    {
    public:
        // CONSTRUCTION.
        /// Attempts to load the tile map file at the specified filepath into memory.
        /// @param[in]  filepath - The path to the tile map file to load.
        ///     May be relative or absolute but must be accessible from the working
        ///     directory of this game.
        /// @return The tile map file, if successfully loaded; false otherwise.
        static std::unique_ptr<TileMapFile> Load(const std::string& filepath);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The width of the map in tiles.
        unsigned int MapWidthInTiles = 0;
        /// The height of the map in tiles.
        unsigned int MapHeightInTiles = 0;
        /// The width (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileWidthInPixels = 0;
        /// The height (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileHeightInPixels = 0;
        /// Descriptions of the tilesets used in the tile map.
        std::vector<TilesetDescription> Tilesets = std::vector<TilesetDescription>();
        /// Descriptions of the layers in the tile map, in order from bottom to top.
        std::vector<TileMapLayerDescription> Layers = std::vector<TileMapLayerDescription>();

    private:
        // HELPER METHODS.
        /// Reads tileset data from the map property tree.
        /// @param[in]  map_file_data - The tile map data.
        /// @return The tilesets in the map.
        /// @throws std::exception - Thrown if an error occurs.
        static std::vector<TilesetDescription> ReadTilesets(const boost::property_tree::ptree& map_file_data);
        /// Reads layer data from the map property tree.
        /// @param[in]  map_file_data - The tile map data.
        /// @param[in]  width_in_tiles - The width of the layer (in tiles).
        /// @param[in]  height_in_tiles - The height of the layer (in tiles).
        /// @return The layers in the map, in order from bottom to top.
        /// @throws std::exception - Thrown if an error occurs.
        static std::vector<TileMapLayerDescription> ReadLayers(
            const boost::property_tree::ptree& map_file_data,
            const unsigned int width_in_tiles,
            const unsigned int height_in_tiles);
        /// Reads tile IDs from the provided layer property tree data.
        /// @param[in]  layer_data - The layer data.
        /// @param[in]  width_in_tiles - The width of the layer (in tiles).
        /// @param[in]  height_in_tiles - The height of the layer (in tiles).
        /// @return The tile IDs in the layer.
        /// @throws std::exception - Thrown if an error occurs.
        static CORE::Array2D<TileId> ReadTileIds(
            const boost::property_tree::ptree::value_type& layer_data,
            const unsigned int width_in_tiles,
            const unsigned int height_in_tiles);
        /// Reads object descriptions from the provided layer property tree data.
        /// @param[in]  layerData - The layer data.
        /// @return The descriptions of all objects in the layer.
        /// @throws std::exception - Thrown if an error occurs.
        static std::vector<TileMapObjectDescription> ReadObjects(const boost::property_tree::ptree::value_type& layerData);
    };
}