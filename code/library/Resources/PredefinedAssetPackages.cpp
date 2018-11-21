#include "Resources/PredefinedAssetPackages.h"

namespace RESOURCES
{
    const std::string INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME = "IntroAssets.package";

    const AssetPackageDefinition INTRO_SEQUENCE_ASSET_PACKAGE_DEFINITION({
        AssetDefinition(AssetType::FONT, RESOURCES::AssetId::SERIF_FONT_TEXTURE, "res/images/main_font_serif.png"),
        AssetDefinition(AssetType::MUSIC, RESOURCES::AssetId::INTRO_MUSIC, "res/sounds/NoahsArkIntro1_80PercentSpeedWithFadeout.wav"),
        AssetDefinition(AssetType::SHADER, RESOURCES::AssetId::COLORED_TEXTURE_SHADER, "res/shaders/ColoredTexture.frag"),
    });

    const std::string MAIN_ASSET_PACKAGE_FILENAME = "MainAssets.package";

    const AssetPackageDefinition MAIN_ASSET_PACKAGE_DEFINITION({
        // FONTS.
        AssetDefinition(AssetType::FONT, RESOURCES::AssetId::FONT_TEXTURE, "res/images/main_font1.png"),

        // OTHER TEXTURES.
        AssetDefinition(AssetType::TEXTURE, AssetId::ANIMAL_TEXTURE, "res/images/animal_sprites.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::ARK_TEXTURE, "res/images/ark_tile_set.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::AXE_TEXTURE, "res/images/noah_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::DUST_CLOUD_TEXTURE, "res/images/tree_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FOOD_TEXTURE, "res/images/food_sprites.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::MAIN_TILESET_TEXTURE, "res/images/main_tile_set.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::NOAH_TEXTURE, "res/images/noah_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::TREE_TEXTURE, "res/images/tree_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::WOOD_LOG_TEXTURE, "res/images/tree_sprite1.png"),

        // GENERAL SOUND EFFECTS.
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::ARK_BUILDING_SOUND, "res/sounds/ark_build_sound1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::AXE_HIT_SOUND, "res/sounds/axe_tree_hit1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::COLLECT_BIBLE_VERSE_SOUND, "res/sounds/collect_bible_verse1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FOOD_PICKUP_SOUND, "res/sounds/food_pickup_sound.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::TREE_SHAKE_SOUND, "res/sounds/tree_shake4.wav"),

        // ANIMAL SOUND EFFECTS.
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::ALLIGATOR_SOUND, "res/sounds/baligator5.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BABOON_SOUND, "res/sounds/baboon2.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BAT_SOUND, "res/sounds/BatsVocalizing-SoundBible.com-1162895834.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BEAR_SOUND, "res/sounds/Bear-SoundBible.com-866204188.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BEE_SOUND, "res/sounds/Bee-SoundBible.com-914644763.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BOBCAT_SOUND, "res/sounds/BOB-CAT.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BUTTERFLY_SOUND, "res/sounds/Butterfly-SoundBible.com-1530156556.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CAMEL_SOUND, "res/sounds/camel5.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CAT_SOUND, "res/sounds/CatMeow-SoundBible.com-1453940411.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CATTLE_SOUND, "res/sounds/SingleCow-SoundBible.com-2051754137.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHEETAH_SOUND, "res/sounds/Cheetah5.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHICKEN_SOUND, "res/sounds/Chicken-SoundBible.com-744739895.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHIPMUNK_SOUND, "res/sounds/AngryChipmunk-SoundBible.com-980210050.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DEER_SOUND, "res/sounds/Elk-SoundBible.com-734134354.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DOG_SOUND, "res/sounds/doberman-pincher_daniel-simion.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DOVE_SOUND, "res/sounds/dove-Mike_Koenig-1208819046.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::EAGLE_SOUND, "res/sounds/BaldEagle-SoundBible.com-2025808531.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FOX_SOUND, "res/sounds/greyfox-vs-coon.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FROG_SOUND, "res/sounds/FrogCroaking-SoundBible.com-1053984354.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GOAT_SOUND, "res/sounds/GoatBah-SoundBible.com-959734934.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GORILLA_SOUND, "res/sounds/Ape-SoundBible.com-1651128585.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HAWK_SOUND, "res/sounds/hawk_screeching-Mike_Koenig-1626170357.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HIPPO_SOUND, "res/sounds/hippo2.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HORSE_SOUND, "res/sounds/HorseNeigh-SoundBible.com-1740540960.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::KOALA_SOUND, "res/sounds/koala1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LEOPARD_SOUND, "res/sounds/leopard3.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LION_SOUND, "res/sounds/GrowlingLion-SoundBible.com-495747737.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LOCUST_SOUND, "res/sounds/Grasshopper-SoundBible.com-1592176719.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::MONKEY_SOUND, "res/sounds/gibbon-monkey-daniel_simon.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::OSPREY_SOUND, "res/sounds/OspreyBirdCall-SoundBible.com-525395846.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::OWL_SOUND, "res/sounds/horned_owl-Mike_Koenig-1945374932.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PEACOCK_SOUND, "res/sounds/PeacockBird-SoundBible.com-98379922.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PELICAN_SOUND, "res/sounds/PelicanChick-SoundBible.com-549990839.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PIG_SOUND, "res/sounds/PigOink-SoundBible.com-1424738584.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::POLAR_BEAR_SOUND, "res/sounds/bear_polar.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::RACCOON_SOUND, "res/sounds/Raccoon-J_Dawg-688727657.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::RAVEN_SOUND, "res/sounds/Raven-SoundBible.com-1790882934.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::SHEEP_SOUND, "res/sounds/SheepBleating-SoundBible.com-1373580685.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::TURKEY_SOUND, "res/sounds/Turkey_Call_2-JimBob-26023545.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::WOLF_SOUND, "res/sounds/Howl.wav"),

        // MUSIC.
        AssetDefinition(AssetType::MUSIC, AssetId::OVERWORLD_BACKGROUND_MUSIC, "res/sounds/overworld_music.wav"),
    });
}
