#include "historymodel.h"

#include "sound.h"

static const int HISTORY_MAX_SIZE = 30;

HistoryModel::HistoryModel(QObject* parent)
    : BaseHistoryModel(parent) {
    addNew(tr("Start"), new Sound);
    setCurrentRow(0);
}

int HistoryModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mItems.size();
}

int HistoryModel::count() const {
    return mItems.size();
}

Sound* HistoryModel::currentSound() const {
    return mCurrentSound;
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const {
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

QHash<int, QByteArray> HistoryModel::roleNames() const {
    return {
        { TextRole, "text" },
        { SoundRole, "sound" },
    };
}

void HistoryModel::addNew(const QString& text, Sound* sound) {
    if (mItems.size() == HISTORY_MAX_SIZE) {
        beginRemoveRows(QModelIndex(), mItems.size() - 1, mItems.size() - 1);
        mItems.pop_back();
        endRemoveRows();
    }
    SoundInfo info;
    info.sound.reset(sound);
    info.text = text;

    beginInsertRows(QModelIndex(), 0, 0);
    mItems.insert(mItems.begin(), std::move(info));
    endInsertRows();
    countChanged(count());
    setCurrentRow(0);
}

void HistoryModel::remove(int row) {
    int size = static_cast<int>(mItems.size());
    Q_ASSERT(row >= 0 && row < size);

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

void HistoryModel::setCurrentRow(int row) {
    mCurrentSound = mItems.at(row).sound.get();
    currentSoundChanged(mCurrentSound);
}
