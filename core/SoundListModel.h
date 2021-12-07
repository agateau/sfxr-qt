#ifndef SOUNDLISTMODEL_H
#define SOUNDLISTMODEL_H

#include <memory>

#include "BaseSoundListModel.h"

class Sound;

class SoundListModel : public BaseSoundListModel {
    Q_OBJECT
public:
    enum Role {
        TextRole,
        SoundRole,
    };

    SoundListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addNew(Sound* sound);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE Sound* soundForRow(int row) const;
    Q_INVOKABLE void resetSoundAtRow(int row);

    int count() const override;

private:
    void onSoundNameChanged(Sound* sound);

    std::vector<std::unique_ptr<Sound>> mItems;
};

#endif // SOUNDLISTMODEL_H
