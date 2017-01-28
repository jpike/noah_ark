#include "Core/NullChecking.h"
#include "Objects/ArkPiece.h"

namespace OBJECTS
{
    /// Constructs an ark piece based on the specified data.
    /// @param[in]  id - The ID of the ark piece.
    /// @param[in]  texture - The texture with the ark piece graphics.
    /// @throws std::invalid_argument - Thrown if the texture is null.
    ArkPiece::ArkPiece(const unsigned int id, const std::shared_ptr<GRAPHICS::Texture>& texture) :
        Id(id),
        Sprite(),
        Built(false)
    {
        // MAKE SURE A VALID TEXTURE WAS PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(texture, "Ark piece texture cannot be null.");

        // SET THE TEXTURE RECTANGLE BASED ON THE ID.
        const unsigned int ARK_TILES_PER_ROW = 8;
        const unsigned int ARK_PIECE_DIMENSION = 16;
        unsigned int ark_column_index = id % ARK_TILES_PER_ROW;
        unsigned int texture_left_position = ark_column_index * ARK_PIECE_DIMENSION;
        unsigned int ark_row_index = id / ARK_TILES_PER_ROW;
        unsigned int texture_top_position = ark_row_index * ARK_PIECE_DIMENSION;
        MATH::FloatRectangle texture_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            static_cast<float>(texture_left_position),
            static_cast<float>(texture_top_position),
            static_cast<float>(ARK_PIECE_DIMENSION),
            static_cast<float>(ARK_PIECE_DIMENSION));
        Sprite = GRAPHICS::Sprite(texture, texture_rectangle);
    }
}