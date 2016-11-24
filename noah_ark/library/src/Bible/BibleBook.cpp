#include "Bible/BibleBook.h"

namespace BIBLE
{
    /// Gets a string for the name of the book.
    /// @param[in]  book - The book of the Bible to get the string name of.
    /// @return The string name for the book.
    std::string ToString(const BibleBook book)
    {
        switch (book)
        {
            case BibleBook::INVALID:
                return "Invalid";
            case BibleBook::GENESIS:
                return "Genesis";
            case BibleBook::ISAIAH:
                return "Isaiah";
            case BibleBook::EZEKIEL:
                return "Ezekiel";
            case BibleBook::MATTHEW:
                return "Matthew";
            case BibleBook::LUKE:
                return "Luke";
            case BibleBook::HEBREWS:
                return "Hebrews";
            case BibleBook::FIRST_PETER:
                return "1 Peter";
            case BibleBook::SECOND_PETER:
                return "2 Peter";
            default:
                return "Unknown";
        }
    }
}
