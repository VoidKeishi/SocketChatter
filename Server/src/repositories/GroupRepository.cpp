#include "GroupRepository.h"

GroupRepository::GroupRepository(DatabaseManager* db) : db(db) {}

bool GroupRepository::createGroupChat(const QString& groupname, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("INSERT INTO groups (group_name, creator, timestamp) VALUES (?, ?, ?)");
    query.addBindValue(groupname);
    query.addBindValue(username);
    query.addBindValue(QDateTime::currentSecsSinceEpoch());
    if (!query.exec()) {
        qDebug() << "Failed to create group chat:" << query.lastError();
        return false;
    }
    return true;
}

bool GroupRepository::createGroupMember(const QString& groupId, const QString& username, const QString& status) {
    QSqlQuery query(db->database());
    query.prepare("INSERT INTO group_members (group_id, member, status) VALUES (?, ?, ?)");
    query.addBindValue(groupId);
    query.addBindValue(username);
    query.addBindValue(status);
    if (!query.exec()) {
        qDebug() << "Failed to create group member:" << query.lastError();
        return false;
    }
    return true;
}

bool GroupRepository::inviteGroupMember(const QString& groupId, const QString& username) {
    return createGroupMember(groupId, username, "pending");
}

bool GroupRepository::acceptGroupInvitation(const QString& groupId, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("UPDATE group_members SET status = 'accepted' WHERE group_id = ? AND member = ?");
    query.addBindValue(groupId);
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to accept group invitation:" << query.lastError();
        return false;
    }
    return true;
}

bool GroupRepository::rejectGroupInvitation(const QString& groupId, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("UPDATE group_members SET status = 'rejected' WHERE group_id = ? AND member = ?");
    query.addBindValue(groupId);
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to reject group invitation:" << query.lastError();
        return false;
    }
    return true;
}

bool GroupRepository::leaveGroupChat(const QString& groupId, const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("DELETE FROM group_members WHERE group_id = ? AND member = ?");
    query.addBindValue(groupId);
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to leave group chat:" << query.lastError();
        return false;
    }
    return true;
}

QPair<QString, QString> GroupRepository::getGroupById(const QString& groupId) {
    QSqlQuery query(db->database());
    query.prepare("SELECT group_name, creator FROM groups WHERE id = ?");
    query.addBindValue(groupId);
    
    if (!query.exec()) {
        qDebug() << "Failed to get group by ID:" << query.lastError();
        return QPair<QString, QString>();
    }
    
    if (query.next()) {
        return QPair<QString, QString>(
            query.value(0).toString(),  // group_name
            query.value(1).toString()   // creator
        );
    }
    
    return QPair<QString, QString>();  // Return empty pair if not found
}


QVector<QPair<QString, QString>> GroupRepository::getGroupMembers(const QString& groupId) {
    QVector<QPair<QString, QString>> members;
    QSqlQuery query(db->database());
    
    query.prepare("SELECT member, status FROM group_members WHERE group_id = ?");
    query.addBindValue(groupId);
    if (!query.exec()) {
        qDebug() << "Failed to fetch group members:" << query.lastError();
        return members;
    }

    while (query.next()) {
        members.append(QPair<QString, QString>(
            query.value(0).toString(),  // username
            query.value(1).toString()   // status
        ));
    }
    return members;
}

QVector<QPair<QString, QString>> GroupRepository::getSentGroupInvitations(const QString& username) {
    QVector<QPair<QString, QString>> invitations;
    QSqlQuery query(db->database());
    
    query.prepare("SELECT g.id, g.group_name FROM groups g "
                 "INNER JOIN group_members gm ON g.id = gm.group_id "
                 "WHERE gm.member = ? AND gm.status = 'pending'");
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to fetch sent invitations:" << query.lastError();
        return invitations;
    }

    while (query.next()) {
        invitations.append(QPair<QString, QString>(
            query.value(0).toString(),  // group id
            query.value(1).toString()   // group name
        ));
    }
    return invitations;
}

QVector<QPair<QString, QString>> GroupRepository::getReceivedGroupInvitations(const QString& username) {
    QVector<QPair<QString, QString>> invitations;
    QSqlQuery query(db->database());
    
    query.prepare("SELECT g.id, g.group_name FROM groups g "
                 "INNER JOIN group_members gm ON g.id = gm.group_id "
                 "WHERE gm.member = ? AND gm.status = 'pending'");
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << "Failed to fetch received invitations:" << query.lastError();
        return invitations;
    }

    while (query.next()) {
        invitations.append(QPair<QString, QString>(
            query.value(0).toString(),  // group id
            query.value(1).toString()   // group name
        ));
    }
    return invitations;
}

QVector<QPair<QString, QString, QString>> GroupRepository::getGroupMessages(const QString& groupId) {
    QVector<QPair<QString, QString, QString>> messages;
    QSqlQuery query(db->database());
    
    query.prepare("SELECT sender, content, timestamp FROM group_messages "
                 "WHERE group_id = ? ORDER BY timestamp DESC");
    query.addBindValue(groupId);
    if (!query.exec()) {
        qDebug() << "Failed to fetch group messages:" << query.lastError();
        return messages;
    }

    while (query.next()) {
        messages.append(QPair<QString, QString, QString>(
            query.value(0).toString(),  // sender
            query.value(1).toString(),  // content 
            query.value(2).toString()   // timestamp
        ));
    }
    return messages;
}

QVector<QPair<QString, QString>> GroupRepository::getUserGroups(const QString& username) {
    QVector<QPair<QString, QString>> groups;
    QSqlQuery query(db->database());
    
    query.prepare("SELECT g.id, g.group_name FROM groups g "
                 "INNER JOIN group_members gm ON g.id = gm.group_id "
                 "WHERE gm.member = ? AND gm.status = 'accepted'");
    query.addBindValue(username);
    
    if (!query.exec()) {
        qDebug() << "Failed to fetch user groups:" << query.lastError();
        return groups;
    }

    while (query.next()) {
        groups.append(QPair<QString, QString>(
            query.value(0).toString(),  // group id
            query.value(1).toString()   // group name
        ));
    }
    return groups;
}