#ifndef WAVSAVER_H
#define WAVSAVER_H

#include <QObject>

class QUrl;

class Sound;

class WavSaver : public QObject {
    Q_OBJECT
public:
    explicit WavSaver(QObject* parent = nullptr);

    Q_INVOKABLE void save(Sound* sound, const QUrl& url);
};

#endif // WAVSAVER_H
