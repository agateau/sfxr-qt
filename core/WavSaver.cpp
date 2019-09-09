#include "WavSaver.h"

#include <QUrl>

#include "Sound.h"
#include "Synthesizer.h"

class WavExportStrategy : public Synthesizer::SynthStrategy {
public:
    FILE* file;
    int file_sampleswritten;
    qreal filesample = 0.0f;
    int fileacc = 0;
    int wav_bits = 16;
    int wav_freq = 44100;

    void write(qreal sample) override;
};

void WavExportStrategy::write(qreal ssample) {
    // quantize depending on format
    // accumulate/count to accomodate variable sample rate?
    ssample *= 4.0; // arbitrary gain to get reasonable output volume...
    ssample = qBound(-1.0, ssample, 1.0);
    filesample += ssample;
    fileacc++;
    if (wav_freq == 44100 || fileacc == 2) {
        filesample /= fileacc;
        fileacc = 0;
        if (wav_bits == 16) {
            short isample = (short)(filesample * 32000);
            fwrite(&isample, 1, 2, file);
        } else {
            unsigned char isample = (unsigned char)(filesample * 127 + 128);
            fwrite(&isample, 1, 1, file);
        }
        filesample = 0.0;
    }
    file_sampleswritten++;
}


WavSaver::WavSaver(QObject* parent)
    : BaseWavSaver(parent) {
}

bool WavSaver::save(Sound* sound, const QUrl& url) {
    QString path = url.path();
    FILE* foutput = fopen(path.toLocal8Bit().constData(), "wb");
    if (!foutput) {
        return false;
    }
    WavExportStrategy wav;
    wav.wav_bits = bits();
    wav.wav_freq = frequency();

    // write wav header
    unsigned int dword = 0;
    unsigned short word = 0;
    fwrite("RIFF", 4, 1, foutput); // "RIFF"
    dword = 0;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fwrite("WAVE", 4, 1, foutput); // "WAVE"

    fwrite("fmt ", 4, 1, foutput); // "fmt "
    dword = 16;
    fwrite(&dword, 1, 4, foutput); // chunk size
    word = 1;
    fwrite(&word, 1, 2, foutput); // compression code
    word = 1;
    fwrite(&word, 1, 2, foutput); // channels
    dword = wav.wav_freq;
    fwrite(&dword, 1, 4, foutput); // sample rate
    dword = wav.wav_freq * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // bytes/sec
    word = wav.wav_bits / 8;
    fwrite(&word, 1, 2, foutput); // block align
    word = wav.wav_bits;
    fwrite(&word, 1, 2, foutput); // bits per sample

    fwrite("data", 4, 1, foutput); // "data"
    dword = 0;
    int foutstream_datasize = ftell(foutput);
    fwrite(&dword, 1, 4, foutput); // chunk size

    // write sample data
    wav.file = foutput;
    wav.file_sampleswritten = 0;
    wav.filesample = 0.0;
    wav.fileacc = 0;

    Synthesizer synth;
    synth.init(sound);
    while (synth.synthSample(256, &wav)) {
    }

    // seek back to header and write size info
    fseek(foutput, 4, SEEK_SET);
    dword = 0;
    dword = foutstream_datasize - 4 + wav.file_sampleswritten * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fseek(foutput, foutstream_datasize, SEEK_SET);
    dword = wav.file_sampleswritten * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // chunk size (data)
    fclose(foutput);

    return true;
}
