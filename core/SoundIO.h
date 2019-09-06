#ifndef SOUNDIO_H
#define SOUNDIO_H

class Sound;

class QIODevice;
class QString;
class QUrl;

namespace SoundIO {

bool load(Sound* sound, const QUrl& url);

bool save(const Sound* sound, const QUrl& url);

bool loadSfxr(Sound* sound, QIODevice* device);

bool loadSfxj(Sound* sound, QIODevice* device);

bool saveSfxr(const Sound* sound, QIODevice* device);

bool saveSfxj(const Sound* sound, QIODevice* device);

} // SoundIO

#endif // SOUNDIO_H
