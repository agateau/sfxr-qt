#include <SoundIO.h>

#include <QDebug>
#include <QFile>
#include <QUrl>

#include <Sound.h>

namespace SoundIO {

bool load(Sound* sound, const QUrl& url) {
    QFile file(url.path());
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    auto readQReal = [&file] {
        float value;
        file.read(reinterpret_cast<char*>(&value), sizeof(float));
        return qreal(value);
    };
    auto readInt = [&file] {
        int value;
        file.read(reinterpret_cast<char*>(&value), sizeof(int));
        return value;
    };

    int version = readInt();
    if (version != 100 && version != 101 && version != 102) {
        return false;
    }

    sound->setWaveType(readInt());

    sound->setVolume(version == 102 ? readQReal() : 0.5);

    sound->setBaseFrequency(readQReal());
    sound->setMinFrequency(readQReal());
    sound->setSlide(readQReal());
    if (version >= 101) {
        sound->setDeltaSlide(readQReal());
    }
    sound->setSquareDuty(readQReal());
    sound->setDutySweep(readQReal());

    sound->setVibratoDepth(readQReal());
    sound->setVibratoSpeed(readQReal());
    // p_vib_delay, unused
    readQReal();

    sound->setAttackTime(readQReal());
    sound->setSustainTime(readQReal());
    sound->setDecayTime(readQReal());
    sound->setSustainPunch(readQReal());

    // filter_on, unused
    bool unused;
    file.read(reinterpret_cast<char*>(&unused), sizeof(bool));

    sound->setLpFilterResonance(readQReal());
    sound->setLpFilterCutoff(readQReal());
    sound->setLpFilterCutoffSweep(readQReal());
    sound->setHpFilterCutoff(readQReal());
    sound->setHpFilterCutoffSweep(readQReal());

    sound->setPhaserOffset(readQReal());
    sound->setPhaserSweep(readQReal());

    sound->setRepeatSpeed(readQReal());

    if (version >= 101) {
        sound->setChangeSpeed(readQReal());
        sound->setChangeAmount(readQReal());
    }

    sound->setUrl(url);

    return true;
}

bool save(const Sound* sound, const QUrl& url) {
    QString path = url.path();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write to" << path;
        return false;
    }

    // File format uses float, but we use qreal, so we need to round the value down
    auto writeQReal = [&file](qreal value) {
        float fvalue = float(value);
        file.write(reinterpret_cast<char*>(&fvalue), sizeof(float));
    };

    auto writeInt = [&file](int value) {
        file.write(reinterpret_cast<char*>(&value), sizeof(int));
    };

    int version = 102;
    writeInt(version);
    writeInt(sound->waveType());

    writeQReal(sound->volume());

    writeQReal(sound->baseFrequency());
    writeQReal(sound->minFrequency());
    writeQReal(sound->slide());
    writeQReal(sound->deltaSlide());
    writeQReal(sound->squareDuty());
    writeQReal(sound->dutySweep());

    writeQReal(sound->vibratoDepth());
    writeQReal(sound->vibratoSpeed());
    qreal p_vib_delay = 0;
    writeQReal(p_vib_delay);

    writeQReal(sound->attackTime());
    writeQReal(sound->sustainTime());
    writeQReal(sound->decayTime());
    writeQReal(sound->sustainPunch());

    bool filter_on = false;
    file.write(reinterpret_cast<char*>(&filter_on), sizeof(bool));
    writeQReal(sound->lpFilterResonance());
    writeQReal(sound->lpFilterCutoff());
    writeQReal(sound->lpFilterCutoffSweep());
    writeQReal(sound->hpFilterCutoff());
    writeQReal(sound->hpFilterCutoffSweep());

    writeQReal(sound->phaserOffset());
    writeQReal(sound->phaserSweep());

    writeQReal(sound->repeatSpeed());

    writeQReal(sound->changeSpeed());
    writeQReal(sound->changeAmount());

    return true;
}

} // namespace
