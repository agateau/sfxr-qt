#include <Sound.h>

#include <QMetaProperty>
#include <QTemporaryDir>
#include <QUrl>

#include <catch2/catch.hpp>

#include <TestConfig.h>

#include <SoundUtils.h>

static bool fuzzyEq(const Sound& s1, const Sound& s2) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        QVariant v1 = property.read(&s1);
        QVariant v2 = property.read(&s2);

        if (property.type() == QVariant::Double) {
            if (v1.toDouble() != Approx(v2.toDouble())) {
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

    SECTION("load/save sfxr") {
        QTemporaryDir tempDir;
        auto path = tempDir.filePath("test.sfxr");
        Sound sound1;
        SoundUtils::mutate(&sound1);
        REQUIRE(sound1.save(path));

        Sound sound2;
        REQUIRE(sound2.load(path));

        REQUIRE(fuzzyEq(sound1, sound2));
    }
}
