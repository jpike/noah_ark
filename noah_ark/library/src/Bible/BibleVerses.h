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
            const std::string& text) :
        Book(book),
        Chapter(chapter),
        Verse(verse),
        Text(text)
        {}
    };

    /// The collection of all Bible verses that can be found during the game.
    extern const std::vector<BibleVerse> BIBLE_VERSES;
}
