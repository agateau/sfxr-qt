#include "historymodel.h"

#include "sound.h"

static const int HISTORY_MAX_SIZE = 30;

HistoryModel::HistoryModel(QObject* parent)
    : QAbstractListModel(parent) {
    append(tr("Start"), new Sound);
}

int HistoryModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mItems.length();
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= mItems.length()) {
        return QVariant();
    }
    SoundInfo info = mItems.at(row);
    switch (role) {
    case TextRole:
        return info.text;
    case SoundRole:
        return QVariant::fromValue(info.sound);
    }
    return QVariant();
}

QHash<int, QByteArray> HistoryModel::roleNames() const {
    return {
        { TextRole, "text" },
        { SoundRole, "sound" },
    };
}

void HistoryModel::append(const QString& text, Sound* sound) {
    SoundInfo info;
    if (mItems.length() == HISTORY_MAX_SIZE) {
        beginRemoveRows(QModelIndex(), mItems.length() - 1, mItems.length() - 1);
        info = mItems.takeLast();
        delete info.sound;
        endRemoveRows();
    }
    sound->setParent(this);
    info.sound = sound;
    info.text = text;

    beginInsertRows(QModelIndex(), 0, 0);
    mItems.prepend(info);
    endInsertRows();
}
