#pragma once

/// A namespace to hold math utilities.
namespace MATH
{
    /// A 2D mathematical vector.  It currently only has the minimal functionality needed,
    /// so it cannot directly perform all common vector operations.
    ///
    /// The ComponentType template parameter is intended to be replaced with
    /// any numerical type that is typically used for vectors (int, float, etc.).
    template <typename ComponentType>
    class Vector2
    {
    public:
        /// Default constructor.
        explicit Vector2() : X(), Y() {};

        /// Constructor that accepts initial values.
        /// @param[in]  x - The x component value.
        /// @param[in]  y - The y component value.
        explicit Vector2(const ComponentType x, const ComponentType y) : X(x), Y(y) {};

        /// Copy constructor.
        /// @param[in]  vectorToCopy - The vector to copy values from.
        Vector2(const Vector2& vectorToCopy);

        /// Destructor.
        ~Vector2() {};

        /// Assignment operator.
        /// @param[in]  rhsVector - The vector on the right-hand side of the assignment.
        /// @return     This vector with values copied from the provided vector.
        Vector2& operator= (const Vector2& rhsVector);

        /// Equality operator.  Direct equality comparison is used for components,
        /// so the precision of components types should be considered when using
        /// this operator.
        /// @param[in]  rhsVector - The vector on the right-hand side of the operator.
        /// @return     True if the vectors are equal; false otherwise.
        bool operator== (const Vector2& rhsVector) const;

        /// Inequality operator.  Direct equality comparison is used for components,
        ///     so the precision of components types should be considered when using
        ///     this operator.
        /// @param[in]  rhsVector - The vector on the right-hand side of the operator.
        /// @return     True if the vectors are unequal; false otherwise.
        bool operator!= (const Vector2& rhsVector) const;

        /// Subtraction operator.
        /// @param[in]  rhs - The vector on the right-hand side of the operator to
        ///     subtract from this vector.
        /// @return A new vector created by subtracting the provided vector from this vector.
        Vector2 operator- (const Vector2& rhs) const;

        ComponentType X;    ///< The x component of the vector.  Public for easy access.
        ComponentType Y;    ///< The y component of the vector.  Public for easy access.

    private:
        /// Copies values from the provided vector into this vector.
        /// @param[in]  vectorToCopy - The vector to copy values from.
        void Copy(const Vector2& vectorToCopy);
    };

    // DEFINE COMMON VECTOR2 TYPES.
    /// @brief  A vector composed of 2 unsigned integer components.
    typedef Vector2<unsigned int> Vector2ui;
    /// @brief  A vector composed of 2 float components.
    typedef Vector2<float> Vector2f;

    // CLASS IMPLEMENTATION.

    template <typename ComponentType>
    Vector2<ComponentType>::Vector2(const Vector2<ComponentType>& vectorToCopy) :
        X(),
        Y()
    {
        Copy(vectorToCopy);
    }

    template <typename ComponentType>
    Vector2<ComponentType>& Vector2<ComponentType>::operator= (const Vector2<ComponentType>& rhsVector)
    {
        // CHECK FOR SELF-ASSIGNMENT.
        bool selfAssignment = (this == &rhsVector);
        if (!selfAssignment)
        {
            // Copy the values of the right-hand side vector into this vector.
            Copy(rhsVector);
        }

        // RETURN THIS OBJECT.
        return (*this);
    }

    template <typename ComponentType>
    bool Vector2<ComponentType>::operator== (const Vector2<ComponentType>& rhsVector) const
    {
        bool xComponentMatches = (this->X == rhsVector.X);
        bool yComponentMatches = (this->Y == rhsVector.Y);

        bool bothComponentsMatch = (xComponentMatches && yComponentMatches);
        return bothComponentsMatch;
    }

    template <typename ComponentType>
    bool Vector2<ComponentType>::operator!= (const Vector2<ComponentType>& rhsVector) const
    {
        bool vectorsEqual = ((*this) == rhsVector);
        return !vectorsEqual;
    }

    template <typename ComponentType>
    Vector2<ComponentType> Vector2<ComponentType>::operator- (const Vector2<ComponentType>& rhs) const
    {
        MATH::Vector2f resulting_vector;
        resulting_vector.X = this->X - rhs.X;
        resulting_vector.Y = this->Y - rhs.Y;
        return resulting_vector;
    }

    template <typename ComponentType>
    void Vector2<ComponentType>::Copy(const Vector2<ComponentType>& vectorToCopy)
    {
        // COPY COMPONENTS FROM THE PROVIDED VECTOR.
        this->X = vectorToCopy.X;
        this->Y = vectorToCopy.Y;
    }
}