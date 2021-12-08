#ifndef SOUNDIO_H
#define SOUNDIO_H

class Result;
class Sound;

class QIODevice;
class QString;
class QUrl;

namespace SoundIO {

Result load(Sound* sound, const QUrl& url);

Result save(const Sound* sound, const QUrl& url);

Result loadSfxr(Sound* sound, QIODevice* device);

Result loadSfxj(Sound* sound, QIODevice* device);

Result saveSfxr(const Sound* sound, QIODevice* device);

Result saveSfxj(const Sound* sound, QIODevice* device);

} // namespace SoundIO

#endif // SOUNDIO_H
