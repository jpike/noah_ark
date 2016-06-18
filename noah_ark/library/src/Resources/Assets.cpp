#include <cassert>
#include <cstdint>
#include <future>
#include <vector>
#include "Resources/Assets.h"

namespace RESOURCES
{
    // ASSET IDs.
    // For assets that require accessing the filesystem, IDs are currently set to filepaths
    // (relative to the working directory) to simplify loading.
    const std::string AXE_TEXTURE_ID = "res/images/noah_sprite1.png";
    const std::string DUST_CLOUD_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string FONT_TEXTURE_ID = "res/images/main_font1.png";
    const std::string GROUND_TILESET_TEXTURE_ID = "res/images/ground_tile_set.png";
    const std::string NOAH_TEXTURE_ID = "res/images/noah_sprite1.png";
    const std::string TREE_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string WOOD_LOG_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string DUST_CLOUD_ANIMATION_ID = "dust_cloud";
    const std::string NOAH_WALK_FRONT_ANIMATION_ID = "noah_walk_front";
    const std::string NOAH_WALK_BACK_ANIMATION_ID = "noah_walk_back";
    const std::string NOAH_WALK_LEFT_ANIMATION_ID = "noah_walk_left";
    const std::string NOAH_WALK_RIGHT_ANIMATION_ID = "noah_walk_right";
    const std::string AXE_HIT_SOUND_ID = "res/sounds/axe_tree_hit1.wav";
    const std::string TREE_SHAKE_SOUND_ID = "res/sounds/tree_shake3.wav";

    Assets::Assets() :
    Textures(),
    AudioSamples(),
    OverworldMapFile(),
    TileMapFiles()
    {}

    bool Assets::LoadAll()
    {
        // LOAD TEXTURES.
        bool textures_loaded = LoadTextures();
        if (!textures_loaded)
        {
            return false;
        }

        // LOAD SOUNDS.
        bool sounds_loaded = LoadSounds();
        if (!sounds_loaded)
        {
            return false;
        }

        // LOAD THE OVERWORLD MAP FILE.
        bool overworld_map_file_loaded = LoadOverworldMapFile();
        if (!overworld_map_file_loaded)
        {
            return false;
        }

        // LOAD THE INDIVIDUAL TILE MAP FILES.
        bool tile_maps_loaded = LoadTileMapFiles();
        if (!tile_maps_loaded)
        {
            return false;
        }

        // INDICATE THAT ALL ASSETS WERE LOADED.
        return true;
    }

    std::shared_ptr<MAPS::Tileset> Assets::GetTileset(const std::vector<MAPS::TilesetDescription>& tileset_descriptions)
    {
        /// @todo   Consider caching this? - There's some "name" property in the JSON files.

        // POPULATE A TILESET FROM EACH DESCRIPTION.
        std::shared_ptr<MAPS::Tileset> tileset = std::make_shared<MAPS::Tileset>();
        for (const auto& tileset_description : tileset_descriptions)
        {
            // LOAD THE TEXTURE FOR THE CURRENT DESCRIPTION.
            /// @todo   Mapping rather than hardcoded filepath here!
            std::shared_ptr<GRAPHICS::Texture> tileset_texture = GetTexture(GROUND_TILESET_TEXTURE_ID);
            bool texture_loaded = (nullptr != tileset_texture);
            if (!texture_loaded)
            {
                // SKIP TO THE NEXT TILESET DESCRIPTION.
                continue;
            }

            // CREATE AND STORE EACH TILE IN THE CURRENT DESCRIPTION.
            MAPS::TileId current_tile_id = tileset_description.FirstTileId;
            MATH::Vector2ui tileset_texture_dimensions = tileset_texture->GetSize();
            unsigned int row_count_of_tiles = tileset_texture_dimensions.Y / tileset_description.TileHeightInPixels;
            unsigned int column_count_of_tiles = tileset_texture_dimensions.X / tileset_description.TileWidthInPixels;
            for (unsigned int tile_row_index = 0;
                tile_row_index < row_count_of_tiles;
                ++tile_row_index)
            {
                for (unsigned int tile_column_index = 0;
                    tile_column_index < column_count_of_tiles;
                    ++tile_column_index)
                {
                    // CALCULATE THE OFFSET WITHIN THE TEXTURE FOR THE CURRENT TILE.
                    int tile_left_texture_offset_in_texels = tile_column_index * tileset_description.TileWidthInPixels;
                    int tile_top_texture_offset_in_texels = tile_row_index * tileset_description.TileHeightInPixels;

                    // CREATE A SPRITE FOR THE CURRENT TILE.
                    MATH::FloatRectangle tile_texture_rect = MATH::FloatRectangle::FromTopLeftAndDimensions(
                        static_cast<float>(tile_left_texture_offset_in_texels),
                        static_cast<float>(tile_top_texture_offset_in_texels),
                        static_cast<float>(tileset_description.TileWidthInPixels),
                        static_cast<float>(tileset_description.TileHeightInPixels));

                    // STORE THE CURRENT TILE.
                    tileset->SetTile(current_tile_id, tileset_texture, tile_texture_rect);

                    // UPDATE THE TILE ID FOR THE NEXT TILE.
                    ++current_tile_id;
                }
            }
        }

        return tileset;
    }

