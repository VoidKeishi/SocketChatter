#pragma once

#include "../database/DatabaseManager.h"
#include <QVector>
#include <QPair>
#include <QString>
#include <QList>

class GroupRepository {
public:
    explicit GroupRepository(DatabaseManager* db);

    // Group Operations
    bool createGroupChat(const QString& groupName, const QString& creator);
    bool deleteGroupChat(const QString& groupName, const QString& sender);
    bool inviteMember(const QString& groupName, const QString& sender, const QString& invitee);
    bool acceptGroupInvitation(const QString& groupName, const QString& username);
    bool rejectGroupInvitation(const QString& groupName, const QString& username);
    QString getGroupCreator(const QString& groupName);
    QList<QPair<QString, QString>> getGroupMembersByName(const QString& groupName);
    QList<QPair<QString, QString>> getUserGroups(const QString& username);
    QString getGroupIdByName(const QString& groupName);

private:
    DatabaseManager* db;
    bool addMember(const QString& groupName, const QString& username);
    bool isMember(const QString& groupName, const QString& username);
};
   