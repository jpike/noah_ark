#include <exception>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Maps/TiledMapJsonFile.h"

namespace MAPS
{
    TiledMapJsonFile::TiledMapJsonFile() :
    WidthInTiles(),
    HeightInTiles(),
    TileWidthInPixels(),
    TileHeightInPixels(),
    Tilesets(),
    Layers()
    {
        Clear();
    }

    bool TiledMapJsonFile::Load(const std::string& filepath)
    {
        // CLEAR ANY PREVIOUSLY LOADED DATA.
        Clear();

        // LOAD THE MAP FROM FILE.
        try
        {
            // PARSE THE JSON FILE.
            boost::property_tree::ptree mapFileData;
            boost::property_tree::json_parser::read_json(filepath, mapFileData);

            // READ THE MAP DIMENSIONS.
            WidthInTiles = mapFileData.get<unsigned int>("width");
            HeightInTiles = mapFileData.get<unsigned int>("height");

            // READ THE TILE DIMENSIONS.
            TileWidthInPixels = mapFileData.get<unsigned int>("tilewidth");
            TileHeightInPixels = mapFileData.get<unsigned int>("tileheight");

            // READ THE TILESETS.
            Tilesets = ReadTilesets(mapFileData);

            // READ THE TILE MAP LAYERS.
            Layers = ReadLayers(mapFileData, WidthInTiles, HeightInTiles);

            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    unsigned int TiledMapJsonFile::GetWidthInTiles() const
    {
        return WidthInTiles;
    }
    
    unsigned int TiledMapJsonFile::GetHeightInTiles() const
    {
        return HeightInTiles;
    }
    
    unsigned int TiledMapJsonFile::GetTileWidthInPixels() const
    {
        return TileWidthInPixels;
    }
    
    unsigned int TiledMapJsonFile::GetTileHeightInPixels() const
    {
        return TileHeightInPixels;
    }
    
    std::vector<TilesetDescription> TiledMapJsonFile::GetTilesets() const
    {
        return Tilesets;
    }
    
    std::vector<TileMapLayerDescription> TiledMapJsonFile::GetLayers() const
    {
        return Layers;
    }

    void TiledMapJsonFile::Clear()
    {
        WidthInTiles = 0;
        HeightInTiles = 0;
        TileWidthInPixels = 0;
        TileHeightInPixels = 0;
        Tilesets.clear();
        Layers.clear();
    }

    std::vector<TilesetDescription> TiledMapJsonFile::ReadTilesets(const boost::property_tree::ptree& mapFileData) const
    {
        std::vector<TilesetDescription> tilesets;

        // READ EACH TILESET.
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& tilesetData, mapFileData.get_child("tilesets"))
        {
            // READ THE CURRENT TILESET DESCRIPTION.
            TilesetDescription tileset = {};

            // Only fields that are currently needed are parsed.
            // The image filepath isn't parsed because the name
            // is used to identify the image resource for the tileset.
            // The image dimensions aren't read because they can be
            // determined later after the image is loaded from file.
            tileset.Name = tilesetData.second.get<std::string>("name");
            tileset.FirstTileId = tilesetData.second.get<unsigned int>("firstgid");
            tileset.TileWidthInPixels = tilesetData.second.get<unsigned int>("tilewidth");
            tileset.TileHeightInPixels = tilesetData.second.get<unsigned int>("tileheight");
            tileset.TransparentColor = tilesetData.second.get<std::string>("transparentcolor");            

            // ADD THE TILESET DESCRIPTION TO A CONTAINER FOR RETURNING.
            tilesets.push_back(tileset);
        }

        return tilesets;
    }

    std::vector<TileMapLayerDescription> TiledMapJsonFile::ReadLayers(
        const boost::property_tree::ptree& map_file_data,
        const unsigned int width_in_tiles,
        const unsigned int height_in_tiles) const
    {
        std::vector<TileMapLayerDescription> layers;

        // READ EACH LAYER.
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& layer_data, map_file_data.get_child("layers"))
        {
            // READ THE CURRENT LAYER'S NAME INTO A NEW LAYER DESCRIPTION.
            TileMapLayerDescription layer = {};
            layer.Name = layer_data.second.get<std::string>("name");

            // POPULATE THE LAYER DATA BASED ON ITS TYPE.
            std::string layerType = layer_data.second.get<std::string>("type");
            bool currentLayerHasTiles = ("tilelayer" == layerType);
            bool currentLayerHasObjects = ("objectgroup" == layerType);
            if (currentLayerHasTiles)
            {
                // POPULATE THE LAYER'S TYPE.
                layer.Type = TileMapLayerType::TILE_LAYER;

                // READ IN THE TILES.
                layer.TileIds = ReadTileIds(layer_data, width_in_tiles, height_in_tiles);
            }
            else if (currentLayerHasObjects)
            {
                // POPULATE THE LAYER'S TYPE.
                layer.Type = TileMapLayerType::OBJECT_LAYER;

                // READ IN THE OBJECTS.
                layer.Objects = ReadObjects(layer_data);
            }
            else
            {
                // Skip adding the current layer to the container
                // because it has an invalid type.
                continue;
            }

            // ADD THE LAYER DESCRIPTION TO A CONTAINER FOR RETURNING.
            layers.push_back(layer);
        }

        return layers;
    }

