#include "Resources/FoodGraphics.h"

namespace RESOURCES
{
    /// Gets a sprite for the specified food type.
    /// @param[in]  food_type - The type of food to get a sprite for.
    /// @return The food sprite, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::Sprite> FoodGraphics::GetSprite(const OBJECTS::Food::TypeId food_type)
    {
        // GET THE TEXTURE RECTANGLE FOR THE SPRITE.
        MATH::FloatRectangle texture_sub_rectangle;
        switch (food_type)
        {
            case OBJECTS::Food::TypeId::ALMOND:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::APPLE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::APRICOT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::AVOCADO:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::BANANA:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::BLACKBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::BLUEBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CANTELOUPE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CASHEW:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CHERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CHESTNUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CLEMENTINE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CLOUDBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::COCONUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::CRANBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::DURIAN:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::ELDERBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::FIG:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::GOURD:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::GRAPE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::GRAPEFRUIT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::GUAVA:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::HAZELNUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::LEMON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::LIME:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::LYCHEE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::MANGO:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::MELON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::MULBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::OLIVE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::ORANGE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PAWPAW:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PEACH:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PEAR:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PECAN:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PERSIMMON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::PLUM:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::POMEGRANATE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::RASPBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::Food::TypeId::WALNUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            default:
                // The food type is invalid, so no texture rectangle can be determined.
                return nullptr;
        }

        // CREATE THE FOOD'S SPRITE.
        auto food_sprite = std::make_shared<GRAPHICS::Sprite>(RESOURCES::AssetId::FOOD_TEXTURE, texture_sub_rectangle);
        return food_sprite;
    }
}
