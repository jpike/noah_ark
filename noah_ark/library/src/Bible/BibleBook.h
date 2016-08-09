#pragma once

#include <string>

namespace BIBLE
{
    /// Enumerates the different books of the Bible but only
    /// those actually used in the game.
    /// Used to avoid duplication and provide type checking.
    enum class BibleBook
    {
        INVALID, ///< An invalid book enumeration value.
        GENESIS, ///< The book of Genesis.
        ISAIAH, ///< The book of Isaiah.
        EZEKIEL, ///< The book of Ezekiel.
        MATTHEW, ///< The book of Matthew.
        LUKE, ///< The book of Luke.
        HEBREWS, ///< The book of Hebrews.
        FIRST_PETER, ///< The book of 1 Peter.
        SECOND_PETER ///< The book of 2 Peter.
    };

    std::string ToString(const BibleBook book);
}
