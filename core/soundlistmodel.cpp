#include "soundlistmodel.h"

#include "sound.h"

SoundListModel::SoundListModel(QObject* parent)
    : BaseSoundListModel(parent) {
    addNew(tr("New"), new Sound);
}

int SoundListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : mItems.size();
}

int SoundListModel::count() const {
    return mItems.size();
}

Sound* SoundListModel::soundForRow(int row) const {
    return mItems.at(row).sound.get();
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
    SoundInfo& info = mItems.at(row);
    info.sound->resetParams();
    info.text = tr("New");
    QModelIndex idx = index(row);
    dataChanged(idx, idx);
}
