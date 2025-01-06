#pragma once

#include "../database/DatabaseManager.h"
#include <QVector>
#include <QPair>

class GroupRepository {
public:
    explicit GroupRepository(DatabaseManager* db);

    bool createGroupChat(const QString& groupname, const QString& username);
    bool createGroupMember(const QString& groupId, const QString& username, const QString& status);
    bool inviteGroupMember(const QString& groupId, const QString& username);
    bool acceptGroupInvitation(const QString& groupId, const QString& username);
    bool rejectGroupInvitation(const QString& groupId, const QString& username);
    bool leaveGroupChat(const QString& groupId, const QString& username);
    bool sendMessageToGroup(const QString& groupId, const QString& sender, const QString& message);
    QPair<QString, QString> getGroupById(const QString& groupId);
    QVector<QPair<QString, QString>> getGroupMembers(const QString& groupId);
    QVector<QPair<QString, QString>> getSentGroupInvitations(const QString& username);
    QVector<QPair<QString, QString>> getReceivedGroupInvitations(const QString& username); 
    QVector<QPair< QString, QPair<QString, QString>>> getGroupMessages(const QString& groupId);
    QVector<QPair<QString, QString>> getUserGroups(const QString& username);

private:
    DatabaseManager* db;
};