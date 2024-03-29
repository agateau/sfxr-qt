#ifndef WAVSAVER_H
#define WAVSAVER_H

#include "BaseWavSaver.h"

#include <QObject>

class QUrl;

class Sound;

class WavSaver : public BaseWavSaver {
    Q_OBJECT
public:
    explicit WavSaver(QObject* parent = nullptr);

    Q_INVOKABLE bool save(Sound* sound, const QUrl& url);
};

#endif // WAVSAVER_H
