#include "Resources/AssetId.h"

namespace RESOURCES
{
    const std::unordered_map<AssetId, std::string> ASSET_FILEPATHS_BY_ID = 
    {
        // FONT TEXTURES.
        { AssetId::FONT_TEXTURE, "res/images/main_font1.png" },

        // OTHER TEXTURES.
        { AssetId::ANIMAL_TEXTURE, "res/images/animal_sprites.png" },
        { AssetId::ARK_TEXTURE, "res/images/ark_tile_set.png" },
        { AssetId::AXE_TEXTURE, "res/images/noah_sprite1.png" },
        { AssetId::DUST_CLOUD_TEXTURE, "res/images/tree_sprite1.png" },
        { AssetId::FOOD_TEXTURE, "res/images/food_sprites.png" },
        { AssetId::MAIN_TILESET_TEXTURE, "res/images/main_tile_set.png" },
        { AssetId::NOAH_TEXTURE, "res/images/noah_sprite1.png" },
        { AssetId::TREE_TEXTURE, "res/images/tree_sprite1.png" },
        { AssetId::WOOD_LOG_TEXTURE, "res/images/tree_sprite1.png" },

        // GENERAL SOUND EFFECTS.
        { AssetId::ARK_BUILDING_SOUND, "res/sounds/ark_build_sound1.wav" },
        { AssetId::AXE_HIT_SOUND, "res/sounds/axe_tree_hit1.wav" },
        { AssetId::COLLECT_BIBLE_VERSE_SOUND, "res/sounds/collect_bible_verse1.wav" },
        { AssetId::FOOD_PICKUP_SOUND, "res/sounds/food_pickup_sound.wav" },
        { AssetId::TREE_SHAKE_SOUND, "res/sounds/tree_shake4.wav" },

        // ANIMAL SOUND EFFECTS.
        { AssetId::ALLIGATOR_SOUND, "res/sounds/baligator5.wav" },
        { AssetId::BABOON_SOUND, "res/sounds/baboon2.wav" },
        { AssetId::BAT_SOUND, "res/sounds/BatsVocalizing-SoundBible.com-1162895834.wav" },
        { AssetId::BEAR_SOUND, "res/sounds/Bear-SoundBible.com-866204188.wav" },
        { AssetId::BEE_SOUND, "res/sounds/Bee-SoundBible.com-914644763.wav" },
        { AssetId::BOBCAT_SOUND, "res/sounds/BOB-CAT.wav" },
        { AssetId::BUTTERFLY_SOUND, "res/sounds/Butterfly-SoundBible.com-1530156556.wav" },
        { AssetId::CAMEL_SOUND, "res/sounds/camel5.wav" },
        { AssetId::CAT_SOUND, "res/sounds/CatMeow-SoundBible.com-1453940411.wav" },
        { AssetId::CATTLE_SOUND, "res/sounds/SingleCow-SoundBible.com-2051754137.wav" },
        { AssetId::CHEETAH_SOUND, "res/sounds/Cheetah5.wav" },
        { AssetId::CHICKEN_SOUND, "res/sounds/Chicken-SoundBible.com-744739895.wav" },
        { AssetId::CHIPMUNK_SOUND, "res/sounds/AngryChipmunk-SoundBible.com-980210050.wav" },
        { AssetId::DEER_SOUND, "res/sounds/Elk-SoundBible.com-734134354.wav" },
        { AssetId::DOG_SOUND, "res/sounds/doberman-pincher_daniel-simion.wav" },
        { AssetId::DOVE_SOUND, "res/sounds/dove-Mike_Koenig-1208819046.wav" },
        { AssetId::EAGLE_SOUND, "res/sounds/BaldEagle-SoundBible.com-2025808531.wav" },
        { AssetId::FOX_SOUND, "res/sounds/greyfox-vs-coon.wav" },
        { AssetId::FROG_SOUND, "res/sounds/FrogCroaking-SoundBible.com-1053984354.wav" },
        { AssetId::GOAT_SOUND, "res/sounds/GoatBah-SoundBible.com-959734934.wav" },
        { AssetId::GORILLA_SOUND, "res/sounds/Ape-SoundBible.com-1651128585.wav" },
        { AssetId::HAWK_SOUND, "res/sounds/hawk_screeching-Mike_Koenig-1626170357.wav" },
        { AssetId::HIPPO_SOUND, "res/sounds/hippo2.wav" },
        { AssetId::HORSE_SOUND, "res/sounds/HorseNeigh-SoundBible.com-1740540960.wav" },
        { AssetId::KOALA_SOUND, "res/sounds/koala1.wav" },
        { AssetId::LEOPARD_SOUND, "res/sounds/leopard3.wav" },
        { AssetId::LION_SOUND, "res/sounds/GrowlingLion-SoundBible.com-495747737.wav" },
        { AssetId::LOCUST_SOUND, "res/sounds/Grasshopper-SoundBible.com-1592176719.wav" },
        { AssetId::MONKEY_SOUND, "res/sounds/gibbon-monkey-daniel_simon.wav" },
        { AssetId::OSPREY_SOUND, "res/sounds/OspreyBirdCall-SoundBible.com-525395846.wav" },
        { AssetId::OWL_SOUND, "res/sounds/horned_owl-Mike_Koenig-1945374932.wav" },
        { AssetId::PEACOCK_SOUND, "res/sounds/PeacockBird-SoundBible.com-98379922.wav" },
        { AssetId::PELICAN_SOUND, "res/sounds/PelicanChick-SoundBible.com-549990839.wav" },
        { AssetId::PIG_SOUND, "res/sounds/PigOink-SoundBible.com-1424738584.wav" },
        { AssetId::POLAR_BEAR_SOUND, "res/sounds/bear_polar.wav" },
        { AssetId::RACCOON_SOUND, "res/sounds/Raccoon-J_Dawg-688727657.wav" },
        { AssetId::RAVEN_SOUND, "res/sounds/Raven-SoundBible.com-1790882934.wav" },
        { AssetId::SHEEP_SOUND, "res/sounds/SheepBleating-SoundBible.com-1373580685.wav" },
        { AssetId::TURKEY_SOUND, "res/sounds/Turkey_Call_2-JimBob-26023545.wav" },
        { AssetId::WOLF_SOUND, "res/sounds/Howl.wav" },

        // MUSIC.
        { AssetId::INTRO_MUSIC, "res/sounds/NoahsArkIntro1_80PercentSpeedWithFadeout.wav" },
        { AssetId::OVERWORLD_BACKGROUND_MUSIC, "res/sounds/overworld_music.wav" },
    };
}
