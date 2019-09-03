#ifndef SOUND_H
#define SOUND_H

#include "BaseSound.h"

class Sound : public BaseSound {
    Q_OBJECT
public:
    explicit Sound(QObject* parent = nullptr);

    void resetParams();
    void fromOther(const Sound* other);
    Q_INVOKABLE bool load(const QUrl& url);
    Q_INVOKABLE bool save(const QUrl& url);

    QString name() const override;

    bool hasRealUrl() const override;

    void setUrl(const QUrl& url) override;

    /**
     * Set the name of the sound until it is saved
     */
    void setUnsavedName(const QString& name);
};

Q_DECLARE_METATYPE(Sound*)

#endif // SOUND_H
