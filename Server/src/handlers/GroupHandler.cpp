#include "GroupHandler.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"
#include "../utils/NotificationFactory.h"
#include "../core/ConnectionManager.h"

GroupHandler::GroupHandler(DatabaseManager* db) 
    : BaseHandler(), groupRepo(db) 
{
    handlers = {
        {"GROUP_CREATE_REQUEST", [this](const QJsonObject& p) { handleCreateGroup(p); }},
        {"GROUP_FETCH_REQUEST", [this](const QJsonObject& p) { handleFetchGroup(p); }},
        {"GROUP_DELETE_REQUEST", [this](const QJsonObject& p) { handleDeleteGroup(p); }},
        {"GROUP_INVITE_REQUEST", [this](const QJsonObject& p) { handleGroupInvite(p); }},
        {"GROUP_INVITE_RESPONSE_REQUEST", [this](const QJsonObject& p) { handleGroupInviteResponse(p); }},
    };
}

void GroupHandler::handleCreateGroup(const QJsonObject& request) {
    QString groupName = request["groupName"].toString();
    QString creator = request["sender"].toString();
    QString groupDetails = request["groupDetails"].toString(); // Extract groupDetails

    if (groupRepo.createGroupChat(groupName, creator, groupDetails)) { // Pass groupDetails to repository
        emit responseReady(ResponseFactory::createGroupCreateResponse(true, "Group created successfully", groupName, groupDetails));
    } else {
        Logger::error("Failed to create group: " + groupName);
        emit responseReady(ResponseFactory::createGroupCreateResponse(false, "Failed to create group", groupName, groupDetails));
    }
}

void GroupHandler::handleFetchGroup(const QJsonObject& request) {
    QString sender = request["sender"].toString();

    auto groups = groupRepo.getUserGroups(sender);
    QList<QPair<QString, QString>> groupList; // List of pairs (groupId, groupName)
    for (const auto& group : groups) {
        groupList.append(group);
    }

    emit responseReady(ResponseFactory::createGroupFetchResponse(true, "Groups fetched successfully", groupList));
}

void GroupHandler::handleDeleteGroup(const QJsonObject& request) {
    QString sender = request["sender"].toString();
    QString groupName = request["groupName"].toString();

    if (groupRepo.deleteGroupChat(groupName, sender)) {
        emit responseReady(ResponseFactory::createGroupDeleteResponse(true, "Group deleted successfully", groupName));

        // Notify members
        auto members = groupRepo.getGroupMembersByName(groupName);
        QJsonArray memberArray;
        for (const auto& member : members) {
            memberArray.append(member.second);
        }
        emit responseReady(ResponseFactory::createGroupDeletedNotification("Group deleted successfully", groupName, memberArray));
    } else {
        Logger::error("Failed to delete group: " + groupName);
        emit responseReady(ResponseFactory::createGroupDeleteResponse(false, "Failed to delete group", groupName));
    }
}

void GroupHandler::handleGroupInvite(const QJsonObject& request) {
    QString sender = request["sender"].toString();
    QString groupName = request["groupName"].toString();
    QString invitee = request["invitee"].toString();

    if (groupRepo.inviteMember(groupName, sender, invitee)) {
        emit responseReady(ResponseFactory::createGroupInviteResponse(true, "Invitation sent successfully", groupName, invitee));

        // Notify invitee
        emit responseReady(NotificationFactory::createGroupInviteNotification(groupName, sender, invitee));
    } else {
        Logger::error("Failed to send group invitation from " + sender + " to " + invitee + " for group " + groupName);
        emit responseReady(ResponseFactory::createGroupInviteResponse(false, "Failed to send group invitation", groupName, invitee));
    }
}

void GroupHandler::handleGroupInviteResponse(const QJsonObject& request) {
    QString sender = request["sender"].toString();
    QString groupName = request["groupName"].toString();
    bool accept = request["accept"].toBool();

    if (accept) {
        if (groupRepo.acceptGroupInvitation(groupName, sender)) {
            emit responseReady(ResponseFactory::createGroupInviteResponseAck(true, true, "Invitation accepted", groupName, sender));

            // Notify group creator
            QString creator = groupRepo.getGroupCreator(groupName);
            emit responseReady(NotificationFactory::createGroupInviteResponseNotification(groupName, sender, true, creator));
        } else {
            Logger::error("Failed to accept invitation for group: " + groupName + " by " + sender);
            emit responseReady(ResponseFactory::createGroupInviteResponseAck(false, true, "Failed to accept invitation", groupName, sender));
        }
    } else {
        if (groupRepo.rejectGroupInvitation(groupName, sender)) {
            emit responseReady(ResponseFactory::createGroupInviteResponseAck(true, false, "Invitation rejected", groupName, sender));

            // Notify group creator
            QString creator = groupRepo.getGroupCreator(groupName);
            emit responseReady(NotificationFactory::createGroupInviteResponseNotification(groupName, sender, false, creator));
        } else {
            Logger::error("Failed to reject invitation for group: " + groupName + " by " + sender);
            emit responseReady(ResponseFactory::createGroupInviteResponseAck(false, false, "Failed to reject invitation", groupName, sender));
        }
    }
}