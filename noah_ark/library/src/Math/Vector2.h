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
        /// @param[in]  vector_to_copy - The vector to copy values from.
        Vector2(const Vector2& vector_to_copy);

        /// Destructor.
        ~Vector2() {};

        /// Assignment operator.
        /// @param[in]  rhs_vector - The vector on the right-hand side of the assignment.
        /// @return     This vector with values copied from the provided vector.
        Vector2& operator= (const Vector2& rhs_vector);

        /// Equality operator.  Direct equality comparison is used for components,
        /// so the precision of components types should be considered when using
        /// this operator.
        /// @param[in]  rhs_vector - The vector on the right-hand side of the operator.
        /// @return     True if the vectors are equal; false otherwise.
        bool operator== (const Vector2& rhs_vector) const;

        /// Inequality operator.  Direct equality comparison is used for components,
        ///     so the precision of components types should be considered when using
        ///     this operator.
        /// @param[in]  rhs_vector - The vector on the right-hand side of the operator.
        /// @return     True if the vectors are unequal; false otherwise.
        bool operator!= (const Vector2& rhs_vector) const;

        /// Subtraction operator.
        /// @param[in]  rhs - The vector on the right-hand side of the operator to
        ///     subtract from this vector.
        /// @return A new vector created by subtracting the provided vector from this vector.
        Vector2 operator- (const Vector2& rhs) const;

        ComponentType X;    ///< The x component of the vector.  Public for easy access.
        ComponentType Y;    ///< The y component of the vector.  Public for easy access.

    private:
        /// Copies values from the provided vector into this vector.
        /// @param[in]  vector_to_copy - The vector to copy values from.
        void Copy(const Vector2& vector_to_copy);
    };

    // DEFINE COMMON VECTOR2 TYPES.
    /// @brief  A vector composed of 2 unsigned integer components.
    typedef Vector2<unsigned int> Vector2ui;
    /// @brief  A vector composed of 2 float components.
    typedef Vector2<float> Vector2f;

    // CLASS IMPLEMENTATION.

    template <typename ComponentType>
    Vector2<ComponentType>::Vector2(const Vector2<ComponentType>& vector_to_copy) :
        X(),
        Y()
    {
        Copy(vector_to_copy);
    }

    template <typename ComponentType>
    Vector2<ComponentType>& Vector2<ComponentType>::operator= (const Vector2<ComponentType>& rhs_vector)
    {
        // CHECK FOR SELF-ASSIGNMENT.
        bool self_assignment = (this == &rhs_vector);
        if (!self_assignment)
        {
            // Copy the values of the right-hand side vector into this vector.
            Copy(rhs_vector);
        }

        // RETURN THIS OBJECT.
        return (*this);
    }

    template <typename ComponentType>
    bool Vector2<ComponentType>::operator== (const Vector2<ComponentType>& rhs_vector) const
    {
        bool x_component_matches = (this->X == rhs_vector.X);
        bool y_component_matches = (this->Y == rhs_vector.Y);

        bool both_components_match = (x_component_matches && y_component_matches);
        return both_components_match;
    }

    template <typename ComponentType>
    bool Vector2<ComponentType>::operator!= (const Vector2<ComponentType>& rhs_vector) const
    {
        bool vectors_equal = ((*this) == rhs_vector);
        return !vectors_equal;
    }

    template <typename ComponentType>
    Vector2<ComponentType> Vector2<ComponentType>::operator- (const Vector2<ComponentType>& rhs) const
    {
        Vector2<ComponentType> resulting_vector;
        resulting_vector.X = this->X - rhs.X;
        resulting_vector.Y = this->Y - rhs.Y;
        return resulting_vector;
    }

    template <typename ComponentType>
    void Vector2<ComponentType>::Copy(const Vector2<ComponentType>& vector_to_copy)
    {
        // COPY COMPONENTS FROM THE PROVIDED VECTOR.
        this->X = vector_to_copy.X;
        this->Y = vector_to_copy.Y;
    }
}