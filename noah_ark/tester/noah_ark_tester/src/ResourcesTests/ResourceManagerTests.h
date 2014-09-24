#pragma once

#include <iostream>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

/// A namespace for testing the FloatRectangle class.
namespace FLOAT_RECTANGLE_TESTS
{
    TEST_CASE( "Boost Property Tree Test.", "[Boost]" )
    {
        try
        {
            boost::property_tree::ptree propertyTree;
            boost::property_tree::json_parser::read_json("res/resources.json", propertyTree);

            std::cout << propertyTree.get<std::string>("textures") << std::endl;
            std::cout << propertyTree.get<std::string>("textures.ground_tile_set") << std::endl;
            std::cout << propertyTree.get<std::string>("textures.ground_tile_set.filepath") << std::endl;
            std::cout << propertyTree.get<std::string>("textures.noah_texture") << std::endl;
            std::cout << propertyTree.get<std::string>("textures.noah_texture.filepath") << std::endl;
            std::cout << propertyTree.get<std::string>("sprites") << std::endl;
            std::cout << propertyTree.get<std::string>("sprites.noah_sprite") << std::endl;
            std::cout << propertyTree.get<std::string>("sprites.noah_sprite.texture") << std::endl;
            std::cout << propertyTree.get<std::string>("sprites.noah_sprite.center") << std::endl;
            std::cout << propertyTree.get<float>("sprites.noah_sprite.center.x") << std::endl;
            std::cout << propertyTree.get<float>("sprites.noah_sprite.center.y") << std::endl;
            std::cout << propertyTree.get<std::string>("sprites.noah_sprite.size") << std::endl;
            std::cout << propertyTree.get<float>("sprites.noah_sprite.size.width") << std::endl;
            std::cout << propertyTree.get<float>("sprites.noah_sprite.size.height") << std::endl;
            std::cout << propertyTree.get<std::string>("animations") << std::endl;
            std::cout << propertyTree.get<std::string>("animations.noah_walk_front") << std::endl;
            std::cout << propertyTree.get<bool>("animations.noah_walk_front.loop") << std::endl;
            std::cout << propertyTree.get<float>("animations.noah_walk_front.duration_in_seconds") << std::endl;
            std::cout << propertyTree.get<std::string>("animations.noah_walk_front.frames") << std::endl;

            BOOST_FOREACH(const boost::property_tree::ptree::value_type& frame, propertyTree.get_child("animations.noah_walk_front.frames"))
            {
                std::cout << frame.second.get<float>("left") << std::endl;
                std::cout << frame.second.get<float>("top") << std::endl;
                std::cout << frame.second.get<float>("width") << std::endl;
                std::cout << frame.second.get<float>("height") << std::endl;
            }
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Standard exception: " << exception.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception." << std::endl;
        }
    }
}
