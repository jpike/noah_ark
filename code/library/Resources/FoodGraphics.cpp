#include "Resources/FoodGraphics.h"

namespace RESOURCES
{
    /// Gets a sprite for the specified food type.
    /// @param[in]  food_type - The type of food to get a sprite for.
    /// @return The food sprite, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::Sprite> FoodGraphics::GetSprite(const OBJECTS::FoodType food_type)
    {
        // GET THE TEXTURE RECTANGLE FOR THE SPRITE.
        MATH::FloatRectangle texture_sub_rectangle;
        switch (food_type)
        {
            case OBJECTS::FoodType::ALMOND:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::APPLE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::APRICOT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::AVOCADO:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::BANANA:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::BLACKBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::BLUEBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CANTELOUPE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CASHEW:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CHERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CHESTNUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CLEMENTINE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CLOUDBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::COCONUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::CRANBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::DURIAN:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::ELDERBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::FIG:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::GOURD:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::GRAPE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::GRAPEFRUIT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::GUAVA:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    16.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::HAZELNUT:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::LEMON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::LIME:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::LYCHEE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::MANGO:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::MELON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::MULBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::OLIVE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::ORANGE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PAWPAW:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    80.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PEACH:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    96.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PEAR:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    112.0f,
                    32.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PECAN:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PERSIMMON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    16.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::PLUM:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::POMEGRANATE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    48.0f,
                    48.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::RASPBERRY:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    64.0f,
                    16.0f,
                    16.0f);
                break;
            case OBJECTS::FoodType::WALNUT:
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
