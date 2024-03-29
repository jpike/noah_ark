#include "Resources/PredefinedAssetPackages.h"

namespace RESOURCES
{
    const std::string INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME = "IntroAssets.package";

    const AssetPackageDefinition INTRO_SEQUENCE_ASSET_PACKAGE_DEFINITION({
        AssetDefinition(AssetType::MUSIC, AssetId::INTRO_MUSIC, "res/sounds/NoahsArkIntro1_80PercentSpeedWithFadeout.wav"),
        AssetDefinition(AssetType::SHADER, AssetId::COLORED_TEXTURE_SHADER, "res/shaders/ColoredTexture.frag"),
    });

    const std::string MAIN_ASSET_PACKAGE_FILENAME = "MainAssets.package";

    const AssetPackageDefinition MAIN_ASSET_PACKAGE_DEFINITION({
        // OTHER TEXTURES.
        AssetDefinition(AssetType::TEXTURE, AssetId::ANIMAL_TEXTURE, "res/images/animal_sprites.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::ARK_TEXTURE, "res/images/ark_tile_set.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::AXE_TEXTURE, "res/images/noah_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::DUST_CLOUD_TEXTURE, "res/images/tree_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FOOD_TEXTURE, "res/images/food_sprites.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::MAIN_TILESET_TEXTURE, "res/images/main_tile_set.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::NOAH_TEXTURE, "res/images/noah_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::NOAH_WIFE_TEXTURE, "res/images/noah_wife_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::SHEM_TEXTURE, "res/images/shem_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::SHEM_WIFE_TEXTURE, "res/images/shem_wife_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::HAM_TEXTURE, "res/images/ham_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::HAM_WIFE_TEXTURE, "res/images/ham_wife_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::JAPHETH_TEXTURE, "res/images/japheth_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::JAPHETH_WIFE_TEXTURE, "res/images/japheth_wife_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::TREE_TEXTURE, "res/images/tree_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::WOOD_LOG_TEXTURE, "res/images/tree_sprite1.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FLAMING_SWORD_TEXTURE, "res/images/flaming_sword_sprite.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::OTHER_OBJECT_TEXTURE, "res/images/object_sprites.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FLOOD_CUTSCENE_MOUNTAIN, "res/images/mountain.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FLOOD_CUTSCENE_WATERS, "res/images/flood_waters.png"),
        AssetDefinition(AssetType::TEXTURE, AssetId::FLOOD_CUTSCENE_ARK, "res/images/flood_scene_ark.png"),

        // GENERAL SOUND EFFECTS.
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::ARK_BUILDING_SOUND, "res/sounds/ark_build_sound1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::AXE_HIT_SOUND, "res/sounds/axe_tree_hit1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::COLLECT_BIBLE_VERSE_SOUND, "res/sounds/collect_bible_verse1.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FOOD_PICKUP_SOUND, "res/sounds/food_pickup_sound.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::TREE_SHAKE_SOUND, "res/sounds/tree_shake4.wav"),

        // ANIMAL SOUND EFFECTS.
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::ALLIGATOR_SOUND, "res/sounds/animals/alligator.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BABOON_SOUND, "res/sounds/animals/baboon.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BAT_SOUND, "res/sounds/animals/bat.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BEAR_SOUND, "res/sounds/animals/bear.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BEE_SOUND, "res/sounds/animals/bee.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BOBCAT_SOUND, "res/sounds/animals/bobcat.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::BUTTERFLY_SOUND, "res/sounds/animals/butterfly.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CAMEL_SOUND, "res/sounds/animals/camel.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CAT_SOUND, "res/sounds/animals/cat.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CATTLE_SOUND, "res/sounds/animals/cattle.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHEETAH_SOUND, "res/sounds/animals/cheetah.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHICKEN_SOUND, "res/sounds/animals/chicken.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CHIPMUNK_SOUND, "res/sounds/animals/chipmunk.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::CRICKET_SOUND, "res/sounds/animals/cricket.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DEER_SOUND, "res/sounds/animals/deer.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DOG_SOUND, "res/sounds/animals/dog.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DOVE_SOUND, "res/sounds/animals/dove.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::DUCK_SOUND, "res/sounds/animals/duck.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::EAGLE_SOUND, "res/sounds/animals/eagle.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::ELEPHANT_SOUND, "res/sounds/animals/elephant.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FOX_SOUND, "res/sounds/animals/fox.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::FROG_SOUND, "res/sounds/animals/frog.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GOAT_SOUND, "res/sounds/animals/goat.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GOOSE_SOUND, "res/sounds/animals/goose.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GORILLA_SOUND, "res/sounds/animals/gorilla.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::GUINEA_PIG_SOUND, "res/sounds/animals/guineapig.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HAWK_SOUND, "res/sounds/animals/hawk.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HIPPO_SOUND, "res/sounds/animals/hippo.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HORSE_SOUND, "res/sounds/animals/horse.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::HYENA_SOUND, "res/sounds/animals/hyena.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::KOALA_SOUND, "res/sounds/animals/koala.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LEOPARD_SOUND, "res/sounds/animals/leopard.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LION_SOUND, "res/sounds/animals/lion.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::LOCUST_SOUND, "res/sounds/animals/locust.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::MONKEY_SOUND, "res/sounds/animals/monkey.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::OSPREY_SOUND, "res/sounds/animals/osprey.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::OWL_SOUND, "res/sounds/animals/owl.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PEACOCK_SOUND, "res/sounds/animals/peacock.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PELICAN_SOUND, "res/sounds/animals/pelican.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::PIG_SOUND, "res/sounds/animals/pig.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::POLAR_BEAR_SOUND, "res/sounds/animals/polarbear.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::RACCOON_SOUND, "res/sounds/animals/raccoon.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::RAVEN_SOUND, "res/sounds/animals/raven.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::SHEEP_SOUND, "res/sounds/animals/sheep.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::SNAKE_SOUND, "res/sounds/animals/snake.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::SQUIRREL_SOUND, "res/sounds/animals/squirrel.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::TURKEY_SOUND, "res/sounds/animals/turkey.wav"),
        AssetDefinition(AssetType::SOUND_EFFECT, AssetId::WOLF_SOUND, "res/sounds/animals/wolf.wav"),

        // MUSIC.
        AssetDefinition(AssetType::MUSIC, AssetId::NEW_GAME_INTRO_MUSIC, "res/sounds/Intro1.wav"),
        AssetDefinition(AssetType::MUSIC, AssetId::OVERWORLD_BACKGROUND_MUSIC, "res/sounds/overworld_music.wav"),
        AssetDefinition(AssetType::MUSIC, AssetId::BASIC_RAIN_BACKGROUND_SOUNDS, "res/sounds/Rain Ending-SoundBible.com-1043019515.wav"),
        AssetDefinition(AssetType::MUSIC, AssetId::THUNDERSTORM_BACKGROUND_SOUNDS, "res/sounds/2_Minute_Thunderstorm-Mike_Koenig-574654058.wav"),
        AssetDefinition(AssetType::MUSIC, AssetId::AFTER_RAIN_BACKGROUND_SOUNDS, "res/sounds/rainforest_ambience-GlorySunz-1938133500.wav"),
        AssetDefinition(AssetType::MUSIC, AssetId::FINAL_CREDITS_MUSIC, "res/sounds/NoahsArkIntro1_PaulstretchPlusMore.wav"),

        // SHADERS.
        AssetDefinition(AssetType::SHADER, AssetId::TIME_OF_DAY_SHADER, "res/shaders/TimeOfDay.frag"),
        AssetDefinition(AssetType::SHADER, AssetId::SHINY_BALL_SHADER, "res/shaders/ShinyBall.frag"),
        AssetDefinition(AssetType::SHADER, AssetId::RAINBOW_SHADER, "res/shaders/Rainbow.frag"),
    });
}
