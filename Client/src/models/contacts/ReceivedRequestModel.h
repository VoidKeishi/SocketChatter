#pragma once
#include <QAbstractListModel>
#include <QStringList>
#include <QDateTime>

class ReceivedRequestModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum Roles {
        UsernameRole = Qt::UserRole + 1,
        TimestampRole
    };

    explicit ReceivedRequestModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onFriendRequestResponse(const QString& username);
    void addRequest(const QString& username);
    void refresh();

signals:
    void countChanged();

private:
    QStringList m_requests;
};