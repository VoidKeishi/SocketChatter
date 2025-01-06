#include "GroupRepository.h"
#include "../utils/Logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QVector>
#include <QPair>
#include <QDateTime>

// Constructor
GroupRepository::GroupRepository(DatabaseManager* dbManager)
    : db(dbManager)
{}

// Create a new group chat
bool GroupRepository::createGroupChat(const QString& groupName, const QString& creator) {
    if (groupName.trimmed().isEmpty()) {
        Logger::error("Group name cannot be empty.");
        return false;
    }

    QSqlQuery query(db->database());
    query.prepare("INSERT INTO groups (group_name, creator, created_at) VALUES (?, ?, ?)");
    query.addBindValue(groupName);
    query.addBindValue(creator);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        Logger::error("Failed to create group: " + query.lastError().text());
        return false;
    }

    // Add creator as a member without status
    return addMember(groupName, creator);
}

// Delete a group chat
bool GroupRepository::deleteGroupChat(const QString& groupName, const QString& sender) {
    // Verify sender is the creator
    QString creator = getGroupCreator(groupName);
    if (creator != sender) {
        Logger::error("Only the group creator can delete the group.");
        return false;
    }

    QSqlQuery query(db->database());
    query.prepare("DELETE FROM groups WHERE group_name = ?");
    query.addBindValue(groupName);

    if (!query.exec()) {
        Logger::error("Failed to delete group: " + query.lastError().text());
        return false;
    }

    // Due to ON DELETE CASCADE, related records in group_members and group_invitations are deleted automatically
    return true;
}

// Invite a member to the group
bool GroupRepository::inviteMember(const QString& groupName, const QString& sender, const QString& invitee) {
    // Check if sender is a member
    if (!isMember(groupName, sender)) {
        Logger::error("Sender is not a member of the group.");
        return false;
    }

    QSqlQuery query(db->database());
    query.prepare("INSERT INTO group_invitations (group_name, inviter, invitee, timestamp) VALUES (?, ?, ?, ?)");
    query.addBindValue(groupName);
    query.addBindValue(sender);
    query.addBindValue(invitee);
    query.addBindValue(QDateTime::currentSecsSinceEpoch());

    if (!query.exec()) {
        Logger::error("Failed to invite member: " + query.lastError().text());
        return false;
    }

    return true;
}

// Accept a group invitation
bool GroupRepository::acceptGroupInvitation(const QString& groupName, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("DELETE FROM group_invitations WHERE group_name = ? AND invitee = ?");
    query.addBindValue(groupName);
    query.addBindValue(username);

    if (!query.exec()) {
        Logger::error("Failed to accept invitation: " + query.lastError().text());
        return false;
    }

    // Add member to group without status
    return addMember(groupName, username);
}

// Reject a group invitation
bool GroupRepository::rejectGroupInvitation(const QString& groupName, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("DELETE FROM group_invitations WHERE group_name = ? AND invitee = ?");
    query.addBindValue(groupName);
    query.addBindValue(username);

    if (!query.exec()) {
        Logger::error("Failed to reject invitation: " + query.lastError().text());
        return false;
    }

    // Optionally, you can log or handle the rejection if needed
    return true;
}

// Get the creator of the group
QString GroupRepository::getGroupCreator(const QString& groupName) {
    QSqlQuery query(db->database());
    query.prepare("SELECT creator FROM groups WHERE group_name = ?");
    query.addBindValue(groupName);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

// Get members of the group by group name
QList<QPair<QString, QString>> GroupRepository::getGroupMembersByName(const QString& groupName) {
    QList<QPair<QString, QString>> members;
    QSqlQuery query(db->database());
    query.prepare("SELECT member FROM group_members WHERE group_name = ?");
    query.addBindValue(groupName);

    if (query.exec()) {
        while (query.next()) {
            QString member = query.value(0).toString();
            members.append(qMakePair(member, QString())); // Status removed
        }
    } else {
        Logger::error("Error retrieving group members: " + query.lastError().text());
    }

    return members;
}

// Get groups a user belongs to
QList<QPair<QString, QString>> GroupRepository::getUserGroups(const QString& username) {
    QList<QPair<QString, QString>> groups;
    QSqlQuery query(db->database());
    query.prepare(R"(
        SELECT g.group_name, g.creator
        FROM groups g
        JOIN group_members gm ON g.group_name = gm.group_name
        WHERE gm.member = ?
    )");
    query.addBindValue(username);

    if (!query.exec()) {
        Logger::error("Error fetching user groups: " + query.lastError().text());
        return groups;
    }

    while (query.next()) {
        QString groupName = query.value("group_name").toString();
        QString creator = query.value("creator").toString();
        groups.append(qMakePair(groupName, creator));
    }

    return groups;
}

// Helper method to add a member to a group without status
bool GroupRepository::addMember(const QString& groupName, const QString& username) {
    QSqlQuery query(db->database());
    // Fixed SQL statement
    query.prepare("INSERT INTO group_members (group_name, member, created_at) VALUES (?, ?, ?)");
    query.addBindValue(groupName);
    query.addBindValue(username);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        Logger::error("Failed to add member: " + query.lastError().text());
        return false;
    }

    return true;
}

// Helper method to check if a user is a member of a group
bool GroupRepository::isMember(const QString& groupName, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("SELECT COUNT(*) FROM group_members WHERE group_name = ? AND member = ?");
    query.addBindValue(groupName);
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}