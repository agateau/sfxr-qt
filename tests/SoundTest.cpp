#include "Sound.h"
#include "SoundIO.h"
#include "SoundUtils.h"
#include "TestConfig.h"
#include "TestUtils.h"

#include <QBuffer>
#include <QMetaProperty>
#include <QTemporaryDir>
#include <QUrl>

#include <catch2/catch_all.hpp>

using Catch::Matchers::Equals;
using Catch::Matchers::WithinAbs;

static constexpr double PRECISION = 0.00001;

static auto Within(float value) {
    return WithinAbs(value, PRECISION);
}

static bool fuzzyEq(const Sound& s1, const Sound& s2) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        QVariant v1 = property.read(&s1);
        QVariant v2 = property.read(&s2);

        if (property.type() == QVariant::Double) {
            if (qAbs(v1.toDouble() - v2.toDouble()) > PRECISION) {
                return false;
            }
        } else {
            if (v1 != v2) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("Sound") {
    WaveForm::registerType();
    SECTION("load sfxr") {
        Sound sound;
        auto path = QUrl::fromLocalFile(QString(TEST_FIXTURES_DIR) + "/pickup.sfxr");
        REQUIRE(sound.load(path));
        CHECK(sound.waveForm() == WaveForm::Enum::Square);
        CHECK_THAT(sound.sustainTime(), Within(0.05916));
        CHECK_THAT(sound.sustainTime(), Within(0.05916));
        CHECK_THAT(sound.sustainTime(), Within(0.05916));
        CHECK_THAT(sound.baseFrequency(), Within(0.5019));
        CHECK_THAT(sound.changeSpeed(), Within(0.54938));
        CHECK(sound.phaserOffset() == 0);
    }

    SECTION("load sfxj") {
        Sound sound;
        auto path = QUrl::fromLocalFile(QString(TEST_FIXTURES_DIR) + "/pickup.sfxj");
        REQUIRE(sound.load(path));
        CHECK(sound.waveForm() == WaveForm::Enum::Square);
        CHECK_THAT(sound.sustainTime(), Within(0.05916));
        CHECK_THAT(sound.baseFrequency(), Within(0.5019));
        CHECK_THAT(sound.changeSpeed(), Within(0.54938));
        CHECK(sound.phaserOffset() == 0);
    }

    SECTION("load/save sfxj") {
        QTemporaryDir tempDir;
        auto path = tempDir.filePath("test.sfxj");
        Sound sound1;
        SoundUtils::mutate(&sound1);
        REQUIRE(sound1.save(path));

        Sound sound2;
        REQUIRE(sound2.load(path));

        REQUIRE(fuzzyEq(sound1, sound2));
    }

    SECTION("loading an sfxj with a version too recent fails") {
        Sound sound;
        QByteArray json = "{ \"version\": 2000000 }";
        QBuffer buffer(&json);
        REQUIRE(buffer.open(QIODevice::ReadOnly));
        {
            QtDebugSilencer silencer;
            REQUIRE(!SoundIO::loadSfxj(&sound, &buffer));
        }
    }
}
