#include "Resources/FoodGraphics.h"

namespace RESOURCES
{
    /// Gets a sprite for the specified food type.
    /// @param[in]  food_type - The type of food to get a sprite for.
    /// @param[in,out]  assets - The assets from which to get the sprite.
    /// @return The food sprite, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::Sprite> FoodGraphics::GetSprite(
        const OBJECTS::FoodType food_type,
        Assets& assets)
    {
        // GET THE FOOD TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> food_texture = assets.GetTexture(RESOURCES::FOOD_TEXTURE_ID);
        if (!food_texture)
        {
            return nullptr;
        }

        // GET THE TEXTURE RECTANGLE FOR THE SPRITE.
        MATH::FloatRectangle texture_sub_rectangle;
        switch (food_type)
        {
            case OBJECTS::FoodType::APPLE:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    0.0f,
                    0.0f,
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
            case OBJECTS::FoodType::MELON:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    32.0f,
                    0.0f,
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
            case OBJECTS::FoodType::FIG:
                texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    64.0f,
                    0.0f,
                    16.0f,
                    16.0f);
                break;
            default:
                // The food type is invalid, so no texture rectangle can be determined.
                return nullptr;
        }

        // CREATE THE FOOD'S SPRITE.
        auto food_sprite = std::make_shared<GRAPHICS::Sprite>(food_texture, texture_sub_rectangle);
        return food_sprite;
    }
}
