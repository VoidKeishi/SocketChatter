#pragma once
#include <QAbstractListModel>
#include <QStringList>

class FriendListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum Roles {
        UsernameRole = Qt::UserRole + 1,
        StatusRole,
        AvatarRole
    };

    explicit FriendListModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void refresh();
    void addFriend(const QString& username);
    void removeFriend(const QString& username);
    void updateFriendsList(const QStringList& friends);

signals:
    void countChanged();

private:
    QStringList m_friends;
};