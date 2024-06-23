#include "SoundPreview.h"

#include "Sound.h"

#include <QPainter>
#include <QPainterPath>
#include <QtConcurrent>

static const QColor WAVE_BORDER_COLOR = Qt::white;
static const QColor WAVE_FILL_COLOR = QColor::fromRgbF(0.6, 0.6, 0.6);
static const QColor POSITION_COLOR = Qt::white;

SoundPreview::SoundPreview(QQuickItem* parent)
        : QQuickPaintedItem(parent), mPreviewWatcher(new QFutureWatcher<QImage>(this)) {
    setImplicitSize(100, 120);
    setAntialiasing(true);

    connect(
        mPreviewWatcher, &QFutureWatcher<QImage>::finished, this, &SoundPreview::onPreviewReady);
}

SoundPlayer* SoundPreview::soundPlayer() const {
    return mSoundPlayer;
}

void SoundPreview::setSoundPlayer(SoundPlayer* value) {
    if (mSoundPlayer == value) {
        return;
    }
    if (mSoundPlayer) {
        disconnect(mSoundPlayer);
    }
    mSoundPlayer = value;
    if (mSoundPlayer) {
        connect(mSoundPlayer, &SoundPlayer::soundChanged, this, &SoundPreview::updatePreview);
        connect(mSoundPlayer, &SoundPlayer::soundModified, this, &SoundPreview::updatePreview);
        connect(mSoundPlayer,
                &SoundPlayer::playPositionChanged,
                this,
                &SoundPreview::onPlayPositionChanged);
    }
    soundPlayerChanged(value);
}

void SoundPreview::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
    updatePreview();
}

struct MinMax {
    qreal min = 1;
    qreal max = -1;
};

static MinMax computeMinMax(const QVector<qreal>& samples, qreal from, qreal to) {
    int fromIdx = from * samples.length();
    int toIdx = qMax(int(to * samples.length()), fromIdx + 1);
    MinMax minMax;
    for (int idx = fromIdx; idx < toIdx; ++idx) {
        auto volume = samples[idx];
        minMax.min = qMin(minMax.min, volume);
        minMax.max = qMax(minMax.max, volume);
    }
    return minMax;
}

void SoundPreview::paint(QPainter* painter) {
    QRectF rect = {0, 0, width(), height()};
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect, 4, 4);
    if (mPreview.isNull()) {
        return;
    }

    painter->drawImage(0, 0, mPreview);
    qreal x = width() * mPlayPosition + 0.5;
    painter->setPen(POSITION_COLOR);
    painter->drawLine(x, 0, x, height());
}

static QImage generatePreviewImage(const QVector<qreal>& samples, qreal width, qreal height) {
    int iWidth = int(width);
    int iHeight = int(height);

    QVector<MinMax> minMaxes(iWidth);
    for (int x = 0; x < iWidth; ++x) {
        qreal from = qreal(x) / width;
        qreal to = qreal(x + 1) / width;
        minMaxes[x] = computeMinMax(samples, from, to);
    }

    qreal halfHeight = height / 2;

    QPainterPath path;

    for (int x = 0; x < iWidth; ++x) {
        qreal y = halfHeight - minMaxes[x].max * halfHeight;

        if (x == 0) {
            path.moveTo(0, y);
        } else {
            path.lineTo(x, y);
        }
    }

    // Go backward for min values to form a shape
    for (int x = iWidth - 1; x >= 0; --x) {
        qreal y = halfHeight - minMaxes[x].min * halfHeight;
        path.lineTo(x, y);
    }

    path.closeSubpath();

    QImage image(iWidth, iHeight, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    {
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(WAVE_BORDER_COLOR);
        painter.setBrush(WAVE_FILL_COLOR);
        painter.translate(0.5, 0.5);
        painter.drawPath(path);
    }
    return image;
}

void SoundPreview::onPreviewReady() {
    mPreview = mPreviewWatcher->result();
    update();
}

void SoundPreview::updatePreview() {
    if (!mSoundPlayer) {
        return;
    }

    if (width() <= 0 || height() <= 0) {
        return;
    }

    auto samples = mSoundPlayer->samples();
    if (samples.isEmpty()) {
        return;
    }
    QFuture<QImage> future = QtConcurrent::run(generatePreviewImage, samples, width(), height());
    mPreviewWatcher->setFuture(future);
}

void SoundPreview::onPlayPositionChanged(qreal position) {
    mPlayPosition = position;
    update();
}
