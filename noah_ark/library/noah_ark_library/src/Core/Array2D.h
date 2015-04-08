#pragma once

#include <vector>

namespace CORE
{
    /// A class to simplify interaction with 2D arrays.
    /// While 2D arrays are possible in C++, they are
    /// often more difficult to use than necessary,
    /// requiring more manual memory management or
    /// ugly syntax.  1D arrays can be used in a 2D
    /// manner but require more work to access elements
    /// in an intuitive 2D manner.  This class aims
    /// to overcome these limitations.
    /// @tparam T - The tye of data to store in the array.
    template <typename T>
    class Array2D
    {
    public:
        // CONSTRUCTION.
        /// Constructor.  The array will be filled with default
        /// constructed elements to fill its maximum capacity.
        /// @param[in]  width - The width of the array (number of columns).
        /// @param[in]  height - The height of the array (number of rows).
        explicit Array2D(const unsigned int width, const unsigned int height);

        // DIMENSION ACCESS.
        /// Gets the width (number of columns) in the array.
        /// @return The width of the array.
        unsigned int GetWidth() const;
        /// Gets the height (number of rows) in the array.
        /// @return The height of the array.
        unsigned int GetHeight() const;

        // ELEMENT ACCESS.
        /// Retrieves a reference to the element at the specified 2D coordinates.
        /// operator() is overloaded because it can take multiple parameters,
        /// unlike operator[].
        /// @param[in]  x - The horizontal coordinate (or column) of the element to retrieve.
        /// @param[in]  y - The vertical coordinate (or row) of the element to retrieve.
        /// @return A reference to the element at the specified 2D position.
        /// @throws std::out_of_range - Thrown if the coordinates are out of range
        ///     of the array's bounds.
        T& operator()(const unsigned int x, const unsigned int y);
        /// Retrieves a constant reference to the element at the specified 2D coordinates.
        /// operator() is overloaded because it can take multiple parameters,
        /// unlike operator[].
        /// @param[in]  x - The horizontal coordinate (or column) of the element to retrieve.
        /// @param[in]  y - The vertical coordinate (or row) of the element to retrieve.
        /// @return A constant reference to the element at the specified 2D position.
        /// @throws std::out_of_range - Thrown if the coordinates are out of range
        ///     of the array's bounds.
        const T& operator()(const unsigned int x, const unsigned int y) const;

    private:
        // HELPER METHODS.
        /// Converts the provided 2D coordinates to a 1D array index.
        /// @param[in]  x - The horizontal coordinate (or column) of the element index.
        /// @param[in]  y - The vertical coordinate (or row) of the element index.
        /// @return The 1D array index for the provided 2D coordinates.
        /// @throws std::out_of_range - Thrown if the coordinates are out of range
        ///     of the array's bounds.
        unsigned int Get1DArrayIndex(const unsigned int x, const unsigned int y) const;

        // MEMBER VARIABLES.
        /// The width (number of columns) in the array.
        unsigned int Width;
        /// The height (number of rows) in the array.
        unsigned int Height;
        /// The raw data in the array.  It is stored in 1D format because this
        /// was deemed to be simplest.  Data is stored starting with the top row,
        /// going down to lower rows.  Within each row, each element is stored
        /// from left to right.
        std::vector<T> Data;
    };

    template <typename T>
    Array2D<T>::Array2D(const unsigned int width, const unsigned int height) :
    Width(width),
    Height(height),
    Data(Width * Height)
    {}

    template <typename T>
    unsigned int Array2D<T>::GetWidth() const
    {
        return Width;
    }
    
    template <typename T>
    unsigned int Array2D<T>::GetHeight() const
    {
        return Height;
    }

    template <typename T>
    T& Array2D<T>::operator()(const unsigned int x, const unsigned int y)
    {
        // CONVERT THE 2D INDEX INTO A 1D INDEX.
        unsigned int elementIndex = Get1DArrayIndex(x, y);

        // RETURN THE ELEMENT AT THE SPECIFIED INDEX.
        // The at() method is used because it has bounds-checking and
        // may throw an exception if given an invalid index.
        return Data.at(elementIndex);
    }

    template <typename T>
    const T& Array2D<T>::operator()(const unsigned int x, const unsigned int y) const
    {
        // CONVERT THE 2D INDEX INTO A 1D INDEX.
        unsigned int elementIndex = Get1DArrayIndex(x, y);

        // RETURN THE ELEMENT AT THE SPECIFIED INDEX.
        // The at() method is used because it has bounds-checking and
        // may throw an exception if given an invalid index.
        return Data.at(elementIndex);
    }

    template <typename T>
    unsigned int Array2D<T>::Get1DArrayIndex(const unsigned int x, const unsigned int y) const
    {
        // MAKE SURE THE COORDINATES ARE WITHIN THE ARRAY'S BOUNDS.
        bool xWithinBounds = (x < Width);
        bool yWithinBounds = (y < Height);
        bool coordinatesWithinBounds = (xWithinBounds && yWithinBounds);
        if (!coordinatesWithinBounds)
        {
            throw std::out_of_range("Array2D coordinates out-of-range.");
        }

        // CALCULATE THE INDEX OF THE FIRST ELEMENT IN THE REQUESTED ROW.
        unsigned int rowIndex = y * Width;

        // MOVE OVER TO THE REQUESTED ELEMENT IN THE ROW.
        unsigned int elementIndex = rowIndex + x;

        return elementIndex;
    }
}