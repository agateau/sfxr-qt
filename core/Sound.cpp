#include "Sound.h"

#include <QDebug>
#include <QFile>
#include <QMetaProperty>
#include <QUrl>

#include <SoundIO.h>

static const char UNSAVED_SCHEME[] = "unsaved";

Sound::Sound(QObject* parent) : BaseSound(parent) {
    resetParams();
}

void Sound::resetParams() {
    setUnsavedName("New");
    setWaveForm(WaveForm::Square);

    setBaseFrequency(0.3);
    setMinFrequency(0.0);
    setSlide(0.0);
    setDeltaSlide(0.0);
    setSquareDuty(0.0);
    setDutySweep(0.0);

    setVibratoDepth(0.0);
    setVibratoSpeed(0.0);

    setAttackTime(0.0);
    setSustainTime(0.3);
    setDecayTime(0.4);
    setSustainPunch(0.0);

    setLpFilterResonance(0.0);
    setLpFilterCutoff(1.0);
    setLpFilterCutoffSweep(0.0);
    setHpFilterCutoff(0.0);
    setHpFilterCutoffSweep(0.0);

    setPhaserOffset(0.0);
    setPhaserSweep(0.0);

    setRepeatSpeed(0.0);

    setChangeSpeed(0.0);
    setChangeAmount(0.0);
}

void Sound::fromOther(const Sound* other) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int idx = 0; idx < mo.propertyCount(); ++idx) {
        QMetaProperty property = mo.property(idx);
        QVariant value = property.read(other);
        property.write(this, value);
    }
}

Result Sound::load(const QUrl& url) {
    return SoundIO::load(this, url);
}

Result Sound::save(const QUrl& url) {
    auto result = SoundIO::save(this, url);
    if (!result) {
        return result;
    }
    setUrl(url);
    return {};
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
