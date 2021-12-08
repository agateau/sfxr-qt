#include "Sound.h"
#include "Synthesizer.h"
#include "TestConfig.h"
#include "TestUtils.h"
#include "WavSaver.h"
#include "WaveForm.h"

#include <QDebug>
#include <QDir>
#include <QTemporaryDir>

#include <catch2/catch.hpp>

static constexpr char FIXTURES_DIR[] = TEST_FIXTURES_DIR "/synthesizer";

static QStringList listTestNames() {
    QStringList lst;
    auto inputDir = QString("%1/input").arg(FIXTURES_DIR);
    for (const auto& info : QDir(inputDir).entryInfoList({"*.sfxj"})) {
        lst << info.baseName();
    }
    return lst;
}

namespace SynthesizerTest {

void updateExpectedFiles() {
    WaveForm::registerType();
    WavSaver wavSaver;

    for (const auto& name : listTestNames()) {
        auto soundPath = QString("%1/input/%2.sfxj").arg(FIXTURES_DIR, name);
        auto expectedPath = QString("%1/expected/%2.wav").arg(FIXTURES_DIR, name);

        qInfo() << "Updating" << expectedPath;
        Sound sound;
        sound.load(QUrl::fromLocalFile(soundPath));
        wavSaver.save(&sound, QUrl::fromLocalFile(expectedPath));
    }
}

} // namespace SynthesizerTest

TEST_CASE("Synthesizer") {
    WaveForm::registerType();
    QTemporaryDir tempDir;
    WavSaver wavSaver;

    for (const auto& name : listTestNames()) {
        SECTION(name.toUtf8().data()) {
            auto soundPath = QString("%1/input/%2.sfxj").arg(FIXTURES_DIR, name);
            auto expectedPath = QString("%1/expected/%2.wav").arg(FIXTURES_DIR, name);
            auto resultPath = tempDir.filePath(name + ".wav");

            Sound sound;
            sound.load(QUrl::fromLocalFile(soundPath));

            // We test Synthesizer through WavSaver
            wavSaver.save(&sound, QUrl::fromLocalFile(resultPath));

            QByteArray expected = loadFile(expectedPath);
            REQUIRE(!expected.isEmpty());

            QByteArray result = loadFile(resultPath);
            REQUIRE(!result.isEmpty());

            REQUIRE(result == expected);
        }
    }
}
