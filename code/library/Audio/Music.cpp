#include <cstring>
#include "Audio/Music.h"

namespace AUDIO
{
    /// Attempts to load music from in-memory data.
    /// @param[in]  binary_data - The binary music data to try loading from.
    ///     Must be one of the formats supported by SFML
    ///     (https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1InputSoundFile.php#a4e034a8e9e69ca3c33a3f11180250400).
    /// @return The music, if successfully loaded; null otherwise.
    std::unique_ptr<Music> Music::LoadFromMemory(const std::string& binary_data)
    {
        // STORE A COPY OF THE BINARY DATA.
        auto music = std::make_unique<Music>();
        std::size_t music_size_in_bytes = binary_data.size();
        music->AudioData = std::make_unique<uint8_t[]>(music_size_in_bytes);
        std::memcpy(music->AudioData.get(), binary_data.data(), music_size_in_bytes);

        // TRY LOADING THE MUSIC FROM THE RAW DATA.
        bool music_loaded = music->Sfml.openFromMemory(music->AudioData.get(), music_size_in_bytes);
        if (music_loaded)
        {
            return music;
        }
        else
        {
            return nullptr;
        }
    }
}