    std::shared_ptr<GRAPHICS::Texture> Assets::GetTexture(const std::string& texture_id)
    {
        // RETURN THE TEXTURE IF IT HAS ALREADY BEEN LOADED.
        auto id_with_texture = Textures.find(texture_id);
        bool texture_already_loaded = (Textures.cend() != id_with_texture);
        if (texture_already_loaded)
        {
            return id_with_texture->second;
        }

        // TRY LOADING THE TEXTURE.
        // Texture IDs map directly to filepaths.
        const auto& texture_filepath = texture_id;
        std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::Load(texture_filepath);
        bool texture_loaded = (nullptr != texture);
        if (texture_loaded)
        {
            // CACHE THE TEXTURE IN THIS COLLECTION BEFORE RETURNING.
            Textures[texture_id] = texture;
            return texture;
        }
        else
        {
            // The texture couldn't be retrieved.
            return nullptr;
        }
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> Assets::GetAnimationSequence(const std::string& animation_id)
    {
        if (DUST_CLOUD_ANIMATION_ID == animation_id)
        {
            // RETURN THE ANIMATION SEQUENCE.
            const bool IS_LOOPING = false;
            const sf::Time TOTAL_DURATION = sf::seconds(2.0f);
            const std::vector<MATH::IntRectangle> FRAMES =
            {
                // Frames are duplicated some so that things appear to loop a few times.
                /// @todo   Consider having a max loop count for animation sequences?
                MATH::IntRectangle::FromTopLeftAndDimensions(48, 0, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(48, 16, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(48, 0, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(48, 16, 16, 16)
            };
            const std::shared_ptr<GRAPHICS::AnimationSequence> DUST_CLOUD_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
                DUST_CLOUD_ANIMATION_ID,
                IS_LOOPING,
                TOTAL_DURATION,
                FRAMES);
            return DUST_CLOUD_ANIMATION;
        }
        else if (NOAH_WALK_FRONT_ANIMATION_ID == animation_id)
        {
            // RETURN THE ANIMATION SEQUENCE.
            const bool IS_LOOPING = true;
            const sf::Time TOTAL_DURATION = sf::seconds(0.7f);
            const std::vector<MATH::IntRectangle> FRAMES =
            {
                MATH::IntRectangle::FromTopLeftAndDimensions(0, 0, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(16, 0, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(32, 0, 16, 16)
            };
            const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_FRONT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
                NOAH_WALK_FRONT_ANIMATION_ID,
                IS_LOOPING,
                TOTAL_DURATION,
                FRAMES);
            return NOAH_WALK_FRONT_ANIMATION;
        }
        else if (NOAH_WALK_BACK_ANIMATION_ID == animation_id)
        {
            // RETURN THE ANIMATION SEQUENCE.
            const bool IS_LOOPING = true;
            const sf::Time TOTAL_DURATION = sf::seconds(0.7f);
            const std::vector<MATH::IntRectangle> FRAMES = 
            {
                MATH::IntRectangle::FromTopLeftAndDimensions(0, 16, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(16, 16, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(32, 16, 16, 16)
            };
            const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_BACK_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
                NOAH_WALK_BACK_ANIMATION_ID,
                IS_LOOPING,
                TOTAL_DURATION,
                FRAMES);
            return NOAH_WALK_BACK_ANIMATION;
        }
        else if (NOAH_WALK_LEFT_ANIMATION_ID == animation_id)
        {
            // RETURN THE ANIMATION SEQUENCE.
            const bool IS_LOOPING = true;
            const sf::Time TOTAL_DURATION = sf::seconds(0.7f);
            const std::vector<MATH::IntRectangle> FRAMES =
            {
                MATH::IntRectangle::FromTopLeftAndDimensions(0, 32, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(16, 32, 16, 16)
            };
            const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_LEFT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
                NOAH_WALK_LEFT_ANIMATION_ID,
                IS_LOOPING,
                TOTAL_DURATION,
                FRAMES);
            return NOAH_WALK_LEFT_ANIMATION;
        }
        else if (NOAH_WALK_RIGHT_ANIMATION_ID == animation_id)
        {
            // RETURN THE ANIMATION SEQUENCE.
            const bool IS_LOOPING = true;
            const sf::Time TOTAL_DURATION = sf::seconds(0.7f);
            const std::vector<MATH::IntRectangle> FRAMES =
            {
                MATH::IntRectangle::FromTopLeftAndDimensions(0, 48, 16, 16),
                MATH::IntRectangle::FromTopLeftAndDimensions(16, 48, 16, 16)
            };
            const std::shared_ptr<GRAPHICS::AnimationSequence> NOAH_WALK_RIGHT_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
                NOAH_WALK_RIGHT_ANIMATION_ID,
                IS_LOOPING,
                TOTAL_DURATION,
                FRAMES);
            return NOAH_WALK_RIGHT_ANIMATION;
        }
        else
        {
            /// @todo   Logging?  Right now, we're just making things fail
            /// easily due to an supported animation being specified.
            assert(false);
            return nullptr;
        }
    }

    std::shared_ptr<GRAPHICS::GUI::Font> Assets::GetFont(const std::string& font_texture_id)
    {
        // GET THE FONT'S TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> font_texture = GetTexture(font_texture_id);
        bool font_texture_loaded = (nullptr != font_texture);
        if (font_texture_loaded)
        {
            // RETURN THE FONT.
            /// @todo Should we store loaded fonts somewhere instead of creating new instances each time?
            std::shared_ptr<GRAPHICS::GUI::Font> font = std::make_shared<GRAPHICS::GUI::Font>(font_texture);
            return font;
        }
        else
        {
            // INDICATE THAT THE FONT COULD NOT BE LOADED.
            return nullptr;
        }
    }

    std::shared_ptr<AUDIO::SoundEffect> Assets::GetSound(const std::string& sound_id)
    {
        // CHECK IF THE AUDIO SAMPLES HAVE ALREADY BEEN LOADED.
        auto id_with_audio_samples = AudioSamples.find(sound_id);
        bool audio_samples_already_loaded = (AudioSamples.cend() != id_with_audio_samples);
        if (audio_samples_already_loaded)
        {
            // RETURN THE SOUND EFFECT USING THE PREVIOUSLY LOADED AUDIO SAMPLES.
            std::shared_ptr<AUDIO::SoundEffect> sound_effect = std::make_shared<AUDIO::SoundEffect>(id_with_audio_samples->second);
            return sound_effect;
        }

        // LOAD THE AUDIO SAMPLES INTO A BUFFER.
        // The sound ID maps directly to a filepath.
        std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
        bool audio_samples_loaded = sound_buffer->loadFromFile(sound_id);
        if (!audio_samples_loaded)
        {
            return nullptr;
        }

        // SAVE THE SOUND TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        AudioSamples[sound_id] = sound_buffer;

        // RETURN THE SOUND EFFECT.
        std::shared_ptr<AUDIO::SoundEffect> sound_effect = std::make_shared<AUDIO::SoundEffect>(sound_buffer);
        return sound_effect;
    }

    bool Assets::LoadTextures()
    {
        // CLEAR ANY PREVIOUSLY LOADED TEXTURES.
        Textures.clear();

        // DEFINE THE TEXTURES TO BE LOADED.
        const std::vector<std::string> TEXTURE_IDS =
        {
            AXE_TEXTURE_ID,
            FONT_TEXTURE_ID,
            GROUND_TILESET_TEXTURE_ID,
            NOAH_TEXTURE_ID,
            TREE_TEXTURE_ID,
            DUST_CLOUD_TEXTURE_ID,
            WOOD_LOG_TEXTURE_ID
        };

        // LOAD ALL OF THE TEXTURES.
        // Texture IDs specify filepaths.
        for (const auto& texture_id : TEXTURE_IDS)
        {
            // CHECK IF THE PROVIDED TEXTURE HAS ALREADY BEEN LOADED.
            // Certain textures may use the same ID.
            auto id_with_texture = Textures.find(texture_id);
            bool texture_already_loaded = (Textures.cend() != id_with_texture);
            if (texture_already_loaded)
            {
                // Skip to the next texture to avoid wasting time loading it again.
                continue;
            }

            // LOAD THE CURRENT TEXTURE.
            // Texture IDs map directly to filepaths.
            const auto& texture_filepath = texture_id;
            std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::Load(texture_filepath);
            bool texture_loaded = (nullptr != texture);
            if (texture_loaded)
            {
                // STORE THE TEXTURE IN THIS COLLECTION.
                Textures[texture_id] = texture;
            }
            else
            {
                // INDICATE THAT A TEXTURE FAILED TO LOAD.
                return false;
            }
        }

        // INDICATE THAT ALL TEXTURES WERE LOADED.
        return true;
    }

    bool Assets::LoadSounds()
    {
        // CLEAR ANY PREVIOUSLY LOADED AUDIO SAMPLES.
        AudioSamples.clear();

        // DEFINE THE AUDIO SAMPLES TO BE LOADED.
        const std::vector<std::string> SOUND_IDS =
        {
            AXE_HIT_SOUND_ID,
            TREE_SHAKE_SOUND_ID
        };

        // LOAD ALL OF THE AUDIO SAMPLES.
        for (const auto& sound_id : SOUND_IDS)
        {
            // LOAD THE CURRENT AUDIO SAMPLES.
            // Sound IDs map directly to filepaths.
            std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
            bool audio_samples_loaded = sound_buffer->loadFromFile(sound_id);
            if (audio_samples_loaded)
            {
                // STORE THE AUDIO SAMPLE BUFFER IN THIS COLLECTION.
                AudioSamples[sound_id] = sound_buffer;
            }
            else
            {
                // INDICATE THAT A SOUND FAILED TO LOAD.
                return false;
            }
        }

        // INDICATE THAT ALL SOUNDS WERE LOADED.
        return true;
    }
    
    bool Assets::LoadOverworldMapFile()
    {
        // CLEAR ANY PREVIOUSLY LOADED OVERWORLD MAP FILE.
        OverworldMapFile = nullptr;
        
        // LOAD THE OVERWORLD MAP FILE.
        const std::string OVERWORLD_MAP_FILEPATH = "res/maps/overworld_map.json";
        OverworldMapFile = MAPS::OverworldMapFile::Load(OVERWORLD_MAP_FILEPATH);
        
        // INDICATE IF LOADING WAS SUCCESSFUL.
        bool overworld_map_file_loaded = (nullptr != OverworldMapFile);
        return overworld_map_file_loaded;
    }
    
    bool Assets::LoadTileMapFiles()
    {
        // CLEAR ANY PREVIOUSLY LOADED TILE MAP FILES.
        // Resizing is done to allocate enough space for the new tile map files
        // while clearing old data at the same time.
        unsigned int tile_map_row_count = OverworldMapFile->OverworldHeightInTileMaps;
        unsigned int tile_map_column_count = OverworldMapFile->OverworldWidthInTileMaps;
        TileMapFiles.Resize(tile_map_column_count, tile_map_row_count);

        // DEFINE AN ARRAY OF FUTURES FOR LOADING THE TILE MAPS IN PARALLEL.
        // Asynchronous loading is done since it is a little faster than synchronous loading
        // (about 7-8 seconds of total asset load time as opposed to 12-13 seconds total).
        /// @todo   Determine if this asynchronous loading is worth it.  In the final
        /// game, we'll probably have an intro sequence playing before the title screen
        /// where this time for loading tile maps won't matter so much.
        CORE::Array2D< std::future< std::unique_ptr<MAPS::TileMapFile> > > tile_map_loaders(
            tile_map_column_count,
            tile_map_row_count);

        // START TO LOAD ALL TILE MAP FILES IN THE OVERWORLD MAP FILE.
        for (unsigned int tile_map_row = 0; tile_map_row < tile_map_row_count; ++tile_map_row)
        {
            for (unsigned int tile_map_column = 0; tile_map_column < tile_map_column_count; ++tile_map_column)
            {
                // STARTING LOADING THE CURRENT TILE MAP.
                std::string tile_map_filepath = OverworldMapFile->GetTileMapFilepath(tile_map_row, tile_map_column);
                tile_map_loaders(tile_map_column, tile_map_row) = std::async(MAPS::TileMapFile::Load, tile_map_filepath);
            }
        }

        // OBTAIN ALL OF THE TILE MAP FILES BEING LOADED ASYNCHRONOUSLY.
        for (unsigned int tile_map_row = 0; tile_map_row < tile_map_row_count; ++tile_map_row)
        {
            for (unsigned int tile_map_column = 0; tile_map_column < tile_map_column_count; ++tile_map_column)
            {
                try
                {
                    // CHECK IF THE TILE MAP LOADER FOR THE CURRENT ROW/COLUMN IS VALID.
                    auto& tile_map_loader = tile_map_loaders(tile_map_column, tile_map_row);
                    if (tile_map_loader.valid())
                    {
                        // WAIT AND GET THE LOADED TILE MAP FILE.
                        std::unique_ptr<MAPS::TileMapFile> tile_map_file = tile_map_loader.get();
                        bool tile_map_file_loaded = (nullptr != tile_map_file);
                        if (tile_map_file_loaded)
                        {
                            // STORE THE TILE MAP FILE IN THIS COLLECTION OF ASSETS.
                            TileMapFiles(tile_map_column, tile_map_row) = std::move(tile_map_file);
                        }
                        else
                        {
                            assert(false);
                            // All tile map files could not be loaded.
                            return false;
                        }
                    }
                }
                catch (const std::exception&)
                {
                    assert(false);
                    // All tile map files could not be loaded.
                    return false;
                }
            }
        }

        // All tile map files were loaded successfully.
        return true;
    }
}