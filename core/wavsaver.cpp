#include "wavsaver.h"

#include "synthesizer.h"

WavSaver::WavSaver(QObject* parent)
    : QObject(parent) {

}

void WavSaver::save(Synthesizer* synth, const QUrl& url) {
    synth->exportWav(url);
}
