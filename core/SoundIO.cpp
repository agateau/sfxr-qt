#include <SoundIO.h>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QUrl>

#include <Sound.h>

namespace SoundIO {

static constexpr int MAX_SUPPORTED_VERSION = 1;

bool load(Sound* sound, const QUrl& url) {
    QString path = url.path();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file" << path;
        return false;
    }
    QString ext = path.section(".", -1);
    if (ext == "sfxr") {
        if (!loadSfxr(sound, &file)) {
            return false;
        }
    } else if (ext == "sfxj") {
        if (!loadSfxj(sound, &file)) {
            return false;
        }
    } else {
        qWarning() << "Can't load from" << ext;
        return false;
    }
    sound->setUrl(url);
    return true;
}

bool loadSfxr(Sound* sound, QIODevice* device) {
    auto readQReal = [device] {
        float value;
        device->read(reinterpret_cast<char*>(&value), sizeof(float));
        return qreal(value);
    };
    auto readInt = [device] {
        int value;
        device->read(reinterpret_cast<char*>(&value), sizeof(int));
        return value;
    };

    int version = readInt();
    if (version != 100 && version != 101 && version != 102) {
        return false;
    }

    sound->setWaveForm(static_cast<WaveForm::Enum>(readInt()));

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
    device->read(reinterpret_cast<char*>(&unused), sizeof(bool));

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
    return true;
}

bool save(const Sound* sound, const QUrl& url) {
    QString path = url.path();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write to" << path;
        return false;
    }
    QString ext = path.section(".", -1);
    if (ext == "sfxr") {
        return saveSfxr(sound, &file);
    } else if (ext == "sfxj") {
        return saveSfxj(sound, &file);
    } else {
        qWarning() << "Can't save to" << ext;
        return false;
    }
}

bool saveSfxr(const Sound* sound, QIODevice* device) {
    // File format uses float, but we use qreal, so we need to round the value down
    auto writeQReal = [device](qreal value) {
        float fvalue = float(value);
        device->write(reinterpret_cast<char*>(&fvalue), sizeof(float));
    };

    auto writeInt = [device](int value) {
        device->write(reinterpret_cast<char*>(&value), sizeof(int));
    };

    int version = 102;
    writeInt(version);
    writeInt(sound->waveForm());

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
    device->write(reinterpret_cast<char*>(&filter_on), sizeof(bool));
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

bool loadSfxj(Sound* sound, QIODevice* device) {
    auto json = device->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (!doc.isObject()) {
        qWarning() << "Invalid json";
        return false;
    }
    auto root = doc.object();
    auto versionValue = root["version"];
    if (!versionValue.isDouble()) {
        qWarning() << "Invalid version field";
        return false;
    }
    auto version = versionValue.toInt();
    if (version > MAX_SUPPORTED_VERSION) {
        qWarning() << "Unsupported version:" << version;
        qWarning() << "This version only supports files up to version" << MAX_SUPPORTED_VERSION;
        return false;
    }

    auto props = root["properties"].toObject();
    auto it = props.constBegin(), end = props.constEnd();
    for (; it != end; ++it) {
        sound->setProperty(it.key().toUtf8(), it.value().toVariant());
    }
    return true;
}

bool saveSfxj(const Sound* sound, QIODevice* device) {
    static const QSet<QString> IGNORED_PROPERTIES = {"url", "name", "objectName", "hasRealUrl"};

    QJsonObject root;
    root["version"] = MAX_SUPPORTED_VERSION;

    QJsonObject props;
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        QString name = property.name();
        if (IGNORED_PROPERTIES.contains(name)) {
            continue;
        }
        props[name] = QJsonValue::fromVariant(property.read(sound));
    }
    root["properties"] = props;

    auto json = QJsonDocument(root).toJson();
    device->write(json);
    return true;
}

} // namespace SoundIO
