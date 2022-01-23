#ifndef SOUNDPREVIEW_H
#define SOUNDPREVIEW_H

#include <QImage>
#include <QObject>
#include <QQuickPaintedItem>

#include <SoundPlayer.h>

class SoundPreview : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(
        SoundPlayer* soundPlayer READ soundPlayer WRITE setSoundPlayer NOTIFY soundPlayerChanged)

public:
    explicit SoundPreview(QQuickItem* parent = nullptr);

    SoundPlayer* soundPlayer() const;
    void setSoundPlayer(SoundPlayer* value);

    void paint(QPainter* painter) override;

signals:
    void soundPlayerChanged(SoundPlayer* soundPlayer);

private:
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
    void updatePreview();

    SoundPlayer* mSoundPlayer = nullptr;
    QImage mPreview;
};

#endif // SOUNDPREVIEW_H
