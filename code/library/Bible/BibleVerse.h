#pragma once

#include <compare>
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
        explicit BibleVerse() = default;
        explicit BibleVerse(
            const BibleBook book,
            const unsigned int chapter,
            const unsigned int verse,
            const std::string& text);

        // OPERATORS.
        auto operator<=>(const BibleVerse&) const = default;
        bool operator<(const BibleVerse& rhs) const;

        // STRING RETRIEVAL.
        std::string GetCitationString() const;
        std::string ToString() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The book the verse comes from.
        BibleBook Book = BibleBook::INVALID;
        /// The chapter the verse comes from.
        unsigned int Chapter = 0;
        /// The number of the verse within the chapter.
        unsigned int Verse = 0;
        /// The textual content of the verse.
        std::string Text = "";
    };
}
