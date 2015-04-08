#pragma once

#include <iostream>
#include <string>
#include <boost/foreach.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace TILED_MAP_JSON_FILE_TESTS
{
    TEST_CASE("Loading a test JSON tile map.", "[TileMapJsonFile]")
    {
        try
        {
            // LOAD.
            const std::string MAP_FILEPATH = "res/maps/tree_test.json";
            boost::property_tree::ptree mapFileTree;
            boost::property_tree::json_parser::read_json(MAP_FILEPATH, mapFileTree);

            // GET TOP-LEVEL DETAILS.
            unsigned int height = mapFileTree.get<unsigned int>("height");
            REQUIRE(24 == height);
            std::string orientation = mapFileTree.get<std::string>("orientation");
            REQUIRE("orthogonal" == orientation);
            /// @todo properties?
            unsigned int tileHeight = mapFileTree.get<unsigned int>("tileheight");
            REQUIRE(16 == tileHeight);
            unsigned int tileWidth = mapFileTree.get<unsigned int>("tilewidth");
            REQUIRE(16 == tileWidth);
            unsigned int version = mapFileTree.get<unsigned int>("version");
            REQUIRE(1 == version);
            unsigned int width = mapFileTree.get<unsigned int>("width");
            REQUIRE(32 == width);

            // GET LAYERS.
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& layer, mapFileTree.get_child("layers"))
            {
                std::string first = layer.first; // This is always empty.
                /// @todo   data?
                unsigned int layerHeight = layer.second.get<unsigned int>("height");
                std::string layerName = layer.second.get<std::string>("name");
                unsigned int layerOpacity = layer.second.get<unsigned int >("opacity");
                std::string layerType = layer.second.get<std::string>("type");
                bool layerVisible = layer.second.get<bool>("visible");
                unsigned int layerWidth = layer.second.get<unsigned int>("width");
                unsigned int layerX = layer.second.get<unsigned int>("x");
                unsigned int layerY = layer.second.get<unsigned int>("y");

                if ("tilelayer" == layerType)
                {
                    /// @todo   data?
                    BOOST_FOREACH(const boost::property_tree::ptree::value_type& data, layer.second.get_child("data"))
                    {
                        std::string dataFirst = data.first;
                        unsigned int dataValue = data.second.get_value<unsigned int>();
                    }
                }
                else if ("objectgroup" == layerType)
                {
                    /// @todo   objects?
                    BOOST_FOREACH(const boost::property_tree::ptree::value_type& object, layer.second.get_child("objects"))
                    {
                        std::string objectFirst = object.first;
                        unsigned int objectHeight = object.second.get<unsigned int>("height");
                        std::string objectName = object.second.get<std::string>("name");
                        /// @todo   properties?
                        std::string objectType = object.second.get<std::string>("type");
                        bool objectVisible = object.second.get<bool>("visible");
                        unsigned int objectWidth = object.second.get<unsigned int>("width");
                        unsigned int objectX = object.second.get<unsigned int>("x");
                        unsigned int objectY = object.second.get<unsigned int>("y");
                    }
                }
            }

            // GET TILESETS.
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& tileset, mapFileTree.get_child("tilesets"))
            {
                std::string first = tileset.first;

                unsigned int firstGid = tileset.second.get<unsigned int>("firstgid");
                std::string imageFilepath = tileset.second.get<std::string>("image");
                unsigned int imageHeight = tileset.second.get<unsigned int>("imageheight");
                unsigned int imageWidth = tileset.second.get<unsigned int>("imagewidth");
                unsigned int margin = tileset.second.get<unsigned int>("margin");
                std::string tilesetName = tileset.second.get<std::string>("name");
                /// @todo   properties?
                unsigned int spacing = tileset.second.get<unsigned int>("spacing");
                unsigned int tilesetTileHeight = tileset.second.get<unsigned int>("tileheight");
                unsigned int tilesetTileWidth = tileset.second.get<unsigned int>("tilewidth");
                std::string transparentColor = tileset.second.get<std::string>("transparentcolor");
            }
        }
        catch (const boost::property_tree::json_parser_error& exception)
        {
            std::cerr << "Exception: " << exception.what() << std::endl;
        }
        catch (const boost::property_tree::ptree_error& exception)
        {
            std::cerr << "Exception: " << exception.what() << std::endl;
        }
    }
}