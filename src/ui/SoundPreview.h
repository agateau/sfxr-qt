#ifndef SOUNDPREVIEW_H
#define SOUNDPREVIEW_H

#include <QFutureWatcher>
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
    void onPlayPositionChanged(qreal position);
    void onPreviewReady();

    QFutureWatcher<QImage>* const mPreviewWatcher;
    SoundPlayer* mSoundPlayer = nullptr;
    QImage mPreview;
    qreal mPlayPosition = 0;
};

#endif // SOUNDPREVIEW_H
