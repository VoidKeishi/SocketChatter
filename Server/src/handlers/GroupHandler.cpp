#include "GroupHandler.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"
#include "../utils/NotificationFactory.h"
#include "../core/ConnectionManager.h"

GroupHandler::GroupHandler(DatabaseManager* db) 
    : BaseHandler(), groupRepo(db) 
{
    handlers = {
        {"GROUP_CREATE", [this](const QJsonObject& p) { handleCreateGroup(p); }},
        {"GROUP_INVITE", [this](const QJsonObject& p) { handleInviteMember(p); }},
        {"GROUP_ACCEPT", [this](const QJsonObject& p) { handleAcceptInvitation(p); }},
        {"GROUP_REJECT", [this](const QJsonObject& p) { handleRejectInvitation(p); }},
        {"GROUP_LEAVE", [this](const QJsonObject& p) { handleLeaveGroup(p); }},
    };
}

void GroupHandler::handleCreateGroup(const QJsonObject& request) {
    QString groupName = request["groupName"].toString();
    QString creator = request["username"].toString();
    
    if (groupRepo.createGroupChat(groupName, creator);) {
        emit responseReady(ResponseFactory::createGroupResponse(true, "Group created successfully", groupName));
    } else {
        Logger::error("Failed to create group");
        emit responseReady(ResponseFactory::createGroupResponse(false, "Failed to create group", groupName));
    }
}

void GroupHandler::handleInviteMember(const QJsonObject& request) {
    QString groupId = request["groupId"].toString();
    QString sender = request["sender"].toString();
    QString receiver = request["receiver"].toString();
    
    if (groupRepo.inviteGroupMember(groupId, receiver)) {
        emit responseReady(ResponseFactory::groupInviteResponse(true, "Invitation sent", sender, receiver));
        // Notify the receiver
        QJsonObject notification = NotificationFactory::createGroupInviteNotification(groupId, sender, receiver);
        ConnectionManager::instance()->sendToClient(receiver, notification);
    } else {
        Logger::error("Failed to send invitation");
        emit responseReady(ResponseFactory::groupInviteResponse(false, "Failed to send invitation", sender, receiver));
    }
}

void GroupHandler::handleAcceptInvitation(const QJsonObject& request) {
    QString groupId = request["groupId"].toString();
    QString username = request["username"].toString();

    // Get group info
    auto groupInfo = groupRepo.getGroupById(groupId);
    if (groupInfo.first.isEmpty()) {
        Logger::error("Group not found");
        emit responseReady(ResponseFactory::groupInviteResponseAck(false, true, "Group not found", "", ""));
        return;
    }

    if (groupRepo.acceptGroupInvitation(groupId, username)) {
        emit responseReady(ResponseFactory::groupInviteResponseAck(true, true, "Invitation accepted", groupInfo.first, username));
        
        // Notify members with actual group name
        QJsonObject notification = NotificationFactory::createGroupInviteResponseNotification(
            groupId, 
            groupInfo.first, // group name
            username, 
            true
        );
        ConnectionManager::instance()->sendToClient(groupInfo.second, notification); // notify creator
    } else {
        Logger::error("Failed to accept invitation");
        emit responseReady(ResponseFactory::groupInviteResponseAck(false, true, "Failed to accept invitation", groupInfo.first, username));
    }
}

void GroupHandler::handleRejectInvitation(const QJsonObject& request) {
    QString groupId = request["groupId"].toString();
    QString username = request["username"].toString();
    
    // Get group info
    auto groupInfo = groupRepo.getGroupById(groupId);
    if (groupInfo.first.isEmpty()) {
        Logger::error("Group not found");
        emit responseReady(ResponseFactory::groupInviteResponseAck(false, false, "Group not found", "", ""));
        return;
    }

    if (groupRepo.rejectGroupInvitation(groupId, username)) {
        emit responseReady(ResponseFactory::groupInviteResponseAck(true, false, "Invitation rejected", groupInfo.first, username));
        
        // Notify members
        QJsonObject notification = NotificationFactory::createGroupInviteResponseNotification(
            groupId,
            groupInfo.first,
            username,
            false
        );
        ConnectionManager::instance()->sendToClient(groupInfo.second, notification); // notify creator
    } else {
        Logger::error("Failed to reject invitation");
        emit responseReady(ResponseFactory::groupInviteResponseAck(false, false, "Failed to reject invitation", groupInfo.first, username));
    }
}

void GroupHandler::handleLeaveGroup(const QJsonObject& request) {
    QString groupId = request["groupId"].toString();
    QString username = request["username"].toString();
    
    // Get group info
    auto groupInfo = groupRepo.getGroupById(groupId);
    if (groupInfo.first.isEmpty()) {
        Logger::error("Group not found");
        emit responseReady(ResponseFactory::groupLeaveResponse(false, "Group not found", "", ""));
        return;
    }

    if (groupRepo.leaveGroupChat(groupId, username)) {
        emit responseReady(ResponseFactory::groupLeaveResponse(true, "Left group successfully", groupInfo.first, username));
        
        // Notify members
        QJsonObject notification = NotificationFactory::createGroupMemberLeftNotification(
            groupId,
            groupInfo.first,
            username
        );
        ConnectionManager::instance()->sendToClient(groupInfo.second, notification); // notify creator
    } else {
        Logger::error("Failed to leave group");
        emit responseReady(ResponseFactory::groupLeaveResponse(false, "Failed to leave group", groupInfo.first, username));
    }
}