#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QObject>

namespace WaveForm {
Q_NAMESPACE

// Must be kept in sync with WaveFormSelector.qml
enum Enum {
    // These 4 values must not change to keep compatibility with .sfxr files
    Square=0,
    Sawtooth=1,
    Sine=2,
    Noise=3,
};

Enum random(const std::vector<Enum>& values);

void registerType();

Q_ENUM_NS(Enum)
}

#endif // WAVEFORM_H
