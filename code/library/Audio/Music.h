#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

namespace AUDIO
{
    /// Longer musical compositions.
    class Music
    {
    public:
        // CONSTRUCTION.
        static std::unique_ptr<Music> LoadFromMemory(const std::string& binary_data);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The underlying music from the SFML library.
        sf::Music Sfml = {};
        /// The underlying audio data.  Must remain in-memory due to how SFML works
        /// (https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Music.php#ae93b21bcf28ff0b5fec458039111386e).
        std::unique_ptr<uint8_t[]> AudioData = nullptr;
    };
}
