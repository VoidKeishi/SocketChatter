#include "ReceivedRequestModel.h"
#include "../../database/DatabaseManager.h"
#include "../../controllers/session/UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ReceivedRequestModel::ReceivedRequestModel(QObject* parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int ReceivedRequestModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_requests.count();
}

QVariant ReceivedRequestModel::data(const QModelIndex& index, int role) const
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

QHash<int, QByteArray> ReceivedRequestModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UsernameRole] = "username";
    return roles;
}

void ReceivedRequestModel::refresh()
{
    beginResetModel();
    m_requests.clear();
    
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT from_user FROM friend_requests WHERE to_user = ? AND status = 'pending'");
    query.addBindValue(UserManager::instance()->currentUser());
    
    if (query.exec()) {
        while (query.next()) {
            m_requests.append(query.value(0).toString());
        }
    }
    
    endResetModel();
    emit countChanged();
}

void ReceivedRequestModel::onFriendRequestResponse(const QString& username, )
{
    int index = m_requests.indexOf(username);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_requests.removeAt(index);
        
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare("UPDATE friend_requests SET status = 'accepted' WHERE from_user = ? AND to_user = ?");
        query.addBindValue(username);
        query.addBindValue(UserManager::instance()->currentUser());
        
        if (!query.exec()) {
            qWarning() << "Failed to update request status:" << query.lastError().text();
        }
        
        endRemoveRows();
        emit countChanged();
    }
}

void ReceivedRequestModel::addRequest(const QString& username)
{
    if (!m_requests.contains(username)) {
        beginInsertRows(QModelIndex(), m_requests.count(), m_requests.count());
        m_requests.append(username);
        endInsertRows();
        emit countChanged();
    }
}