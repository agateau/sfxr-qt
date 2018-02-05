#ifndef SOUND_H
#define SOUND_H

#include "basesound.h"

class Sound : public BaseSound {
    Q_OBJECT
public:
    explicit Sound(QObject* parent = nullptr);

    void resetParams();
    Q_INVOKABLE void fromOther(Sound* other);
    Q_INVOKABLE bool load(const QUrl& url);
    Q_INVOKABLE bool save(const QUrl& url);
};

Q_DECLARE_METATYPE(Sound*)

#endif // SOUND_H
