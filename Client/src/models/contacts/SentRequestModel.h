#pragma once
#include <QAbstractListModel>
#include <QStringList>
#include <QDateTime>

class SentRequestModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum Roles {
        UsernameRole = Qt::UserRole + 1,
        TimestampRole
    };

    explicit SentRequestModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onFriendRequestSent(const QString& username);
    void removeRequest(const QString& username);
    void refresh();

signals:
    void countChanged();

private:
    QStringList m_requests;
};