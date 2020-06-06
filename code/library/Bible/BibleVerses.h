#pragma once

#include <vector>
#include "Bible/BibleVerse.h"

/// Contains code directly to the Bible.
namespace BIBLE
{
    /// The collection of all Bible verses that can be found during the game.
    extern const std::vector<BibleVerse> BIBLE_VERSES;

    const BIBLE::BibleVerse* FindBibleVerse(
        const BIBLE::BibleBook book,
        const unsigned int chapter,
        const unsigned int verse);
}
