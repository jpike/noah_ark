#pragma once

#include "Bible/BibleVerse.h"
#include "Graphics/Sprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// A present left by an animal that contains a Bible verse.
    /// Mainly just a fun way to have interactions with the animals and collecting of Bible verses.
    class Present
    {
    public:
        // CONSTRUCTION.
        explicit Present(const MATH::Vector2f& center_world_position, const BIBLE::BibleVerse& bible_verse);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the present.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
        /// The Bible verse contained within the present.
        BIBLE::BibleVerse BibleVerse = BIBLE::BibleVerse();
    };
}
