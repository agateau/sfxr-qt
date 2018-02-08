#ifndef SOUNDLISTMODEL_H
#define SOUNDLISTMODEL_H

#include "basesoundlistmodel.h"

#include <memory>

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

    Q_INVOKABLE void addNew(const QString& text, Sound* sound);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void setCurrentRow(int row);

    int count() const override;

    Sound* currentSound() const override;

private:
    struct SoundInfo {
        QString text;
        std::unique_ptr<Sound> sound;
    };

    std::vector<SoundInfo> mItems;
    // We store the current sound as a pointer rather than a row so that it
    // does not change when rows are removed before it.
    Sound* mCurrentSound = nullptr;
};

#endif // SOUNDLISTMODEL_H
