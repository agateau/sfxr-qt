#ifndef SOUND_H
#define SOUND_H

#include "basesound.h"

class Sound : public BaseSound {
    Q_OBJECT
public:
    explicit Sound(QObject* parent = nullptr);

    void resetParams();
    bool loadSettings(char* filename);
    bool saveSettings(char* filename);

private:
    float p_vib_delay;
    bool filter_on;
};

#endif // SOUND_H
