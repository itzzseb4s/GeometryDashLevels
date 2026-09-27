#include <Geode/Geode.hpp>
#include <Geode/modify/LocalLevelManager.hpp>
#include <Geode/modify/LevelTools.hpp>

using namespace geode::prelude;

namespace {
    constexpr int CUSTOM_MAIN_LEVEL = 1;

    constexpr char const* CUSTOM_LEVEL_NAME = "Test Level";
    constexpr int CUSTOM_STARS = 3;
    constexpr int CUSTOM_COINS = 3;
    constexpr GJDifficulty CUSTOM_DIFFICULTY = GJDifficulty::Normal;
}

// ============================================================
// REEMPLAZAR LA CADENA DE STEREO MADNESS
// ============================================================

class $modify(CustomLocalLevelManager, LocalLevelManager) {
    gd::string getMainLevelString(int id) {
        if (id != CUSTOM_MAIN_LEVEL)
            return LocalLevelManager::getMainLevelString(id);

        auto file = CCString::createWithFormat(
            "level%i.txt"_spr,
            id
        );

        if (file == nullptr)
            return LocalLevelManager::getMainLevelString(id);

        auto content = CCString::createWithContentsOfFile(
            file->getCString()
        );

        if (content == nullptr)
            return LocalLevelManager::getMainLevelString(id);

        return gd::string(content->getCString());
    }
};

// ============================================================
// REEMPLAZAR LOS DATOS DE STEREO MADNESS
// ============================================================

class $modify(CustomLevelTools, LevelTools) {
    GJGameLevel* getLevel(int levelID, bool loaded) {

        // Los niveles 2+ siguen siendo vanilla.
        if (levelID != CUSTOM_MAIN_LEVEL)
            return LevelTools::getLevel(levelID, loaded);

        auto level = GJGameLevel::create();

        if (level == nullptr)
            return nullptr;

        // Nombre
        level->m_levelName = CUSTOM_LEVEL_NAME;

        // ID
        level->m_levelID = CUSTOM_MAIN_LEVEL;

        // Tipo
        level->m_levelType = GJLevelType::Saved;

        // Música de prueba
        level->m_audioTrack = 0;

        // Monedas
        level->m_coins = CUSTOM_COINS;

        // Estrellas
        level->m_stars = CUSTOM_STARS;

        // Dificultad
        level->m_difficulty = CUSTOM_DIFFICULTY;

        // Cargar nuestra cadena de nivel.
        if (!loaded) {
            level->m_levelString =
                LocalLevelManager::sharedState()
                    ->getMainLevelString(CUSTOM_MAIN_LEVEL);
        }

        return level;
    }

    bool verifyLevelIntegrity(
        gd::string verifyString,
        int levelID
    ) {
        if (levelID == CUSTOM_MAIN_LEVEL)
            return true;

        return LevelTools::verifyLevelIntegrity(
            verifyString,
            levelID
        );
    }
};

// ============================================================
// MOD CARGADO
// ============================================================

$on_mod(Loaded) {
    log::info("Geometry Dash Levels loaded!");
    log::info("Stereo Madness replaced with Test Level.");
}
