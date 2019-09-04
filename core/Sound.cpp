#include "Sound.h"

#include <QDebug>
#include <QFile>
#include <QMetaProperty>
#include <QUrl>

#include <SoundIO.h>

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
    return SoundIO::load(this, url);
}

bool Sound::save(const QUrl& url) {
    if (!SoundIO::save(this, url)) {
        return false;
    }
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
