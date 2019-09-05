#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QObject>

namespace WaveForm {
Q_NAMESPACE

// Must be kept in sync with WaveFormSelector.qml
enum Enum {
    Square,
    Sawtooth,
    Sine,
    Noise,
    Last = Noise,
};

Enum random(int max);

void registerType();

Q_ENUM_NS(Enum)
}

#endif // WAVEFORM_H
