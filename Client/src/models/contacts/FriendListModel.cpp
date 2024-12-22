#include "FriendListModel.h"
#include "../../database/DatabaseManager.h"
#include "../../controllers/session/UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

FriendListModel::FriendListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int FriendListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_friends.count();
}

QVariant FriendListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_friends.count())
        return QVariant();

    const QString& friend_username = m_friends.at(index.row());

    switch (role) {
        case UsernameRole:
            return friend_username;
        case StatusRole:
            return "Online"; // TODO: Implement online status
        case AvatarRole:
            return "qrc:/src/views/assets/person.png";
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> FriendListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UsernameRole] = "username";
    roles[StatusRole] = "status";
    roles[AvatarRole] = "avatar";
    return roles;
}

void FriendListModel::refresh()
{
    beginResetModel();
    m_friends.clear();
    
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT friend_username FROM friends WHERE username = ?");
    query.addBindValue(UserManager::instance()->currentUser());
    
    if (query.exec()) {
        while (query.next()) {
            m_friends.append(query.value(0).toString());
        }
    } else {
        qWarning() << "Failed to fetch friends:" << query.lastError().text();
    }
    
    endResetModel();
    emit countChanged();
}

void FriendListModel::addFriend(const QString& username)
{
    if (!m_friends.contains(username)) {
        beginInsertRows(QModelIndex(), m_friends.count(), m_friends.count());
        m_friends.append(username);
        
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare("INSERT INTO friends (username, friend_username) VALUES (?, ?)");
        query.addBindValue(UserManager::instance()->currentUser());
        query.addBindValue(username);
        
        if (!query.exec()) {
            qWarning() << "Failed to add friend to database:" << query.lastError().text();
        }
        
        endInsertRows();
        emit countChanged();
    }
}

void FriendListModel::removeFriend(const QString& username)
{
    int index = m_friends.indexOf(username);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_friends.removeAt(index);
        
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare("DELETE FROM friends WHERE username = ? AND friend_username = ?");
        query.addBindValue(UserManager::instance()->currentUser());
        query.addBindValue(username);
        
        if (!query.exec()) {
            qWarning() << "Failed to remove friend from database:" << query.lastError().text();
        }
        
        endRemoveRows();
        emit countChanged();
    }
}

void FriendListModel::updateFriendsList(const QStringList& friends) {
    beginResetModel();
    m_friends = friends;
    
    // Update database
    QSqlDatabase db = DatabaseManager::instance()->database();
    db.transaction();
    
    QSqlQuery query(db);
    // Clear existing friends
    query.prepare("DELETE FROM friends WHERE username = ?");
    query.addBindValue(UserManager::instance()->currentUser());
    if (!query.exec()) {
        qWarning() << "Failed to clear friends:" << query.lastError().text();
        db.rollback();
        return;
    }
    
    // Insert new friends
    query.prepare("INSERT INTO friends (username, friend_username) VALUES (?, ?)");
    QString currentUser = UserManager::instance()->currentUser();
    for (const QString& friend_username : friends) {
        query.addBindValue(currentUser);
        query.addBindValue(friend_username);
        if (!query.exec()) {
            qWarning() << "Failed to add friend:" << query.lastError().text();
            db.rollback();
            return;
        }
    }
    
    db.commit();
    endResetModel();
    emit countChanged();
}