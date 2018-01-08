#ifndef WAVSAVER_H
#define WAVSAVER_H

#include <QObject>

class QUrl;

class Synthesizer;

class WavSaver : public QObject {
    Q_OBJECT
public:
    explicit WavSaver(QObject* parent = nullptr);

    Q_INVOKABLE void save(Synthesizer* synth, const QUrl& url);
};

#endif // WAVSAVER_H
