#include "WavSaver.h"

#include "Sound.h"
#include "Synthesizer.h"

#include <QFile>
#include <QUrl>
#include <QtEndian>

class WavExportStrategy : public Synthesizer::SynthStrategy {
public:
    int file_sampleswritten;
    qreal filesample = 0.0f;
    int fileacc = 0;
    int wav_bits = 16;
    int wav_freq = 44100;

    ~WavExportStrategy() {
    }

    bool open(const QString& path) {
        auto file = std::make_unique<QFile>(path);
        if (!file->open(QIODevice::WriteOnly)) {
            return false;
        }
        mDevice = std::move(file);
        return true;
    }

    qint64 fwrite(const void* ptr, size_t size) {
        return mDevice->write(reinterpret_cast<const char*>(ptr), size);
    }

    qint64 ftell() const {
        return mDevice->pos();
    }

    void fseek(qint64 offset) {
        mDevice->seek(offset);
    }

    void fwriteUInt16(quint16 value) {
        value = qToLittleEndian(value);
        mDevice->write(reinterpret_cast<char*>(&value), 2);
    }

    void fwriteUInt32(quint32 value) {
        value = qToLittleEndian(value);
        mDevice->write(reinterpret_cast<char*>(&value), 4);
    }

    void fwriteInt16(qint16 value) {
        value = qToLittleEndian(value);
        mDevice->write(reinterpret_cast<char*>(&value), 2);
    }

    // Synthesizer::SynthStrategy implementation
    void write(qreal sample) override;

private:
    std::unique_ptr<QIODevice> mDevice;
};

void WavExportStrategy::write(qreal ssample) {
    // quantize depending on format
    // accumulate/count to accomodate variable sample rate?
    ssample = qBound(-1.0, ssample, 1.0);
    filesample += ssample;
    fileacc++;
    if (wav_freq == 44100 || fileacc == 2) {
        filesample /= fileacc;
        fileacc = 0;
        if (wav_bits == 16) {
            qint16 isample = qint16(filesample * 32000);
            fwriteInt16(isample);
        } else {
            quint8 isample = quint8(filesample * 127 + 128);
            fwrite(&isample, 1);
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
    wav.fwrite("RIFF", 4); // "RIFF"
    wav.fwriteUInt32(0);   // remaining file size
    wav.fwrite("WAVE", 4); // "WAVE"

    wav.fwrite("fmt ", 4);          // "fmt "
    wav.fwriteUInt32(16);           // chunk size
    wav.fwriteUInt16(1);            // compression code
    wav.fwriteUInt16(1);            // channels
    wav.fwriteUInt32(wav.wav_freq); // sample rate
    quint64 bytesPerSec = wav.wav_freq * wav.wav_bits / 8;
    wav.fwriteUInt32(bytesPerSec);      // bytes/sec
    wav.fwriteUInt16(wav.wav_bits / 8); // block align
    wav.fwriteUInt16(wav.wav_bits);     // bits per sample

    wav.fwrite("data", 4); // "data"
    auto foutstream_datasize = wav.ftell();
    wav.fwriteUInt32(foutstream_datasize); // chunk size

    // write sample data
    wav.file_sampleswritten = 0;
    wav.filesample = 0.0;
    wav.fileacc = 0;

    Synthesizer synth;
    synth.init(sound);
    while (synth.synthSample(256, &wav)) {
    }

    // seek back to header and write size info
    wav.fseek(4);
    quint64 remainingFileSize =
        foutstream_datasize - 4 + wav.file_sampleswritten * wav.wav_bits / 8;
    wav.fwriteUInt32(remainingFileSize); // remaining file size
    wav.fseek(foutstream_datasize);
    quint64 dataChunkSize = wav.file_sampleswritten * wav.wav_bits / 8;
    wav.fwriteUInt32(dataChunkSize); // chunk size (data)

    return true;
}
