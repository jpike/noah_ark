#pragma once

#include <memory>
#include "Math/Vector2.h"

namespace MATH
{
    ///////////////////////////////////////////////////////////
    /// @brief  An interface for objects or components that
    ///         can be transformed using standard mathematical
    ///         transformations such as positioning, rotation,
    ///         or scaling.
    ///////////////////////////////////////////////////////////
    class ITransformable
    {
    public:
        /// @brief  Virtual destructor to allow proper destruction of implementing classes.
        virtual ~ITransformable() {};

        /// @brief      Sets the position component of the transformable.
        ///             The position component will be stored as a shared pointer,
        ///             so multiple transformables should be able to synchronize
        ///             positions by sharing the same position component.
        /// @param[in]  positionComponent - The position component to set.
        /// @throws     std::invalid_argument - Thrown if the provided position component is null.
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent) = 0;
    };
}