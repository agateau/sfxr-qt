#include "soundlistmodel.h"

#include "sound.h"

SoundListModel::SoundListModel(QObject* parent)
    : BaseSoundListModel(parent) {
    addNew(tr("New"), new Sound);
    setCurrentRow(0);
}

int SoundListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mItems.size();
}

int SoundListModel::count() const {
    return mItems.size();
}

Sound* SoundListModel::currentSound() const {
    return mCurrentSound;
}

QVariant SoundListModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (row < 0 || row >= static_cast<int>(mItems.size())) {
        return QVariant();
    }
    const SoundInfo& info = mItems.at(row);
    switch (role) {
    case TextRole:
        return info.text;
    case SoundRole:
        return QVariant::fromValue(info.sound.get());
    }
    return QVariant();
}

QHash<int, QByteArray> SoundListModel::roleNames() const {
    return {
        { TextRole, "text" },
        { SoundRole, "sound" },
    };
}

void SoundListModel::addNew(const QString& text, Sound* sound) {
    SoundInfo info;
    info.sound.reset(sound);
    info.text = text;

    beginInsertRows(QModelIndex(), 0, 0);
    mItems.insert(mItems.begin(), std::move(info));
    endInsertRows();
    countChanged(count());
    setCurrentRow(0);
}

void SoundListModel::remove(int row) {
    int size = static_cast<int>(mItems.size());
    Q_ASSERT(row >= 0 && row < size);

    if (size == 1) {
        // Never remove the last item, reset it instead
        SoundInfo& info = mItems.at(0);
        info.sound->resetParams();
        info.text = tr("New");
        QModelIndex idx = index(0);
        dataChanged(idx, idx);
        return;
    }

    // If we are removing the current sound, select another one
    Sound* removedSound = mItems.at(row).sound.get();
    if (removedSound == mCurrentSound) {
        if (row == size - 1) {
            // Removing the last row, select the previous one
            setCurrentRow(row - 1);
        } else {
            // Select the next one
            setCurrentRow(row + 1);
        }
    }

    beginRemoveRows(QModelIndex(), row, row);
    mItems.erase(mItems.begin() + row);
    endRemoveRows();

    countChanged(count());
}

void SoundListModel::setCurrentRow(int row) {
    mCurrentSound = mItems.at(row).sound.get();
    currentSoundChanged(mCurrentSound);
}
