#pragma once

#include <string>

namespace BIBLE
{
    /// Enumerates the different books of the Bible but only
    /// those actually used in the game.
    /// Used to avoid duplication and provide type checking.
    enum class BibleBook
    {
        /// An invalid book enumeration value.
        INVALID,
        /// The book of Genesis.
        GENESIS,
        /// The book of Isaiah.
        ISAIAH, 
        /// The book of Ezekiel.
        EZEKIEL, 
        /// The book of Matthew.
        MATTHEW, 
        /// The book of Luke.
        LUKE, 
        /// The book of Hebrews.
        HEBREWS, 
        /// The book of 1 Peter.
        FIRST_PETER, 
        /// The book of 2 Peter.
        SECOND_PETER 
    };

    std::string ToString(const BibleBook book);
}
