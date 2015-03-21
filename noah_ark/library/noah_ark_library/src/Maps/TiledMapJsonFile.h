#pragma once

#include <string>
#include <vector>
#include "Math/Vector2.h"

namespace MAPS
{
    /// A typedef to hide the underlying type used for tile IDs
    /// and clarify when variables refer to tile IDs.
    typedef unsigned int TileId;

    /// Identifies the different types of layers that may
    /// exist in a tile map.
    enum class TileMapLayerType
    {
        /// The layer contains tiles in a grid.
        TILE_LAYER,
        /// The layer contains objects defined by rectangles.
        OBJECT_LAYER
    };

    /// A description of an object in a tile map object layer.
    struct TileMapObjectDescription
    {
        /// The name identifying the object.
        std::string Name;
        /// The type of the object.  An arbitrary string
        /// as set in the tile map file.
        std::string Type;
        /// The top left position of the object, relative
        /// to the top left position of its containing map.
        MATH::Vector2ui TopLeftPositionInPixels;
        /// The width of the object in pixels.
        unsigned int WidthInPixels;
        /// The height of the object in pixels.
        unsigned int HeightInPixels;
    };

    /// A description of a layer in a tile map.
    /// Many fields of layers in Tiled map files (ex. position/dimensions)
    /// are excluded because they only provide redundant data that could
    /// be figured out from other tile map properties.
    struct TileMapLayerDescription
    {
        /// The name identifying the tile map layer.
        std::string Name;
        /// The type of the tile map layer.
        TileMapLayerType Type;
        /// The IDs of each tile in the layer.  Only populated if the type
        /// of layer in this description is for a tile layer.  The tile
        /// IDs are contained in 1D container to simplify interaction logic.
        /// The tiles start with the top row, going from left to right,
        /// with rows further down in the layer following.
        std::vector<TileId> TileIds;
        /// A list of objects in the layer.  Only populated if the type
        /// of layer in this description is for an object layer.
        std::vector<TileMapObjectDescription> Objects;
    };

    /// A basic description of a tileset as read from
    /// a Tiled tile map.  Only basic properties that
    /// are currently needed are included as fields,
    /// but this list may be expanded later as necessary.
    struct TilesetDescription
    {
        /// The name of the tileset.  Identifies the texture
        /// resource for the tileset.
        std::string Name;
        /// The globally unique tile ID of the first tile in the tileset.
        unsigned int FirstTileId;
        /// The width of a tile in the tileset in pixels.
        unsigned int TileWidthInPixels;
        /// The height of a tile in the tileset in pixels.
        unsigned int TileHeightInPixels;
        /// The transparent color in the tileset image.
        /// Must be represented as an HTML hexadecimal HTML color
        /// code like #00ffcc.  Capitalization doesn't matter.
        std::string TransparentColor;
    };

    /// A tile map exported from the Tiled map editor
    /// (http://www.mapeditor.org/) in JSON format.
    /// This class is responsible for reading in these
    /// JSON tile maps.  It currently does not support
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