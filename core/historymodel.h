#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>

class Sound;

class HistoryModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
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
    Q_INVOKABLE void remove(int row);

    int count() const;

signals:
    void countChanged(int count);

private:
    struct SoundInfo {
        QString text;
        Sound* sound;
    };

    QList<SoundInfo> mItems;
};

#endif // HISTORYMODEL_H
