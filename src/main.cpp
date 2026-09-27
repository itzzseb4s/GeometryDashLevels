#include <Geode/Geode.hpp>
#include <Geode/modify/LocalLevelManager.hpp>
#include <Geode/modify/LevelTools.hpp>

using namespace geode::prelude;

namespace {
    constexpr int CUSTOM_LEVEL_ID = 1;

    constexpr char const* CUSTOM_LEVEL_NAME = "Test Level";
    constexpr int CUSTOM_STARS = 3;
    constexpr int CUSTOM_COINS = 3;
    constexpr GJDifficulty CUSTOM_DIFFICULTY = GJDifficulty::Normal;
}

class $modify(CustomLocalLevelManager, LocalLevelManager) {
    gd::string getMainLevelString(int id) {
        if (id != CUSTOM_LEVEL_ID)
            return LocalLevelManager::getMainLevelString(id);

        auto path = Mod::get()->getResourcesDir() / "levels" / "level1.txt";

        auto result = utils::file::readString(path);

        if (result.isErr()) {
            log::error(
                "Could not read custom level: {}",
                result.unwrapErr()
            );

            return "";
        }

        log::info(
            "Loaded custom level string: {} characters",
            result.unwrap().size()
        );

        return result.unwrap();
    }
};

class $modify(CustomLevelTools, LevelTools) {
    GJGameLevel* getLevel(int levelID, bool loaded) {

        if (levelID != CUSTOM_LEVEL_ID)
            return LevelTools::getLevel(levelID, loaded);

        auto level = GJGameLevel::create();

        if (!level)
            return nullptr;

        level->m_levelID = CUSTOM_LEVEL_ID;
        level->m_levelName = CUSTOM_LEVEL_NAME;
        level->m_levelType = GJLevelType::Saved;

        level->m_audioTrack = 0;
        level->m_coins = CUSTOM_COINS;
        level->m_stars = CUSTOM_STARS;
        level->m_difficulty = CUSTOM_DIFFICULTY;

        if (!loaded) {
            level->m_levelString =
                LocalLevelManager::sharedState()
                    ->getMainLevelString(CUSTOM_LEVEL_ID);
        }

        return level;
    }

    bool verifyLevelIntegrity(
        gd::string verifyString,
        int levelID
    ) {
        if (levelID == CUSTOM_LEVEL_ID)
            return true;

        return LevelTools::verifyLevelIntegrity(
            verifyString,
            levelID
        );
    }
};

$on_mod(Loaded) {
    auto path =
        Mod::get()->getResourcesDir()
        / "levels"
        / "level1.txt";

    log::info(
        "Geometry Dash Levels loaded!"
    );

    log::info(
        "Level resource path: {}",
        path.string()
    );

    auto test = utils::file::readString(path);

    if (test.isErr()) {
        log::error(
            "TEST FAILED: level1.txt cannot be read"
        );
    }
    else {
        log::info(
            "TEST OK: level1.txt read successfully ({} chars)",
            test.unwrap().size()
        );
    }
}
