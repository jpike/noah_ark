#pragma once

#include <string>
#include <unordered_map>

namespace RESOURCES
{
    /// Defines unique IDs for all assets in the game.
    enum class AssetId
    {
        /// Reserved to indicate an invalid or non-existent asset.
        INVALID,

        // FONT TEXTURES.
        /// The main font texture.
        FONT_TEXTURE,
        /// A serif font texture.
        SERIF_FONT_TEXTURE,

        // OTHER TEXTURES.
        /// The texture with animal sprites.
        ANIMAL_TEXTURE,
        /// The texture with ark tiles.
        ARK_TEXTURE,
        /// The texture with axe sprites.
        AXE_TEXTURE,
        /// The texture with dust cloud sprites.
        DUST_CLOUD_TEXTURE,
        /// The texture with food sprites.
        FOOD_TEXTURE,
        /// The main tileset texture.
        MAIN_TILESET_TEXTURE,
        /// The texture with Noah sprites.
        NOAH_TEXTURE,
        /// The texture with tree sprites.
        TREE_TEXTURE,
        /// The texture with wood logs.
        WOOD_LOG_TEXTURE,

        // SHADERS.
        /// A shader that tints a texture based on a color parameter.
        COLORED_TEXTURE_SHADER,
        /// A shader that tints an image based on the time of day.
        TIME_OF_DAY_SHADER,

        // GENERAL SOUND EFFECTS.
        /// The sound that plays when building a piece of the ark.
        ARK_BUILDING_SOUND,
        /// The sounds that plays when hitting a tree with an axe.
        AXE_HIT_SOUND,
        /// The sound that plays when collecting a Bible verse.
        COLLECT_BIBLE_VERSE_SOUND,
        /// The sound that plays when picking up food.
        FOOD_PICKUP_SOUND,
        /// The sound that plays when a tree is shaking from being hit with an axe.
        TREE_SHAKE_SOUND,

        // ANIMAL SOUND EFFECTS.
        ALLIGATOR_SOUND,
        BABOON_SOUND,
        BAT_SOUND,
        BEAR_SOUND,
        BEE_SOUND,
        BOBCAT_SOUND,
        BUTTERFLY_SOUND,
        CAMEL_SOUND,
        CAT_SOUND,
        CATTLE_SOUND,
        CHEETAH_SOUND,
        CHICKEN_SOUND,
        CHIPMUNK_SOUND,
        CRICKET_SOUND,
        DEER_SOUND,
        DOG_SOUND,
        DOVE_SOUND,
        DUCK_SOUND,
        EAGLE_SOUND,
        ELEPHANT_SOUND,
        FOX_SOUND,
        FROG_SOUND,
        GOAT_SOUND,
        GOOSE_SOUND,
        GORILLA_SOUND,
        GUINEA_PIG_SOUND,
        HAWK_SOUND,
        HIPPO_SOUND,
        HORSE_SOUND,
        HYENA_SOUND,
        KOALA_SOUND,
        LEOPARD_SOUND,
        LION_SOUND,
        LOCUST_SOUND,
        MONKEY_SOUND,
        MOSQUITO_SOUND,
        OSPREY_SOUND,
        OWL_SOUND,
        PEACOCK_SOUND,
        PELICAN_SOUND,
        PIG_SOUND,
        POLAR_BEAR_SOUND,
        RACCOON_SOUND,
        RAVEN_SOUND,
        SEAGULL_SOUND,
        SHEEP_SOUND,
        SNAKE_SOUND,
        SQUIRREL_SOUND,
        TURKEY_SOUND,
        WOLF_SOUND,

        // MUSIC.
        /// The intro music.
        INTRO_MUSIC,
        /// The intro music when starting a new game (save file).
        NEW_GAME_INTRO_MUSIC,
        /// The overworld background music.
        OVERWORLD_BACKGROUND_MUSIC,
    };

    /// A mapping of asset IDs to filepaths.
    /// Not all assets have files, so if an entry doesn't exist for a given
    /// asset ID, then it does not have a corresponding file.
    extern const std::unordered_map<AssetId, std::string> ASSET_FILEPATHS_BY_ID;
}
