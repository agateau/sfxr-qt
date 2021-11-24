#include "WavSaver.h"

#include <QUrl>

#include "Sound.h"
#include "Synthesizer.h"

class WavExportStrategy : public Synthesizer::SynthStrategy {
public:
    int file_sampleswritten;
    qreal filesample = 0.0f;
    int fileacc = 0;
    int wav_bits = 16;
    int wav_freq = 44100;

    ~WavExportStrategy() {
        fclose(file);
    }

    bool open(const QString& path) {
        file = fopen(path.toLocal8Bit().constData(), "wb");
        return bool(file);
    }

    size_t fwrite(const void *ptr, size_t size, size_t nmemb) {
        return ::fwrite(ptr, size, nmemb, file);
    }

    long ftell() {
        return ::ftell(file);
    }

    int fseek(long offset, int whence) {
        return ::fseek(file, offset, whence);
    }

    // Synthesizer::SynthStrategy implementation
    void write(qreal sample) override;

private:
    FILE* file = nullptr;
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
            fwrite(&isample, 1, 2);
        } else {
            unsigned char isample = (unsigned char)(filesample * 127 + 128);
            fwrite(&isample, 1, 1);
        }
        filesample = 0.0;
    }
    file_sampleswritten++;
}

WavSaver::WavSaver(QObject* parent) : BaseWavSaver(parent) {
}

bool WavSaver::save(Sound* sound, const QUrl& url) {
    QString path = url.path();
    WavExportStrategy wav;
    if (!wav.open(path)) {
        return false;
    }
    wav.wav_bits = bits();
    wav.wav_freq = frequency();

    // write wav header
    unsigned int dword = 0;
    unsigned short word = 0;
    wav.fwrite("RIFF", 4, 1); // "RIFF"
    dword = 0;
    wav.fwrite(&dword, 1, 4); // remaining file size
    wav.fwrite("WAVE", 4, 1); // "WAVE"

    wav.fwrite("fmt ", 4, 1); // "fmt "
    dword = 16;
    wav.fwrite(&dword, 1, 4); // chunk size
    word = 1;
    wav.fwrite(&word, 1, 2); // compression code
    word = 1;
    wav.fwrite(&word, 1, 2); // channels
    dword = wav.wav_freq;
    wav.fwrite(&dword, 1, 4); // sample rate
    dword = wav.wav_freq * wav.wav_bits / 8;
    wav.fwrite(&dword, 1, 4); // bytes/sec
    word = wav.wav_bits / 8;
    wav.fwrite(&word, 1, 2); // block align
    word = wav.wav_bits;
    wav.fwrite(&word, 1, 2); // bits per sample

    wav.fwrite("data", 4, 1); // "data"
    dword = 0;
    int foutstream_datasize = wav.ftell();
    wav.fwrite(&dword, 1, 4); // chunk size

    // write sample data
    wav.file_sampleswritten = 0;
    wav.filesample = 0.0;
    wav.fileacc = 0;

    Synthesizer synth;
    synth.init(sound);
    while (synth.synthSample(256, &wav)) {
    }

    // seek back to header and write size info
    wav.fseek(4, SEEK_SET);
    dword = 0;
    dword = foutstream_datasize - 4 + wav.file_sampleswritten * wav.wav_bits / 8;
    wav.fwrite(&dword, 1, 4); // remaining file size
    wav.fseek(foutstream_datasize, SEEK_SET);
    dword = wav.file_sampleswritten * wav.wav_bits / 8;
    wav.fwrite(&dword, 1, 4); // chunk size (data)

    return true;
}
