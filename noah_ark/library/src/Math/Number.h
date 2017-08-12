#pragma once

namespace MATH
{
    /// Utilities for working with numbers.
    /// Designed with templates to allow working with multiple different data types,
    /// but methods may not work as expected for all data types.
    class Number
    {
    public:
        template <typename NumberType>
        static bool IsEven(const NumberType number);
    };

    /// Determines if a number is even.
    /// @param[in]  number - The number to check.
    /// @return True if the number is even; false otherwise.
    template <typename NumberType>
    bool Number::IsEven(const NumberType number)
    {
        NumberType remainder = (number % 2);
        bool is_even = (0 == remainder);
        return remainder;
    }
}
