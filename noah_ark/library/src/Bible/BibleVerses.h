#pragma once

#include <string>

/// Contains code directly to the Bible.
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

    /// Gets a string for the name of the book.
    /// @param[in]  book - The book of the Bible to get the string name of.
    /// @return The string name for the book.
    std::string ToString(const BibleBook book);

    /// A Bible verse, with book, chapter, and verse information
    /// in addition to content.
    struct BibleVerse
    {
        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The book the verse comes from.
        BibleBook Book;
        /// The chapter the verse comes from.
        unsigned int Chapter;
        /// The number of the verse within the chapter.
        unsigned int Verse;
        /// The textual content of the verse.
        std::string Text;

        // CONSTRUCTION.
        /// Initializes all fields.
        /// @param[in]  book - The book the verse comes from.
        /// @param[in]  chapter - The chapter the verse comes from.
        /// @param[in]  verse - The number of the verse within its chapter.
        /// @param[in]  text - The textual content of the verse.
        explicit BibleVerse(
            const BibleBook book,
            const unsigned int chapter,
            const unsigned int verse,
            const std::string& text);

        // OPERATORS.
        /// Less than operator to allow sorting Bible verses
        /// by book, chapter, and then verse.
        /// @param[in]   rhs - The Bible verse on the right hand side
        ///     of the less than operator to compare with.
        /// @return True if this Bible verse comes before the provided
        ///     verse; false otherwise.
        bool operator< (const BibleVerse& rhs) const;

        // STRING RETRIEVAL.
        /// Gets a string identifying the Bible verse, commonly used
        /// for citation purposes (book, chapter, verse).
        /// @return A citation string identifying the verse.
        std::string GetCitationString() const;

        /// Gets the full string representation of the verse for display:
        ///     VerseText - Book Chapter:VerseNumber
        /// @return     The full string representation of the verse.
        std::string ToString() const;
    };

    /// The collection of all Bible verses that can be found during the game.
    extern const std::vector<BibleVerse> BIBLE_VERSES;
}
