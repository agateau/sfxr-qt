#include "SoundIO.h"

#include "Result.h"
#include "Sound.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QSaveFile>
#include <QUrl>
#include <QtEndian>

namespace SoundIO {

static constexpr int MAX_SUPPORTED_VERSION = 1;

Result load(Sound* sound, const QUrl& url) {
    QString path = url.path();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        auto message = QCoreApplication::translate("SoundIO", "Cannot open %1: %2.")
                           .arg(path)
                           .arg(file.errorString());
        return Result::createError(message);
    }
    QString ext = path.section(".", -1);
    if (ext == "sfxr") {
        auto result = loadSfxr(sound, &file);
        if (!result) {
            return result;
        }
    } else if (ext == "sfxj") {
        auto result = loadSfxj(sound, &file);
        if (!result) {
            return result;
        }
    } else {
        auto message =
            QCoreApplication::translate("SoundIO", "Cannot load file with extension \"%1\".")
                .arg(ext);
        return Result::createError(message);
    }
    sound->setUrl(url);
    return {};
}

Result loadSfxr(Sound* sound, QIODevice* device) {
    auto readQReal = [device] {
        float value;
        device->read(reinterpret_cast<char*>(&value), sizeof(float));
        value = qFromLittleEndian(value);
        return qreal(value);
    };
    auto readInt32 = [device] {
        qint32 value;
        device->read(reinterpret_cast<char*>(&value), sizeof(qint32));
        value = qFromLittleEndian(value);
        return value;
    };

    int version = readInt32();
    if (version != 100 && version != 101 && version != 102) {
        auto message =
            QCoreApplication::translate("SoundIO", "Invalid version value: %1.").arg(version);
        return Result::createError(message);
    }

    sound->setWaveForm(static_cast<WaveForm::Enum>(readInt32()));

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
    return {};
}

Result save(const Sound* sound, const QUrl& url) {
    QString path = url.path();
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        auto message = QCoreApplication::translate("SoundIO", "Cannot write file.");
        return Result::createError(message);
    }

    QString ext = QFileInfo(path).suffix();
    if (ext.isEmpty()) {
        auto message = QCoreApplication::translate("SoundIO", "No file format provided.");
        return Result::createError(message);
    }

    Result result;
    if (ext == "sfxr") {
        result = saveSfxr(sound, &file);
    } else if (ext == "sfxj") {
        result = saveSfxj(sound, &file);
    } else {
        auto message =
            QCoreApplication::translate("SoundIO", "Cannot save to format \"%1\".").arg(ext);
        return Result::createError(message);
    }
    if (!result.isOk()) {
        return result;
    }

    if (!file.commit()) {
        auto message = QCoreApplication::translate("SoundIO", "Failed to save file: %1.")
                           .arg(file.errorString());
        return Result::createError(message);
    }
    return {};
}

Result saveSfxr(const Sound* sound, QIODevice* device) {
    // File format uses float, but we use qreal, so we need to round the value down
    auto writeQReal = [device](qreal value) {
        float fvalue = float(value);
        fvalue = qToLittleEndian(fvalue);
        device->write(reinterpret_cast<char*>(&fvalue), sizeof(float));
    };

    auto writeInt32 = [device](qint32 value) {
        value = qToLittleEndian(value);
        device->write(reinterpret_cast<char*>(&value), sizeof(qint32));
    };

    qint32 version = 102;
    writeInt32(version);
    writeInt32(sound->waveForm());

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

    qint32 filter_on = 0;
    writeInt32(filter_on);
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

    return {};
}

Result loadSfxj(Sound* sound, QIODevice* device) {
    auto json = device->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (!doc.isObject()) {
        auto message = QCoreApplication::translate("SoundIO", "Invalid JSON.");
        return Result::createError(message);
    }
    auto root = doc.object();
    auto versionValue = root["version"];
    if (!versionValue.isDouble()) {
        auto message = QCoreApplication::translate("SoundIO", "Invalid version field.");
        return Result::createError(message);
    }
    auto version = versionValue.toInt();
    if (version > MAX_SUPPORTED_VERSION) {
        auto message = QCoreApplication::translate("SoundIO",
                                                   "Unsupported version: %1. This application only "
                                                   "supports SFXJ files up to version %2.")
                           .arg(version)
                           .arg(MAX_SUPPORTED_VERSION);
        return Result::createError(message);
    }

    auto props = root["properties"].toObject();
    auto it = props.constBegin(), end = props.constEnd();
    for (; it != end; ++it) {
        sound->setProperty(it.key().toUtf8(), it.value().toVariant());
    }
    return {};
}

Result saveSfxj(const Sound* sound, QIODevice* device) {
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
    if (device->write(json) == -1) {
        auto message = QCoreApplication::translate("SoundIO", "Failed to save file: %1.")
                           .arg(device->errorString());
        return Result::createError(message);
    }
    return {};
}

} // namespace SoundIO
