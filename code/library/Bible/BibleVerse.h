#pragma once

#include <string>
#include "Bible/BibleBook.h"

namespace BIBLE
{
    /// A Bible verse, with book, chapter, and verse information
    /// in addition to content.
    class BibleVerse
    {
    public:
        // CONSTRUCTION.
        explicit BibleVerse(
            const BibleBook book,
            const unsigned int chapter,
            const unsigned int verse,
            const std::string& text);

        // OPERATORS.
        bool operator< (const BibleVerse& rhs) const;

        // STRING RETRIEVAL.
        std::string GetCitationString() const;
        std::string ToString() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The book the verse comes from.
        BibleBook Book;
        /// The chapter the verse comes from.
        unsigned int Chapter;
        /// The number of the verse within the chapter.
        unsigned int Verse;
        /// The textual content of the verse.
        std::string Text;
    };
}
