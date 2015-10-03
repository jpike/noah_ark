#include <exception>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Maps/TileMapFile.h"

namespace MAPS
{
    std::unique_ptr<TileMapFile> TileMapFile::Load(const std::string& filepath)
    {
        // LOAD THE MAP FROM FILE.
        try
        {
            // PARSE THE JSON FILE.
            boost::property_tree::ptree map_file_data;
            boost::property_tree::json_parser::read_json(filepath, map_file_data);

            // READ THE MAP DIMENSIONS.
            unsigned int width_in_tiles = map_file_data.get<unsigned int>("width");
            bool width_valid = (width_in_tiles > 0);
            if (!width_valid)
            {
                return nullptr;
            }

            unsigned int height_in_tiles = map_file_data.get<unsigned int>("height");
            bool height_valid = (height_in_tiles > 0);
            if (!height_valid)
            {
                return nullptr;
            }

            // READ THE TILE DIMENSIONS.
            unsigned int tile_width_in_pixels = map_file_data.get<unsigned int>("tilewidth");
            bool tile_width_valid = (tile_width_in_pixels > 0);
            if (!tile_width_valid)
            {
                return nullptr;
            }

            unsigned int tile_height_in_pixels = map_file_data.get<unsigned int>("tileheight");
            bool tile_height_valid = (tile_height_in_pixels > 0);
            if (!tile_height_valid)
            {
                return nullptr;
            }

            // READ THE TILESETS.
            std::vector<TilesetDescription> tilesets = ReadTilesets(map_file_data);
            bool tilesets_read = !tilesets.empty();
            if (!tilesets_read)
            {
                return nullptr;
            }

            // READ THE TILE MAP LAYERS.
            std::vector<TileMapLayerDescription> layers = ReadLayers(map_file_data, width_in_tiles, height_in_tiles);
            bool layers_read = !layers.empty();
            if (!layers_read)
            {
                return nullptr;
            }

            // POPULATE THE TILE MAP FILE.
            std::unique_ptr<TileMapFile> tile_map_file = std::make_unique<TileMapFile>();
            tile_map_file->MapWidthInTiles = width_in_tiles;
            tile_map_file->MapHeightInTiles = height_in_tiles;
            tile_map_file->TileWidthInPixels = tile_width_in_pixels;
            tile_map_file->TileHeightInPixels = tile_height_in_pixels;
            tile_map_file->Tilesets = tilesets;
            tile_map_file->Layers = layers;

            return tile_map_file;
        }
        catch (const std::exception&)
        {
            // The tile map file could not be loaded.
            return nullptr;
        }
    }

    std::vector<TilesetDescription> TileMapFile::ReadTilesets(const boost::property_tree::ptree& map_file_data)
    {
        std::vector<TilesetDescription> tilesets;

        // READ EACH TILESET.
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& tileset_data, map_file_data.get_child("tilesets"))
        {
            // READ THE CURRENT TILESET DESCRIPTION.
            TilesetDescription tileset = {};

            // Only fields that are currently needed are parsed.
            // The image filepath isn't parsed because the name
            // is used to identify the image resource for the tileset.
            // The image dimensions aren't read because they can be
            // determined later after the image is loaded from file.
            tileset.Name = tileset_data.second.get<std::string>("name");
            tileset.FirstTileId = tileset_data.second.get<unsigned int>("firstgid");
            tileset.TileWidthInPixels = tileset_data.second.get<unsigned int>("tilewidth");
            tileset.TileHeightInPixels = tileset_data.second.get<unsigned int>("tileheight");
            tileset.TransparentColor = tileset_data.second.get<std::string>("transparentcolor");

            // ADD THE TILESET DESCRIPTION TO A CONTAINER FOR RETURNING.
            tilesets.push_back(tileset);
        }

        return tilesets;
    }

    std::vector<TileMapLayerDescription> TileMapFile::ReadLayers(
        const boost::property_tree::ptree& map_file_data,
        const unsigned int width_in_tiles,
        const unsigned int height_in_tiles)
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

    CORE::Array2D<TileId> TileMapFile::ReadTileIds(
        const boost::property_tree::ptree::value_type& layer_data,
        const unsigned int width_in_tiles,
        const unsigned int height_in_tiles)
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

    std::vector<TileMapObjectDescription> TileMapFile::ReadObjects(const boost::property_tree::ptree::value_type& layerData)
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