    CORE::Array2D<TileId> TiledMapJsonFile::ReadTileIds(
        const boost::property_tree::ptree::value_type& layer_data,
        const unsigned int width_in_tiles,
        const unsigned int height_in_tiles) const
    {
        CORE::Array2D<TileId> tile_ids(width_in_tiles, height_in_tiles);

        // READ EACH TILE ID.
        unsigned int current_x_position = 0;
        unsigned int current_y_position = 0;
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& tile_id_data, layer_data.second.get_child("data"))
        {
            // ADD THE CURRENT TILE ID TO THE COLLECTION.
            TileId tile_id = tile_id_data.second.get_value<TileId>();
            tile_ids(current_x_position, current_y_position) = tile_id;

            // ADJUST THE POSITION FOR THE NEXT TILE ID.
            ++current_x_position;
            bool full_row_populated = (current_x_position >= width_in_tiles);
            if (full_row_populated)
            {
                // Move to the beginning of the next row.
                current_x_position = 0;
                ++current_y_position;
            }

            // CHECK IF THE ARRAY OF TILE IDs HAS BEEN FULLY POPULAYED.
            bool full_array_populated = (current_y_position >= height_in_tiles);
            if (full_array_populated)
            {
                // Return the tile IDs collected so far.  Any other tile IDs will
                // be discarded.  This is acceptable since this scenario isn't expected
                // to actually occur.  If it does occur, the preference would be
                // to avoid any out-of-bounds exceptions.
                return tile_ids;
            }
        }

        return tile_ids;
    }

    std::vector<TileMapObjectDescription> TiledMapJsonFile::ReadObjects(const boost::property_tree::ptree::value_type& layerData) const
    {
        std::vector<TileMapObjectDescription> objects;

        // READ EACH OBJECT.
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& objectData, layerData.second.get_child("objects"))
        {
            // READ THE CURRENT OBJECT DESCRIPTION.
            TileMapObjectDescription object = {};

            // Only fields that are currently needed are parsed.
            object.Name = objectData.second.get<std::string>("name");
            object.Type = objectData.second.get<std::string>("type");
            object.TopLeftPositionInPixels.X = objectData.second.get<unsigned int>("x");
            object.TopLeftPositionInPixels.Y = objectData.second.get<unsigned int>("y");
            object.WidthInPixels = objectData.second.get<unsigned int>("width");
            object.HeightInPixels = objectData.second.get<unsigned int>("height");

            // ADD THE OBJECT DESCRIPTION TO A CONTAINER FOR RETURNING.
            objects.push_back(object);
        }

        return objects;
    }
}