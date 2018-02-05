#include "historymodel.h"

#include "sound.h"

static const int HISTORY_MAX_SIZE = 30;

HistoryModel::HistoryModel(QObject* parent)
    : QAbstractListModel(parent) {

}

int HistoryModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mSounds.length();
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= mSounds.length()) {
        return QVariant();
    }
    Sound* sound = mSounds.at(row);
    switch (role) {
    case TextRole:
        return QString::number((qint64)sound);
    case SoundRole:
        return QVariant::fromValue(sound);
    }
    return QVariant();
}

QHash<int, QByteArray> HistoryModel::roleNames() const {
    return {
        { TextRole, "text" },
        { SoundRole, "sound" },
    };
}

void HistoryModel::append(Sound* sound) {
    Sound* dst;
    if (mSounds.length() < HISTORY_MAX_SIZE) {
        dst = new Sound(this);
    } else {
        beginRemoveRows(QModelIndex(), mSounds.length() - 1, mSounds.length() - 1);
        dst = mSounds.takeLast();
        endRemoveRows();
    }
    dst->fromOther(sound);

    beginInsertRows(QModelIndex(), 0, 0);
    mSounds.prepend(dst);
    endInsertRows();
}
