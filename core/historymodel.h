#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>

class Sound;

class HistoryModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Role {
        TextRole,
        SoundRole,
    };

    HistoryModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void append(Sound* sound);

private:
    QList<Sound*> mSounds;
};

#endif // HISTORYMODEL_H
