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

    Q_INVOKABLE void append(const QString& text, Sound* sound);

private:
    struct SoundInfo {
        QString text;
        Sound* sound;
    };

    QList<SoundInfo> mItems;
};

#endif // HISTORYMODEL_H
