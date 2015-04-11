#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "Maps/TileMapLayerDescription.h"
#include "Maps/TileMapObjectDescription.h"
#include "Maps/TilesetDescription.h"

namespace MAPS
{
    /// A tile map exported from the Tiled map editor
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
    class TiledMapJsonFile
    {
    public:
        // CONSTRUCTION.
        /// Constructor.  No data will be loaded from any file.
        explicit TiledMapJsonFile();

        // FILE LOADING.
        /// Loads Tiled map data from the specified JSON file
        /// to populate this object.  Any pre-existing data
        /// in this object will be cleared before loading.
        /// @param[in]  filepath - The path to the JSON map file.
        /// @return True if the file was successfully loaded;
        ///     false otherwise.
        bool Load(const std::string& filepath);

        // DATA ACCESS.
        /// Gets the tile map's width (in tiles);
        /// @return The map's width.
        unsigned int GetWidthInTiles() const;
        /// Gets the tile map's height (in tiles);
        /// @return The map's height.
        unsigned int GetHeightInTiles() const;
        /// Gets the width (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        /// @return The width of a tile.
        unsigned int GetTileWidthInPixels() const;
        /// Gets the height (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        /// @return The height of a tile.
        unsigned int GetTileHeightInPixels() const;
        /// Gets descriptions of tilesets for the tile map.
        /// @return Tilesets for the tile map.
        std::vector<TilesetDescription> GetTilesets() const;
        /// Gets descriptions of layers in the tile map.
        /// @return Layers for the tile map.
        std::vector<TileMapLayerDescription> GetLayers() const;

    private:
        // HELPER METHODS.
        /// Clears any existing data loaded into this object.
        void Clear();
        /// Reads tileset data from the map property tree.
        /// @param[in]  mapFileData - The tile map data.
        /// @return The tilesets in the map.
        /// @throws std::exception - Thrown if an error occurs.
        std::vector<TilesetDescription> ReadTilesets(const boost::property_tree::ptree& mapFileData) const;
        /// Reads layer data from the map property tree.
        /// @param[in]  mapFileData - The tile map data.
        /// @return The layers in the map, in order from bottom to top.
        /// @throws std::exception - Thrown if an error occurs.
        std::vector<TileMapLayerDescription> ReadLayers(const boost::property_tree::ptree& mapFileData) const;
        /// Reads tile IDs from the provided layer property tree data.
        /// @param[in]  layerData - The layer data.
        /// @return The tile IDs in the layer.  See @ref TileMapLayerDescription::TileIds
        ///     for details of the exact layout of the tile IDs.
        /// @throws std::exception - Thrown if an error occurs.
        std::vector<TileId> ReadTileIds(const boost::property_tree::ptree::value_type& layerData) const;
        /// Reads object descriptions from the provided layer property tree data.
        /// @param[in]  layerData - The layer data.
        /// @return The descriptions of all objects in the layer.
        /// @throws std::exception - Thrown if an error occurs.
        std::vector<TileMapObjectDescription> ReadObjects(const boost::property_tree::ptree::value_type& layerData) const;

        // MEMBER VARIABLES.
        /// The width of the map in tiles.
        unsigned int WidthInTiles;
        /// The height of the map in tiles.
        unsigned int HeightInTiles;
        /// The width (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileWidthInPixels;
        /// The height (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        unsigned int TileHeightInPixels;
        /// Descriptions of the tilesets used in the tile map.
        std::vector<TilesetDescription> Tilesets;
        /// Descriptions of the layers in the tile map, in order from bottom to top.
        std::vector<TileMapLayerDescription> Layers;
    };
}