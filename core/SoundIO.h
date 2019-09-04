#ifndef SOUNDIO_H
#define SOUNDIO_H

class Sound;

class QString;
class QUrl;

namespace SoundIO {

bool load(Sound* sound, const QUrl& url);
bool save(const Sound* sound, const QUrl& url);

} // SoundIO

#endif // SOUNDIO_H
