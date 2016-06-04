#pragma once

#include <deque>
#include <string>

namespace CORE
{
    /// A class to hold utility methods related to strings.
    ///
    /// It is not intended to be treated like an instantiatable
    /// data type since a custom string class has not yet been
    /// necessary.
    class String
    {
    public:
        /// Splits the provided string into separate words.
        /// Any whitespace character is considered a delimiter.
        /// Punctuation is included in adjacent words.
        /// @param[in]  text - The text to split into words.
        /// @param[in]  max_length_per_word_in_characters - The maximum length (in characters)
        ///     for each word.  If a word is found that exceeds this maximum length, it will
        ///     be broken up into separate words.
        /// @return The words from the provided text, in original order.
        static std::deque<std::string> SplitIntoWords(
            const std::string& text,
            const unsigned int max_length_per_word_in_characters);
    };
}
