#pragma once
#include "../database/DatabaseManager.h"
#include <QVector>
#include <QPair>

class ContactRepository {
public:
    explicit ContactRepository(DatabaseManager* db);
    
    bool createFriendRequest(const QString& from, const QString& to);
    bool updateRequestStatus(const QString& from, const QString& to, const QString& status);
    QVector<QString> getFriendList(const QString& username);
    QVector<QPair<QString, QString>> getPendingRequests(const QString& username);
    bool areFriends(const QString& user1, const QString& user2);

private:
    DatabaseManager* db;
};