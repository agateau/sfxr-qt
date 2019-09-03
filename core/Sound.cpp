#include "Sound.h"

#include <QDebug>
#include <QFile>
#include <QMetaProperty>
#include <QUrl>

static const char UNSAVED_SCHEME[] = "unsaved";

Sound::Sound(QObject* parent)
    : BaseSound(parent) {
    resetParams();
}

void Sound::resetParams() {
    setUnsavedName("New");
    setWaveType(0);

    setBaseFrequency(0.3f);
    setMinFrequency(0.0f);
    setSlide(0.0f);
    setDeltaSlide(0.0f);
    setSquareDuty(0.0f);
    setDutySweep(0.0f);

    setVibratoDepth(0.0f);
    setVibratoSpeed(0.0f);

    setAttackTime(0.0f);
    setSustainTime(0.3f);
    setDecayTime(0.4f);
    setSustainPunch(0.0f);

    setLpFilterResonance(0.0f);
    setLpFilterCutoff(1.0f);
    setLpFilterCutoffSweep(0.0f);
    setHpFilterCutoff(0.0f);
    setHpFilterCutoffSweep(0.0f);

    setPhaserOffset(0.0f);
    setPhaserSweep(0.0f);

    setRepeatSpeed(0.0f);

    setChangeSpeed(0.0f);
    setChangeAmount(0.0f);
}

void Sound::fromOther(const Sound* other) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int idx = 0; idx < mo.propertyCount(); ++idx) {
        QMetaProperty property = mo.property(idx);
        QVariant value = property.read(other);
        property.write(this, value);
    }
}

bool Sound::load(const QUrl& url) {
    QString path = url.path();
    QFile file(path);
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

    setWaveType(readInt());

    setVolume(version == 102 ? readFloat() : 0.5f);

    setBaseFrequency(readFloat());
    setMinFrequency(readFloat());
    setSlide(readFloat());
    if (version >= 101) {
        setDeltaSlide(readFloat());
    }
    setSquareDuty(readFloat());
    setDutySweep(readFloat());

    setVibratoDepth(readFloat());
    setVibratoSpeed(readFloat());
    // p_vib_delay, unused
    readFloat();

    setAttackTime(readFloat());
    setSustainTime(readFloat());
    setDecayTime(readFloat());
    setSustainPunch(readFloat());

    // filter_on, unused
    bool unused;
    file.read(reinterpret_cast<char*>(&unused), sizeof(bool));

    setLpFilterResonance(readFloat());
    setLpFilterCutoff(readFloat());
    setLpFilterCutoffSweep(readFloat());
    setHpFilterCutoff(readFloat());
    setHpFilterCutoffSweep(readFloat());

    setPhaserOffset(readFloat());
    setPhaserSweep(readFloat());

    setRepeatSpeed(readFloat());

    if (version >= 101) {
        setChangeSpeed(readFloat());
        setChangeAmount(readFloat());
    }

    setUrl(url);
    return true;
}

bool Sound::save(const QUrl& url) {
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
    writeInt(wave_type);

    writeFloat(sound_vol);

    writeFloat(p_base_freq);
    writeFloat(p_freq_limit);
    writeFloat(p_freq_ramp);
    writeFloat(p_freq_dramp);
    writeFloat(p_duty);
    writeFloat(p_duty_ramp);

    writeFloat(p_vib_strength);
    writeFloat(p_vib_speed);
    float p_vib_delay = 0;
    writeFloat(p_vib_delay);

    writeFloat(p_env_attack);
    writeFloat(p_env_sustain);
    writeFloat(p_env_decay);
    writeFloat(p_env_punch);

    bool filter_on = false;
    file.write(reinterpret_cast<char*>(&filter_on), sizeof(bool));
    writeFloat(p_lpf_resonance);
    writeFloat(p_lpf_freq);
    writeFloat(p_lpf_ramp);
    writeFloat(p_hpf_freq);
    writeFloat(p_hpf_ramp);

    writeFloat(p_pha_offset);
    writeFloat(p_pha_ramp);

    writeFloat(p_repeat_speed);

    writeFloat(p_arp_speed);
    writeFloat(p_arp_mod);

    setUrl(url);
    return true;
}

QString Sound::name() const {
    QString fileName = mUrl.fileName();
    return fileName.section(".", 0, -2);
}

bool Sound::hasRealUrl() const {
    return mUrl.scheme() != UNSAVED_SCHEME;
}

void Sound::setUrl(const QUrl& url) {
    BaseSound::setUrl(url);
    nameChanged(name());
    hasRealUrlChanged(hasRealUrl());
}

void Sound::setUnsavedName(const QString& name) {
    auto urlString = QString("%1:///%2.sfxr").arg(UNSAVED_SCHEME).arg(name);
    setUrl(urlString);
}
