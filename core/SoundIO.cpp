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

    auto readFloat = [&file] {
        float value;
        file.read(reinterpret_cast<char*>(&value), sizeof(float));
        return value;
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

    sound->setVolume(version == 102 ? readFloat() : 0.5f);

    sound->setBaseFrequency(readFloat());
    sound->setMinFrequency(readFloat());
    sound->setSlide(readFloat());
    if (version >= 101) {
        sound->setDeltaSlide(readFloat());
    }
    sound->setSquareDuty(readFloat());
    sound->setDutySweep(readFloat());

    sound->setVibratoDepth(readFloat());
    sound->setVibratoSpeed(readFloat());
    // p_vib_delay, unused
    readFloat();

    sound->setAttackTime(readFloat());
    sound->setSustainTime(readFloat());
    sound->setDecayTime(readFloat());
    sound->setSustainPunch(readFloat());

    // filter_on, unused
    bool unused;
    file.read(reinterpret_cast<char*>(&unused), sizeof(bool));

    sound->setLpFilterResonance(readFloat());
    sound->setLpFilterCutoff(readFloat());
    sound->setLpFilterCutoffSweep(readFloat());
    sound->setHpFilterCutoff(readFloat());
    sound->setHpFilterCutoffSweep(readFloat());

    sound->setPhaserOffset(readFloat());
    sound->setPhaserSweep(readFloat());

    sound->setRepeatSpeed(readFloat());

    if (version >= 101) {
        sound->setChangeSpeed(readFloat());
        sound->setChangeAmount(readFloat());
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
    auto writeFloat = [&file](qreal value) {
        float fvalue = float(value);
        file.write(reinterpret_cast<char*>(&fvalue), sizeof(float));
    };

    auto writeInt = [&file](int value) {
        file.write(reinterpret_cast<char*>(&value), sizeof(int));
    };

    int version = 102;
    writeInt(version);
    writeInt(sound->waveType());

    writeFloat(sound->volume());

    writeFloat(sound->baseFrequency());
    writeFloat(sound->minFrequency());
    writeFloat(sound->slide());
    writeFloat(sound->deltaSlide());
    writeFloat(sound->squareDuty());
    writeFloat(sound->dutySweep());

    writeFloat(sound->vibratoDepth());
    writeFloat(sound->vibratoSpeed());
    qreal p_vib_delay = 0;
    writeFloat(p_vib_delay);

    writeFloat(sound->attackTime());
    writeFloat(sound->sustainTime());
    writeFloat(sound->decayTime());
    writeFloat(sound->sustainPunch());

    bool filter_on = false;
    file.write(reinterpret_cast<char*>(&filter_on), sizeof(bool));
    writeFloat(sound->lpFilterResonance());
    writeFloat(sound->lpFilterCutoff());
    writeFloat(sound->lpFilterCutoffSweep());
    writeFloat(sound->hpFilterCutoff());
    writeFloat(sound->hpFilterCutoffSweep());

    writeFloat(sound->phaserOffset());
    writeFloat(sound->phaserSweep());

    writeFloat(sound->repeatSpeed());

    writeFloat(sound->changeSpeed());
    writeFloat(sound->changeAmount());

    return true;
}

} // namespace
