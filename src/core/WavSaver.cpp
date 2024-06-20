#include "WavSaver.h"

#include "Sound.h"
#include "Synthesizer.h"

#include <QSaveFile>
#include <QUrl>
#include <QtEndian>

class WavExportStrategy : public Synthesizer::SynthStrategy {
public:
    WavExportStrategy(QIODevice* device, int bits, int frequency)
            : mDevice(device), mBits(bits), mFrequency(frequency) {
    }

    ~WavExportStrategy() {
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

    int sampleCount() const {
        return mSampleCount;
    }

private:
    QIODevice* const mDevice;
    const int mBits = 16;
    const int mFrequency = 44100;

    qreal mAccumulatedSamples = 0.0f;
    int mAccumulatedCount = 0;
    int mSampleCount = 0;
};

void WavExportStrategy::write(qreal ssample) {
    // quantize depending on format
    // accumulate/count to accomodate variable sample rate?
    ssample = qBound(-1.0, ssample, 1.0);
    mAccumulatedSamples += ssample;
    mAccumulatedCount++;
    if (mFrequency == 44100 || mAccumulatedCount == 2) {
        mAccumulatedSamples /= mAccumulatedCount;
        mAccumulatedCount = 0;
        if (mBits == 16) {
            qint16 isample = qint16(mAccumulatedSamples * 32000);
            fwriteInt16(isample);
        } else {
            quint8 isample = quint8(mAccumulatedSamples * 127 + 128);
            fwrite(&isample, 1);
        }
        mAccumulatedSamples = 0.0;
        mSampleCount++;
    }
}

WavSaver::WavSaver(QObject* parent) : BaseWavSaver(parent) {
}

bool WavSaver::save(Sound* sound, const QUrl& url) {
    QString path = url.path();
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    quint32 bytesPerBlock = bits() / 8;

    WavExportStrategy wav(&file, bits(), frequency());

    // RIFF chunk
    wav.fwrite("RIFF", 4);
    wav.fwriteUInt32(0); // remaining file size. Filled at the end
    wav.fwrite("WAVE", 4);

    // "fmt " chunk
    wav.fwrite("fmt ", 4);
    wav.fwriteUInt32(16);          // chunk size
    wav.fwriteUInt16(1);           // compression code
    wav.fwriteUInt16(1);           // channels
    wav.fwriteUInt32(frequency()); // sample rate
    quint64 bytesPerSec = frequency() * bytesPerBlock;
    wav.fwriteUInt32(bytesPerSec);   // bytes/sec
    wav.fwriteUInt16(bytesPerBlock); // block align
    wav.fwriteUInt16(bits());        // bits per sample

    // "data" chunk
    wav.fwrite("data", 4);
    auto foutstream_datasize = wav.ftell();
    wav.fwriteUInt32(0); // chunk size. Filled at the end

    // write sample data
    Synthesizer synth;
    synth.init(sound);
    while (synth.synthSample(256, &wav)) {
    }

    // Fill RIFF chunk
    auto fileSize = wav.ftell() - 8;
    wav.fseek(4);
    wav.fwriteUInt32(fileSize);

    // Fill data chunk
    wav.fseek(foutstream_datasize);
    quint32 dataChunkSize = wav.sampleCount() * bytesPerBlock;
    wav.fwriteUInt32(dataChunkSize);

    return file.commit();
}
