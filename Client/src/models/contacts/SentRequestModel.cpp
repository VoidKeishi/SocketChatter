#include "SentRequestModel.h"
#include "../../database/DatabaseManager.h"
#include "../../controllers/session/UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SentRequestModel::SentRequestModel(QObject* parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int SentRequestModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_requests.count();
}

QVariant SentRequestModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_requests.count())
        return QVariant();

    const QString& username = m_requests.at(index.row());

    switch (role) {
        case UsernameRole:
            return username;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> SentRequestModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UsernameRole] = "username";
    return roles;
}

void SentRequestModel::refresh()
{
    beginResetModel();
    m_requests.clear();
    
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT to_user FROM friend_requests WHERE from_user = ? AND status = 'pending'");
    query.addBindValue(UserManager::instance()->currentUser());
    
    if (query.exec()) {
        while (query.next()) {
            m_requests.append(query.value(0).toString());
        }
    }
    
    endResetModel();
    emit countChanged();
}

void SentRequestModel::onFriendRequestSent(const QString& username)
{
    beginInsertRows(QModelIndex(), m_requests.count(), m_requests.count());
    m_requests.append(username);
    
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("INSERT INTO friend_requests (from_user, to_user, status) VALUES (?, ?, 'pending')");
    query.addBindValue(UserManager::instance()->currentUser());
    query.addBindValue(username);
    
    if (!query.exec()) {
        qWarning() << "Failed to save sent request:" << query.lastError().text();
    }
    
    endInsertRows();
    emit countChanged();
}

void SentRequestModel::removeRequest(const QString& username)
{
    int index = m_requests.indexOf(username);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_requests.removeAt(index);
        
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare("UPDATE friend_requests SET status = 'cancelled' WHERE from_user = ? AND to_user = ?");
        query.addBindValue(UserManager::instance()->currentUser());
        query.addBindValue(username);
        
        if (!query.exec()) {
            qWarning() << "Failed to cancel request:" << query.lastError().text();
        }
        
        endRemoveRows();
        emit countChanged();
    }
}