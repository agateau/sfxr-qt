#include "sound.h"

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
    FILE* file = fopen(path.toLocal8Bit().constData(), "rb");
    if (!file) {
        return false;
    }

    auto readFloat = [file] {
        float value;
        fread(&value, 1, sizeof(float), file);
        return value;
    };
    auto readInt = [file] {
        int value;
        fread(&value, 1, sizeof(int), file);
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
    fread(&unused, 1, sizeof(bool), file);

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

    fclose(file);

    setUrl(url);
    return true;
}

bool Sound::save(const QUrl& url) {
    QString path = url.path();
    FILE* file = fopen(path.toLocal8Bit().constData(), "wb");
    if (!file) {
        return false;
    }

    // File format uses float, but we use qreal, so we need to round the value down
    auto writeFloat = [file](qreal value) {
        float fvalue = float(value);
        fwrite(&fvalue, 1, sizeof(float), file);
    };

    int version = 102;
    fwrite(&version, 1, sizeof(int), file);

    fwrite(&wave_type, 1, sizeof(int), file);

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
    fwrite(&filter_on, 1, sizeof(bool), file);
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

    fclose(file);
    setUrl(url);
    return true;
}

QString Sound::name() const {
    QString fileName = mUrl.fileName();
    return fileName.section(".", 0, -2);
}

void Sound::setUrl(const QUrl& url) {
    BaseSound::setUrl(url);
    nameChanged(name());
}

void Sound::setUnsavedName(const QString& name) {
    auto urlString = QString("%1:///%2.sfxr").arg(UNSAVED_SCHEME).arg(name);
    setUrl(urlString);
}
