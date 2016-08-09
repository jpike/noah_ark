#include "Bible/BibleVerse.h"

namespace BIBLE
{
    /// Initializes all fields.
    /// @param[in]  book - The book the verse comes from.
    /// @param[in]  chapter - The chapter the verse comes from.
    /// @param[in]  verse - The number of the verse within its chapter.
    /// @param[in]  text - The textual content of the verse.
    BibleVerse::BibleVerse(
        const BibleBook book,
        const unsigned int chapter,
        const unsigned int verse,
        const std::string& text) :
        Book(book),
        Chapter(chapter),
        Verse(verse),
        Text(text)
    {}

    /// Less than operator to allow sorting Bible verses
    /// by book, chapter, and then verse.
    /// @param[in]   rhs - The Bible verse on the right hand side
    ///     of the less than operator to compare with.
    /// @return True if this Bible verse comes before the provided
    ///     verse; false otherwise.
    bool BibleVerse::operator< (const BibleVerse& rhs) const
    {
        if (Book < rhs.Book) return true;
        if (Book > rhs.Book) return false;

        // The books are equal, so compare chapters.
        if (Chapter < rhs.Chapter) return true;
        if (Chapter > rhs.Chapter) return false;

        // The books and chapters are equal, so compare verse numbers.
        if (Verse < rhs.Verse) return true;

        // This verse is either equal to or greater than right-hand side verse.
        return false;
    }

    /// Gets a string identifying the Bible verse, commonly used
    /// for citation purposes (book, chapter, verse).
    /// @return A citation string identifying the verse.
    std::string BibleVerse::GetCitationString() const
    {
        std::string citation_string =
            BIBLE::ToString(Book) +
            " " +
            std::to_string(Chapter) +
            ":" +
            std::to_string(Verse);
        return citation_string;
    }

    /// Gets the full string representation of the verse for display:
    ///     VerseText - Book Chapter:VerseNumber
    /// @return     The full string representation of the verse.
    std::string BibleVerse::ToString() const
    {
        std::string full_verse_string =
            Text +
            " - " +
            GetCitationString();
        return full_verse_string;
    }
}
