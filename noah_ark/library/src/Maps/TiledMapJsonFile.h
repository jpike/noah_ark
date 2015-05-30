#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "Maps/ITileMapData.h"
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
    ///
    /// @note    This class uses boost's property tree library,
    /// which isn't thread-safe by default.
    /// This code expects BOOST_SPIRIT_THREADSAFE to be defined for
    /// this class to be used safely across multiple threads.
    class TiledMapJsonFile : public ITileMapData
    {
    public:
        // CONSTRUCTION/DESTRUCTION.
        /// Constructor.  No data will be loaded from any file.
        explicit TiledMapJsonFile();
        /// Destructor.
        virtual ~TiledMapJsonFile();

        // FILE LOADING.
        /// Loads Tiled map data from the specified JSON file
        /// to populate this object.  Any pre-existing data
        /// in this object will be cleared before loading.
        /// @param[in]  filepath - The path to the JSON map file.
        /// @return True if the file was successfully loaded;
        ///     false otherwise.
        bool Load(const std::string& filepath);

        // DATA ACCESS.
        /// @copydoc    ITileMapData::GetWidthInTiles() const
        virtual unsigned int GetWidthInTiles() const;
        /// @copydoc    ITileMapData::GetHeightInTiles() const
        virtual unsigned int GetHeightInTiles() const;
        /// @copydoc    ITileMapData::GetTileWidthInPixels() const
        virtual unsigned int GetTileWidthInPixels() const;
        /// @copydoc    ITileMapData::GetTileHeightInPixels() const
        virtual unsigned int GetTileHeightInPixels() const;
        /// @copydoc    ITileMapData::GetTilesets() const
        virtual std::vector<TilesetDescription> GetTilesets() const;
        /// @copydoc    ITileMapData::GetLayers() const
        virtual std::vector<TileMapLayerDescription> GetLayers() const;

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
        /// @param[in]  map_file_data - The tile map data.
        /// @param[in]  width_in_tiles - The width of the layer (in tiles).
        /// @param[in]  height_in_tiles - The height of the layer (in tiles).
        /// @return The layers in the map, in order from bottom to top.
        /// @throws std::exception - Thrown if an error occurs.
        std::vector<TileMapLayerDescription> ReadLayers(
            const boost::property_tree::ptree& map_file_data,
            const unsigned int width_in_tiles,
            const unsigned int height_in_tiles) const;
        /// Reads tile IDs from the provided layer property tree data.
        /// @param[in]  layer_data - The layer data.
        /// @param[in]  width_in_tiles - The width of the layer (in tiles).
        /// @param[in]  height_in_tiles - The height of the layer (in tiles).
        /// @return The tile IDs in the layer.
        /// @throws std::exception - Thrown if an error occurs.
        CORE::Array2D<TileId> ReadTileIds(
            const boost::property_tree::ptree::value_type& layer_data,
            const unsigned int width_in_tiles,
            const unsigned int height_in_tiles) const;
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