#include "Sound.h"

#include <QUrl>

#include <catch2/catch.hpp>

#include "TestConfig.h"

TEST_CASE("Sound") {
    SECTION("load sfxr") {
        Sound sound;
        auto path = QUrl::fromLocalFile(QString(TEST_FIXTURES_DIR) + "/pickup.sfxr");
        REQUIRE(sound.load(path));
        CHECK(sound.waveType() == 0);
        CHECK(sound.sustainTime() == Approx(0.05916));
        CHECK(sound.baseFrequency() == Approx(0.5019));
        CHECK(sound.changeSpeed() == Approx(0.54938));
        CHECK(sound.phaserOffset() == 0);
    }
}
