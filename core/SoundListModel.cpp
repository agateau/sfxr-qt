#include "SoundListModel.h"

#include "Sound.h"

#include <algorithm>

SoundListModel::SoundListModel(QObject* parent)
    : BaseSoundListModel(parent) {
    addNew(new Sound);
}

int SoundListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mItems.size();
}

int SoundListModel::count() const {
    return mItems.size();
}

Sound* SoundListModel::soundForRow(int row) const {
    return mItems.at(row).get();
}

QVariant SoundListModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= static_cast<int>(mItems.size())) {
        return QVariant();
    }
    Sound* sound = soundForRow(row);
    switch (role) {
    case TextRole:
        return sound->name();
    case SoundRole:
        return QVariant::fromValue(sound);
    }
    return QVariant();
}

QHash<int, QByteArray> SoundListModel::roleNames() const {
    return {
        { TextRole, "text" },
        { SoundRole, "sound" },
    };
}

void SoundListModel::addNew(Sound* sound) {
    beginInsertRows(QModelIndex(), 0, 0);
    mItems.insert(mItems.begin(), std::unique_ptr<Sound>(sound));
    endInsertRows();
    connect(sound, &Sound::nameChanged, this, [this, sound] {
        onSoundNameChanged(sound);
    });
    countChanged(count());
}

void SoundListModel::remove(int row) {
    int size = static_cast<int>(mItems.size());
    Q_ASSERT(row >= 0 && row < size);

    beginRemoveRows(QModelIndex(), row, row);
    mItems.erase(mItems.begin() + row);
    endRemoveRows();

    countChanged(count());
}

void SoundListModel::resetSoundAtRow(int row) {
    soundForRow(row)->resetParams();
    QModelIndex idx = index(row);
    dataChanged(idx, idx);
}

void SoundListModel::onSoundNameChanged(Sound* sound) {
    auto it = std::find_if(mItems.begin(), mItems.end(), [sound](const std::unique_ptr<Sound>& rowSound) {
        return sound == rowSound.get();
    });
    Q_ASSERT(it != mItems.end());
    int row = int(std::distance(mItems.begin(), it));
    QModelIndex idx = index(row);
    dataChanged(idx, idx);
}
