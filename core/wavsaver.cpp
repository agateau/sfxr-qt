#include "wavsaver.h"

#include "sound.h"
#include "synthesizer.h"

WavSaver::WavSaver(QObject* parent)
    : QObject(parent) {

}

void WavSaver::save(Sound* sound, const QUrl& url) {
    Synthesizer synth;
    synth.setSound(sound);
    synth.exportWav(url);
}
