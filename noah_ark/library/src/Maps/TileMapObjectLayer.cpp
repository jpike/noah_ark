#include "Maps/TileMapObjectLayer.h"
#include "Objects/ObjectType.h"

namespace MAPS
{
    /// @todo   Finish implementation!
    TileMapObjectLayer::TileMapObjectLayer(
        const std::string& name,
        const std::vector<TileMapObjectDescription>& objects,
        GRAPHICS::GraphicsSystem& graphics_system) :
    Name(name),
    Trees()
    {
        // CREATE OBJECTS FOR EACH PROVIDED OBJECT DESCRIPTION.
        /// @todo   Create a separate helper method?
        for (const auto& object_description : objects)
        {
            // CREATE THE OBJECT BASED ON ITS TYPE.
            /// @todo   Maybe this "factory" creation should go in the ObjectType struct?
            OBJECTS::ObjectType::Type object_type = OBJECTS::ObjectType::FromString(object_description.Type);
            switch (object_type)
            {
                case OBJECTS::ObjectType::TREE:
                {
                    /// @todo   Do we need a base class/shared_ptr?
                    /*OBJECTS::Tree tree(object_description, graphics_system);
                    /// @todo   Spatial partition?
                    Trees.push_back(tree);*/
                }
                case OBJECTS::ObjectType::INVALID:
                default:
                    // Don't create any object since a valid object type was not detected.
                    break;
            }
        }
    }

    TileMapObjectLayer::~TileMapObjectLayer()
    {}

    std::shared_ptr<Tile> TileMapObjectLayer::GetTileAtGridPosition(
        const unsigned int tile_x_grid_position,
        const unsigned int tile_y_grid_position) const
    {
        // No tiles exist in an object layer for returning.
        return nullptr;
    }

    void TileMapObjectLayer::Render(sf::RenderTarget& render_target)
    {
        /// @todo   Implement!
    }
}
